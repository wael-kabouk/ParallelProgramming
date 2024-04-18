#include<stdio.h>
#include<mpi.h>

#define RANGE 80

int main(int argc, char **argv)
{
    int size, rank; 
    double array[RANGE];
    double arr[20];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    // Initialize the array
    if(rank == 0)
        for(int i = 0; i < RANGE; i++){
            array[i] = 0.1*i;
     }

    MPI_Scatter(array, 20, MPI_DOUBLE, arr, 20, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    printf("%d. Received data:\n", rank);
    for (int i = 0; i < 20; i++)
        printf("%lf, ", arr[i]);
    printf("\n");

    MPI_Finalize();
    return 0;
}