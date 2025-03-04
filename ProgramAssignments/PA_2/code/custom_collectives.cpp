#include <iostream> 
#include <mpi.h>

#include "custom_collectives.h"


////////////////////////////////////////////////////////////////
// All Reduce (Hypercubic permutation method)
void custom_allreduce_sum(int *local, int *global, int num_elem, int rank, int size) {
  //// 1. Initialization ////
  // create a buffer to store the received data
  int* buffer = new int[num_elem];

  //// 2. Hypercubic permutation method ////
  // transverse d = 0, 1, ..., ceil(log2(size))-1
  for (int d = 0; (1 << d) < size; d++) {
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
        MPI_Send(local, num_elem, MPI_INT, partner, d, MPI_COMM_WORLD);
        MPI_Recv(buffer, num_elem, MPI_INT, partner, d, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      } else {
        // higher rank receive first
        MPI_Recv(buffer, num_elem, MPI_INT, partner, d, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(local, num_elem, MPI_INT, partner, d, MPI_COMM_WORLD);
      }
      // conduct the sum operation
      for (int i = 0; i < num_elem; i++) {
        local[i] += buffer[i];
      }
    }
  }
  // free the buffer
  delete[] buffer;

  //// 3. save the result to the global buffer ////
  for (int i = 0; i < num_elem; i++) {
    global[i] = local[i];
  }
}




////////////////////////////////////////////////////////////////////////////////
// Many-to-one (Arbiturary permutation method)
int custom_many2many(int *send_data, int *sendcounts, int** recv_data_ptr, int rank, int size) {
  
  //// 1. Initialization ////
  // Swittch the number of data each process sends to others 
  int* recvcounts = new int[size];

  // copy the number of data to send to itself, saving extra communication resource
  recvcounts[rank] = sendcounts[rank];

  //// 2. Communication ////
  // transverse all other processes
  for (int j = 0; j < size; j++) {
    // skip itself
    if (j == rank) continue; 
    
    // Avoid deadlock 
    /*
    Each process can either send or receive data to/from another process.
    Therefore deadlock occurs when two processes in the pipeline trying to both send data to (or both receive data from) each other at the same time
    */
    if (rank < j) {
      // ower rank send first
      MPI_Send(&sendcounts[j], 1, MPI_INT, j, 100, MPI_COMM_WORLD);
      MPI_Recv(&recvcounts[j], 1, MPI_INT, j, 100, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else { // rank > j
      // higher rank receive first
      MPI_Recv(&recvcounts[j], 1, MPI_INT, j, 100, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Send(&sendcounts[j], 1, MPI_INT, j, 100, MPI_COMM_WORLD);
    }
  }
  
  //// 3. calculate the total length of received data and allocate the receive buffer ////
  int total_recv = 0;
  for (int i = 0; i < size; i++) {
    total_recv += recvcounts[i];
  }
  int* recv_data = new int[total_recv];

  // calculate the starting position of received data in recv_data
  int* displs = new int[size];
  displs[0] = 0;
  for (int i = 1; i < size; i++) {
    displs[i] = displs[i-1] + recvcounts[i-1];
  }
  
  // calculate the starting position of send data in send_data
  int* send_displs = new int[size];
  send_displs[0] = 0;
  for (int i = 1; i < size; i++) {
    send_displs[i] = send_displs[i-1] + sendcounts[i-1];
  }
  
  //// 4. Exchange the data between processes ////
  for (int i = 0; i < size; i++) {
    if (i == rank) { // slect diagonal elements
      // copy the data from send_data to recv_data
      for (int k = 0; k < sendcounts[rank]; k++) {
        recv_data[displs[rank] + k] = send_data[send_displs[rank] + k];
      }
    } else {
      // Avoid deadlock
      /*
      Each process can either send or receive data to/from another process.
      Therefore deadlock occurs when two processes in the pipeline trying to both send data to (or both receive data from) each other at the same time
      */
      if (rank < i) {
        // lower rank send first
        MPI_Send(send_data + send_displs[i], sendcounts[i], MPI_INT, i, 200, MPI_COMM_WORLD);
        MPI_Recv(recv_data + displs[i], recvcounts[i], MPI_INT, i, 200, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      } else { // rank > i
        // higher rank receive first
        MPI_Recv(recv_data + displs[i], recvcounts[i], MPI_INT, i, 200, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(send_data + send_displs[i], sendcounts[i], MPI_INT, i, 200, MPI_COMM_WORLD);
      }
    }
  }
  
  //// 5. Post treatment ////
  // free the memory 
  delete[] recvcounts;
  delete[] displs;
  delete[] send_displs;

  //  return the pointer to the received data
  *recv_data_ptr = recv_data;
  return total_recv;
}
