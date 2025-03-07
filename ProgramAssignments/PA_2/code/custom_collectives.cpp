#include <iostream> 
#include <mpi.h>

#include "custom_collectives.h"


////////////////////////////////////////////////////////////////
// All Reduce (Hypercubic permutation method)
void custom_allreduce_sum(int *local, int *global, int num_elem, int rank, int size) {
  /*
  Parameters:
    local: the pointer to the local data
    global: the pointer to the global data
    num_elem: the number of elements in the data
    rank: the rank of the process
    size: the number of processes
  */

  //// 1. Initialization ////
  // create a local buffer to store the received data
  int* buffer = new int[num_elem];

  //// 2. Hypercubic permutation method ////
  /*
  d                 = the number of dimensions
  1 << d            := 2^d, a mask for ``flip operation''
  rank ^ (1 << d)   := rank XOR 2^d, ``flip operation'' flip the d-th bit of rank
  partner           = rank XOR 2^d,i the partner processpr of rank-th processor
  for loop          : transverse d = 0, 1, ..., ceil(log2(size))-1, e.g. 2^3 = 8 => (0, 1, 2)
  */
  // for each dimention d
  for (int d = 0; (1 << d) < size; d++) { 
    // find the partner process for the current rank of the process
    int partner = rank ^ (1 << d); 

    // only communicate with valid partners
    if (partner < size) {
      // for avodiing deadlock
      /*
      Each process can either send or receive data to/from another process.
      Therefore deadlock occurs when two processes in the pipeline trying to both send data to (or both receive data from) each other at the same time
      */
      if (rank < partner) {
        // lower rank send first
        MPI_Send(local,  num_elem, MPI_INT, partner, d, MPI_COMM_WORLD);
        MPI_Recv(buffer, num_elem, MPI_INT, partner, d, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      } else {
        // higher rank receive first
        MPI_Recv(buffer, num_elem, MPI_INT, partner, d, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(local,  num_elem, MPI_INT, partner, d, MPI_COMM_WORLD);
      }
      // conduct the sum operation
      for (int i = 0; i < num_elem; i++) {
        local[i] += buffer[i];
      }
    }
  }
  // free the loacl buffer
  delete[] buffer;

  //// 3. save the result to the global buffer ////
  for (int i = 0; i < num_elem; i++) {
    global[i] = local[i];
  }
}




////////////////////////////////////////////////////////////////////////////////
int custom_many2many(int *send_data, int *sendcounts, int** recv_data_ptr, int rank, int size) {
  /*
    Parameters:
      - send_data: Pointer to the data array to be sent.
      - sendcounts: Array of length 'size'. sendcounts[j] indicates the number of elements
                     this process sends to process j.
      - recv_data_ptr: Pointer to a pointer for the received data. This function will allocate
                      the buffer for the received data and assign its address to *recv_data_ptr.
      - rank: The rank of the current process.
      - size: The total number of processes.

    Return:
        - The total number of elements received.
   */

  // STEP 0: Allocate memory for a full p x p matrix to hold all processes' sendcounts.
  // The matrix is stored in row-major order, where row i contains the sendcounts of process i.
  int* all_sendcounts = new int[size * size];
  /*
    Copy the sendcounts of the current process (its own row) into all_sendcounts.
    For row corresponding to this process (rank), set:
    all_sendcounts[rank * size + j] = number of elements this process sends to process j.
  */
  for (int j = 0; j < size; j++) {
    all_sendcounts[rank * size + j] = sendcounts[j];
  }

  // STEP 1: Exchange sendcounts with all other processes (manual allgather).
  /*
    Each process will exchange its row (sendcounts array) with every other process.
    To avoid deadlock, if rank < i then send first then receive; otherwise, receive first then send.
  */
  for (int i = 0; i < size; i++) {
    if (i == rank) continue; // Skip self
    if (rank < i) {
      // Lower-ranked process: send my row, then receive row from process i.
      MPI_Send(all_sendcounts + rank * size, size, MPI_INT, i, 100, MPI_COMM_WORLD);
      MPI_Recv(all_sendcounts + i * size,     size, MPI_INT, i, 100, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {
      // Higher-ranked process: receive row from process i, then send my row.
      MPI_Recv(all_sendcounts + i * size, size, MPI_INT, i, 100, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Send(all_sendcounts + rank * size, size, MPI_INT, i, 100, MPI_COMM_WORLD);
    }
  }

  // STEP 2: Calculate the total number of elements to be received and the offset for each source.
  /*
      For the current process, the number of elements to receive from process i is:
      recvcount_from[i] = all_sendcounts[i*size + rank]
  */
  int* recvcount_from = new int[size];
  for (int i = 0; i < size; i++) {
    recvcount_from[i] = all_sendcounts[i * size + rank];
  }
  // Compute the total number of elements to be received.
  int total_recv = 0;
  for (int i = 0; i < size; i++) {
    total_recv += recvcount_from[i];
  }
  // Allocate memory for the final receive buffer.
  int* final_recv = new int[total_recv];

  // Compute the receive offsets for each source process.
  /*
    recv_offset[i] indicates the starting index in final_recv for data received from process i.
  */
  int* recv_offset = new int[size];
  recv_offset[0] = 0;
  for (int i = 1; i < size; i++) {
    recv_offset[i] = recv_offset[i - 1] + recvcount_from[i - 1];
  }

  // STEP 3: Compute the send offsets.
  /*
    send_offset[j] indicates the starting index in send_data for data that should be sent to process j.
    send_offset[0] = 0, and send_offset[j] = send_offset[j-1] + sendcounts[j-1].
  */
  int* send_offset = new int[size];
  send_offset[0] = 0;
  for (int j = 1; j < size; j++) {
    send_offset[j] = send_offset[j - 1] + sendcounts[j - 1];
  }
  
  // STEP 4: Exchange data with other processes using a ring-based approach with message splitting.
  /*
    For each round j (from 1 to size-1):
      - Determine the destination:  dest   = (rank + j) mod size.
      - Determine the source:       source = (rank - j + size) mod size.
      - Instead of sending the entire message at once, split the message into smaller chunks.
      - Divide the message size by 'size' (number of processes, P) to obtain a chunk size.
  */
  for (int j = 1; j < size; j++) {
    int dest = (rank + j) % size;
    int source = (rank - j + size) % size;

    // Determine the number of elements to send to 'dest' and to receive from 'source'.
    int send_count = all_sendcounts[rank * size + dest]; // Same as sendcounts[dest]
    int recv_count = all_sendcounts[source * size + rank];

    // Compute chunk sizes for sending and receiving.
    /*
      We divide the total message length by the number of processes to determine
      Size of each block = m/P
       (send_count + size - 1): - 1 is added to ensure that the division rounds up.
       (send_count + size - 1) / size: This is equivalent to ceil(send_count / size).
    */ 
    int chunk_size_send = (send_count + size - 1) / size;
    int chunk_size_recv = (recv_count + size - 1) / size;

    // Define the number of steps (chunks) to iterate.
    int steps = std::max(size, 1); // Ensure at least one step to avoid division by zero

    // Loop over each chunk.
    for (int c = 0; c < steps; c++) {
      // Compute offset and length for the sending chunk.
      int off_s = c * chunk_size_send;
      if (off_s >= send_count) break; // No more data to send in this round.
      int len_s = std::min(chunk_size_send, send_count - off_s);

      // Compute offset and length for the receiving chunk.
      int off_r = c * chunk_size_recv;
      if (off_r >= recv_count) break; // No more data to receive.
      int len_r = std::min(chunk_size_recv, recv_count - off_r);

      // To avoid deadlock, use a convention:
      /*
        if (rank < dest) then send first and then receive; else, receive first and then send.
      */
      if (rank < dest) {
        MPI_Send(send_data + send_offset[dest] + off_s, len_s, MPI_INT, dest, 200 + j, MPI_COMM_WORLD);
        MPI_Recv(final_recv + recv_offset[source] + off_r, len_r, MPI_INT, source, 200 + j, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      } else {
        MPI_Recv(final_recv + recv_offset[source] + off_r, len_r, MPI_INT, source, 200 + j, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(send_data + send_offset[dest] + off_s, len_s, MPI_INT, dest, 200 + j, MPI_COMM_WORLD);
      }
    }
  }

  // STEP 5: Process the data that the process sends to itself.
  /*
    For j = 0, simply copy the data from send_data to final_recv.
  */
  int self_count = all_sendcounts[rank * size + rank]; // This equals sendcounts[rank]
  for (int k = 0; k < self_count; k++) {
    final_recv[recv_offset[rank] + k] = send_data[send_offset[rank] + k];
  }

  // STEP 6: Clean up temporary arrays and return the result.
  delete[] all_sendcounts;
  delete[] recvcount_from;
  delete[] recv_offset;
  delete[] send_offset;

  *recv_data_ptr = final_recv;
  return total_recv;
}

