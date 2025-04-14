#include <vector>
#include <map>
#include <algorithm>
#include <utility>
#include <iostream>
#include <mpi.h>
#include <cassert>
#include "functions.h"


void SUMMA(int m, int p, int n,
    int *row_ptr_A, int *idx_A, int *values_A,
    int *row_ptr_B, int *idx_B, int *values_B,
    std::map<std::pair<int, int>, int> &C_block,
    std::function<int(int, int)> plus, std::function<int(int, int)> times,
    MPI_Comm row_comm, MPI_Comm col_comm)
    {
    /*
    Summa requires the following steps:
    1: k-th iteration
    2: Broadcast the k-th block of A to all processors in the row
    3: Broadcast the k-th block of B to all processors in the column
    3: Compute the C_block = A_block * B_block
    */
    int row_rank, col_rank, row_size, col_size;
    MPI_Comm_rank(row_comm, &col_rank);
    MPI_Comm_rank(col_comm, &row_rank);
    MPI_Comm_size(row_comm, &col_size);
    MPI_Comm_size(col_comm, &row_size);
    for (int k=0; k<col_size; k++){
        int nnz_A = 0;
        int nnz_B = 0;
        int ni_A = 0;
        int ni_B = 0;
        int *tmp_A_row=nullptr;
        int *tmp_A_idx=nullptr;
        int *tmp_A_v=nullptr;
        int *tmp_B_row=nullptr;
        int *tmp_B_idx=nullptr;
        int *tmp_B_v=nullptr;

        // Step 1: Broadcast the k-th block of A row wise
        // Generate the temporary CSR
        if (col_rank == k) {
            ni_A = m;
            nnz_A = row_ptr_A[ni_A];
            tmp_A_row = new int[ni_A + 1]; 
            tmp_A_idx = new int[nnz_A];
            tmp_A_v   = new int[nnz_A];
            for (int i = 0; i <= ni_A; i++) {
                tmp_A_row[i] = row_ptr_A[i];
            }
            for (int i = 0; i < nnz_A; i++) {
                tmp_A_idx[i] = idx_A[i];
                tmp_A_v[i]   = values_A[i];
            }
        }
        MPI_Bcast(&nnz_A, 1, MPI_INT, k, row_comm);
        MPI_Bcast(&ni_A, 1, MPI_INT, k, row_comm);
        if (col_rank != k){
            tmp_A_row = new int[ni_A+1];
            tmp_A_idx = new int[nnz_A];
            tmp_A_v = new int[nnz_A];
        }
        MPI_Bcast(tmp_A_row, ni_A + 1, MPI_INT, k, row_comm);
        MPI_Bcast(tmp_A_idx, nnz_A, MPI_INT, k, row_comm);
        MPI_Bcast(tmp_A_v, nnz_A, MPI_INT, k, row_comm);
        
        // Step 2: Broadcast the k-th block of B column wise
        if(row_rank == k){
            ni_B = p;
            nnz_B = row_ptr_B[ni_B];
            tmp_B_row = new int[ni_B+1];
            tmp_B_idx = new int[nnz_B];
            tmp_B_v = new int[nnz_B];

            for (int i = 0; i <= ni_B; i++) {
                tmp_B_row[i] = row_ptr_B[i];
            }
            for (int i = 0; i < nnz_B; i++) {
                tmp_B_idx[i] = idx_B[i];
                tmp_B_v[i] = values_B[i];
            }
        }
        MPI_Bcast(&nnz_B, 1, MPI_INT, k, col_comm);
        MPI_Bcast(&ni_B, 1, MPI_INT, k, col_comm);
        if (row_rank != k){
            tmp_B_row = new int[ni_B+1];
            tmp_B_idx = new int[nnz_B];
            tmp_B_v = new int[nnz_B];
        }
        MPI_Bcast(tmp_B_row, ni_B+1, MPI_INT, k, col_comm);
        MPI_Bcast(tmp_B_idx, nnz_B, MPI_INT, k, col_comm);
        MPI_Bcast(tmp_B_v, nnz_B, MPI_INT, k, col_comm);
        // Step 3: Compute the C_block = A_block * B_block
        for(int i=0; i<ni_A; i++){
            for(int j=tmp_A_row[i]; j<tmp_A_row[i+1];j++){
                int A_col = tmp_A_idx[j];              
                for(int l=tmp_B_row[A_col]; l<tmp_B_row[A_col+1]; l++){
                    int B_col = tmp_B_idx[l];
                    std::pair<int, int> coord = {i, B_col};
                    C_block[coord] = plus(C_block[coord], times(tmp_A_v[j], tmp_B_v[l]));
                }
            }
        }
    }   
}


void COO2CSR(int m, int n,
             std::vector<std::pair<std::pair<int,int>, int>> &matrix,
             int* &row_ptr, int* &idx, int* &values){


   int lenth = matrix.size();
   row_ptr = new int[m + 1]();
   idx = new int[lenth];
   values = new int[lenth];

   // Count entries per row
   for (int i = 0; i < lenth; i++) {
       int row = matrix[i].first.first;
       row_ptr[row + 1]++;
       idx[i] = matrix[i].first.second;
       values[i] = matrix[i].second;

   }

   // Prefix sum
   for (int i = 0; i < m; i++) {
       row_ptr[i + 1] += row_ptr[i];
   }
}

void MTX2COO(
            std::map<std::pair<int, int>, int> C_block,
            std::vector<std::pair<std::pair<int,int>, int>> &matrix
        )
{
    // Convert the Sparse matrix to COO format
    // Assume all the entries exists are nonzero
    for (auto &[key, val] : C_block) {
        int global_i = key.first ;
        int global_j = key.second ;
        matrix.push_back(std::make_pair(std::make_pair(global_i, global_j), val));
    }
}



void spgemm_2d(int m, int p, int n,
    std::vector<std::pair<std::pair<int,int>, int>> &A,
    std::vector<std::pair<std::pair<int,int>, int>> &B,
    std::vector<std::pair<std::pair<int,int>, int>> &C,
    std::function<int(int, int)> plus, std::function<int(int, int)> times,
    MPI_Comm row_comm, MPI_Comm col_comm)
{
int rank, size;
MPI_Comm_rank(row_comm, &rank);
MPI_Comm_size(row_comm, &size);
int col_rank, row_rank;
MPI_Comm_rank(row_comm, &col_rank);  
MPI_Comm_rank(col_comm, &row_rank);
// Step 1: Convert COO to CSR
// Treat the CSR as a part of the total matrix,
// with the index represent the original positions
int *row_ptr_A, *idx_A, *values_A;
int *row_ptr_B, *idx_B, *values_B;
COO2CSR(m, p, A, row_ptr_A, idx_A, values_A);
COO2CSR(p, n, B, row_ptr_B, idx_B, values_B);

// Step 2: create a matrix of C, and do SUMMA algorithm

std::map<std::pair<int, int>, int> C_block;
SUMMA(m, p, n,
 row_ptr_A, idx_A, values_A,
 row_ptr_B, idx_B, values_B,
 C_block, 
 plus, times,
 row_comm, col_comm);

// Step 3: Convert the matrix form of C to COO
MTX2COO( C_block, C);

// Finally, free the memory
delete[] row_ptr_A;
delete[] idx_A;
delete[] values_A;
delete[] row_ptr_B;
delete[] idx_B;
delete[] values_B;
}


