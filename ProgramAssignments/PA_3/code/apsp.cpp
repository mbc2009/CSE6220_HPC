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
                  // Compare with the operation of Plus in spgemm
                  // Treat all zero as infinity, mistake may happen
                  // as the diagonal line is actually zero.
                  // But we know the location of the errowr ------ Diagonal line
                  [](int a, int b){ if(a==0) return b;
                                    else return std::min(a,b); },
                  // Plus the current elements included in the sparse matrix
                  // The current matrix do not have an infinite value
                  // so that we can treat the zeros as zero itself
                  // and do not cause errows.
                  [](int a, int b){ return a+b; }, row_comm, col_comm);
        // Replace diagonal entries with 0
        for (auto &entry : L) {
            if (entry.first.first == entry.first.second) {
                entry.second = 0;
            }
        }
        max_iter*=2;
      }

      // Give the L to the result
      result = std::move(L);
}
