
#include <mpi.h>
#include <cmath>
#include <algorithm>  // for std::min, std::max
#include <cstring>    // for memcpy
#include <iostream>

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

  // =========== STEP 1. Initialization ===========
  // create a local buffer to store the received data
  int* buffer = new int[num_elem];

  // =========== STEP 2. Hypercubic permutation method =========== 
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

  // =========== STEP 3. save the result to the global buffer  ===========
  for (int i = 0; i < num_elem; i++) {
    global[i] = local[i];
  }
}




////////////////////////////////////////////////////////////////////////////////
// All Reduce (Ring method)
int custom_many2many(int *send_data, int *sendcounts, int** recv_data_ptr, int rank, int size) {
  
  // ================== STEP 1: Exchange the send counts to get the receive counts ==================
  // create a receive counts array
  int* recvcounts = new int[size];

  // copy the send counts to the receive counts
  recvcounts[rank] = sendcounts[rank];

  // Exchange the send counts to get the receive counts
  for (int j = 0; j < size; j++) {
    if (j == rank) continue;
    if (rank < j) {
      MPI_Send(&sendcounts[j], 1, MPI_INT, j, 100, MPI_COMM_WORLD);
      MPI_Recv(&recvcounts[j], 1, MPI_INT, j, 100, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {
      MPI_Recv(&recvcounts[j], 1, MPI_INT, j, 100, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Send(&sendcounts[j], 1, MPI_INT, j, 100, MPI_COMM_WORLD);
    }
  }

  // ================== STEP 2: Calculate the total amount of data to be received and allocate the receive buffer ==================
  int total_recv = 0;
  for (int i = 0; i < size; i++) {
    total_recv += recvcounts[i];
  }
  int* recv_data = new int[total_recv];

  // Calculate the starting offset of the received data in recv_data
  int* recv_displs = new int[size];
  recv_displs[0] = 0;
  for (int i = 1; i < size; i++) {
    recv_displs[i] = recv_displs[i-1] + recvcounts[i-1];
  }

  // Calculate the starting offset of the send data in send_data
  int* send_displs = new int[size];
  send_displs[0] = 0;
  for (int i = 1; i < size; i++) {
    send_displs[i] = send_displs[i-1] + sendcounts[i-1];
  }


  // ================== STEP 3: Copy the data from the send buffer to the receive buffer ==================
  if (sendcounts[rank] > 0) {
    // Copy the data from the send buffer to the receive buffer
    for (int idx = 0; idx < sendcounts[rank]; idx++) {
      recv_data[recv_displs[rank] + idx] = send_data[send_displs[rank] + idx];
    }
  }

  // ================== STEP 4: Cyclic shit communication ==================
  for (int k = 1; k < size; k++) {
    // send to partner_send
    int partner_send = (rank + k) % size;
    // recv from partner_recv
    int partner_recv = (rank - k + size) % size;

    // send data size
    int send_count = sendcounts[partner_send];
    // recv data size
    int recv_count = recvcounts[partner_recv];

    // starting point of the send data
    int send_offset = send_displs[partner_send];
    // starting point of the recv data
    int recv_offset = recv_displs[partner_recv];

    // avoid self communication
    if (rank < partner_send) {
      // send to partner_send
      MPI_Send(send_data + send_offset, send_count, MPI_INT, partner_send, 200, MPI_COMM_WORLD);
      // recv from partner_recv
      MPI_Recv(recv_data + recv_offset, recv_count, MPI_INT, partner_recv, 200, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else if (rank > partner_send) {
      // recv from partner_recv
      MPI_Recv(recv_data + recv_offset, recv_count, MPI_INT, partner_recv, 200, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      // send to partner_send
      MPI_Send(send_data + send_offset, send_count, MPI_INT, partner_send, 200, MPI_COMM_WORLD);
    } else {
    }
  }

  // ================== STEP 5: Return the receive buffer to the caller ==================
  delete[] recvcounts;
  delete[] recv_displs;
  delete[] send_displs;

  // return the receive buffer to the caller
  *recv_data_ptr = recv_data;
  return total_recv;
}

