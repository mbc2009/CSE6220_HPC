#include <stdio.h>
#include <ctime>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <mpi.h>


double pi_calc(long int n) {
    
    // Write your code below
    ////////////////////////////////////////
    // Manage the processors: if n not divisible, add 1 for each processor label <= n%p
    int size,rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_size(MPI_COMM_WORLD, &rank);
    int residue = n%size;
    int points=n/size;
    if (rank < residue){
        psize[i] = points+1;
    }else{
        psize[i] = points;
    }
    // Generate Random vectors in a list l, with order in [x1,y1,x2,y2]
    int dimension = 2;
    int total = dimension * points;
    float* vectors = new float[total];
    srand(time(NULL) + rank);
    for(int i = 0; i < total; i++){
        vectors[i] = static_cast <float> rand() / RAND_MAX;
    }
    // Calculate squared distance (x^2 + y^2)
    float* distance = new float[points];
    for(int i = 0; i < points; i++){
        float x = vectors[2 * i];
        float y = vectors[2 * i + 1];
        float r = x*x +y*y;
        distance[i] = r
    }

    // Wait until all processors finished, add all processor list to processor rank 0


    // calculate pi

    ////////////////////////////////////////
    return 0.0;
}
