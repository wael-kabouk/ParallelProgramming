#include<stdio.h>
#include<mpi.h>

#define RANGE 10

int main(int argc, char **argv)
{
    int size, rank; 
    double array[RANGE];



    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    // Initialize the array
    if(rank == 0)
        for(int i = 0; i < RANGE; i++){
            array[i] = 0.1*i;
     }

    if(rank == 0){
        MPI_Send(array+5, 5, MPI_DOUBLE, 1, 1, MPI_COMM_WORLD);
        printf("Node %d is sent the data\n", rank);
    } else if(rank == 1) { 
        printf("Receiving the data...\n");
        MPI_Recv(array+5, 5, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Node %d is received the data\n", rank);
        printf("Received data:\n");
        for(int i = 0; i < 5; i++)
            printf("%lf, ", array[5+i]);
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}