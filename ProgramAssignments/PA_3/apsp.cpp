#include <vector>
#include <utility>
#include <array>
#include <mpi.h>
#include <map>
#include <cassert>
#include <iostream>
#include "functions.h"

void apsp(int n, std::vector<std::pair<std::pair<int, int>, int>> &graph,
          std::vector<std::pair<std::pair<int, int>, int>> &result,
          MPI_Comm row_comm, MPI_Comm col_comm)
{
    int row_rank, col_rank, row_size, col_size;
    MPI_Comm_rank(row_comm, &col_rank);
    MPI_Comm_rank(col_comm, &row_rank);
    MPI_Comm_size(row_comm, &col_size);
    MPI_Comm_size(col_comm, &row_size);
    int block_size_n = (n+row_size-1) / row_size;
    int row_start = row_rank * block_size_n;
    int row_end = std::min(n, row_start + block_size_n);

    int col_start = col_rank * block_size_n;
    int col_end = std::min(n, col_start + block_size_n);



    std::vector<std::pair<std::pair<int,int>, int>> L = graph;
    // Remain the size constraint




        /* Try to use the Floyd-Warshall algorithm
           The dynamic programming approach
           Initially, Create the D^0 matrix D=input
           With other entries set to infinity
           In each iteration, we compute the shortest path by min{D[i][j], D[i][k]+D[k][j]}
        */
    int max_iter = 1;
    while (max_iter<n){
        std::vector<std::pair<std::pair<int,int>, int>> L_tmp = std::move(L);
        spgemm_2d(n,n,n,L_tmp,L_tmp,L,
                  // TODO: Choose operation here
                  // Compare with the operation of Plus in spgemm
                  // Treat all zero as infinity
                  [](int a, int b){ if(a==0 && b==0) return 0;
                                    else if(a==0) return b;
                                    else if(b==0) return a;
                                    else return std::min(a,b); },
                  // TODO: Choose operation here
                  [](int a, int b){ if (a==0 || b==0) return 0;
                                    else return a+b; }, row_comm, col_comm);
        // After each run, the size of L is growing and indexes become global index
        // Need to check the domain of the entries and send them to the right processor
        
        std::vector<std::pair<std::pair<int, int>, int>> L_local;

        for (const auto& entry : L) {
            int i = entry.first.first;
            int row = i - row_start;
            int j = entry.first.second;
            int col = j - col_start;
        
            if (i >= row_start && i < row_end &&
                j >= col_start && j < col_end) {
                L_local.push_back({{row, col}, entry.second});
            }
        }
        L = std::move(L_local);


        // There are several possible entries are incorrect
        // Case 1, the diagonal entries are really 0, and they are removed or replaced
        // Case 2, the diagonal entries "Plus" to another edge, and remove or replace that edge
        // Do the check here and replace the entries, compare with the L_tmp
        std::map<std::pair<int, int>, int> merged;

        // Insert entries from L_tmp
        for (const auto& entry : L_tmp) {
            merged[entry.first] = entry.second;
        }
        // Keep the index with in the current block
        // Merge entries from L (output of spgemm)
        for (const auto& entry : L) {
            const auto& key = entry.first;
            int val = entry.second;

            if (merged.count(key)) {
                merged[key] = std::min(merged[key], val);  // keep smaller
            } else {
                merged[key] = val;
            }
        }

        // Reconstruct L with merged minimal entries
        L.clear();
        for (const auto& entry : merged) {
            L.push_back({entry.first, entry.second});
        }

        max_iter*=2;
        MPI_Barrier(row_comm);
        MPI_Barrier(col_comm);
      }

      //Additionally, we need to add the offset back to the entries
      for (auto& entry : L) {
        entry.first.first += row_start;
        entry.first.second += col_start;
      }
      // Give the L to the result
      result = std::move(L);
}
