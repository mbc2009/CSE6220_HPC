#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>
#include <string>
#include <sstream>
#include <map>
#include "functions.h"

void distribute_matrix_2d(int m, int n, std::vector<std::pair<std::pair<int, int>, int>> &full_matrix,
                          std::vector<std::pair<std::pair<int, int>, int>> &local_matrix,
                          int root, MPI_Comm comm_2d)
{
    /*
    This function distribute the matrix to the 2D processors spacially
    Assume the root processor is 0
    Parameters:
        - m: the number of rows of the matrix
        - n: the number of columns of the matrix
        - full_matrix: the full matrix to be distributed
        - local_matrix: the local matrix for each processor
        - root: the root processor
        - comm_2d: the 2D communicator
    */
    // Get the rank and size of processors
    int rank, size_2d;
    MPI_Comm_rank(comm_2d, &rank);
    MPI_Comm_size(comm_2d, &size_2d);
    int size;
    size=sqrt(size_2d);

    // Split and distribute the matrix to the processors
    // Split by 2D spatially and send the data to the processors immediately
    int local_m = (m+size-1) / size;
    int local_n = (n+size-1) / size;
    int length=full_matrix.size();
    if (rank == root) {
        std::map<int, std::vector<std::pair<std::pair<int, int>, int>>> buf_send;
        // The order is perserved in this case
        for (int i = 0; i < length; i++) {
            int row = full_matrix[i].first.first;
            int col = full_matrix[i].first.second;
            // Find the target processor
            int row_block = row / local_m;
            int col_block = col / local_n;
            int dest = row_block * size + col_block;
            buf_send[dest].push_back(std::make_pair(std::make_pair(row, col), full_matrix[i].second));
        }
        // Send the data to target processor
        for (int i=0; i<size_2d; i++){
            int count=buf_send[i].size();
            if (i == root){
                local_matrix=buf_send[i];
            }
            else{
                MPI_Send(&count, 1, MPI_INT, i, 0, comm_2d);
                MPI_Send(buf_send[i].data(), count * sizeof(buf_send[i][0]), MPI_BYTE, i, 1, comm_2d);
            }
        }
    }
    else{
        int count;
        MPI_Recv(&count, 1, MPI_INT, root, 0, comm_2d, MPI_STATUS_IGNORE);
        local_matrix.resize(count);
        MPI_Recv(local_matrix.data(), count * sizeof(local_matrix[0]), MPI_BYTE, root, 1, comm_2d, MPI_STATUS_IGNORE);
    }

}
