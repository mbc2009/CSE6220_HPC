#include <stdio.h>
#include <ctime>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <mpi.h>
#include <random>

double pi_calc(long int n) {
    int size, rank;

    // Initialize MPI environment variables
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Task allocation: calculate points assigned to this processor
    int residue = n % size;        // Remaining points after division
    int points = n / size;         // Basic number of points per processor
    if (rank < residue) {
        points += 1;               // Distribute residual points to lower ranks
    }

    // Generate random points in the unit square [0,1] x [0,1]
    int dimension = 2;             // x, y coordinates per point
    int total = dimension * points; 
    std::vector<float> vectors(total); // automatic memory management
    std::mt19937 rng(time(0) + rank * 100); // Seed random number generator uniquely for each rank
    std::uniform_real_distribution<float> dist(0.0, 1.0); // Generate normalized random values
    for (int i = 0; i < total; i++) {
        vectors[i] = dist(rng);
    }

    // Count points inside the unit circle
    int local_count = 0;
    for (int i = 0; i < points; i++) {
        float x = vectors[2 * i];
        float y = vectors[2 * i + 1];
        if (x * x + y * y <= 1.0) { // Check if the point lies inside the circle
            local_count++;
        }
    }

    // Reduce local counts to calculate the global count (sum over all processors)
    int global_count = 0;
    MPI_Reduce(&local_count,       // Input data address
               &global_count,      // Output data address (only valid on rank 0)
               1,                  // Number of elements to reduce
               MPI_INT,            // Data type (integer)
               MPI_SUM,            // Reduction operation (sum)
               0,                  // Root processor rank
               MPI_COMM_WORLD);    // Communication group

    // Calculate and return pi on the root processor
    if (rank == 0) {
        return 4.0 * global_count / n;
    }else{return 0;}
}
