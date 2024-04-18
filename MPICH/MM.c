#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>
#include <mpi.h>

#define N 100

void print_results(char *prompt, double a[N]);

int main(int argc, char *argv[]) {

    double start_time, end_time;

	double interval;
    srand(time(NULL)); 

    int i, j, rank, size;
    double A[N][N], b[N], c[N];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Initialize matrix A in process 0
    if (rank == 0) {
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                A[i][j] = 2;
            }
        }
        for (i = 0; i < N; i++) {
            b[i] = 1;
        }
    }

    // Broadcast vector b to all processes
    MPI_Bcast(b, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Scatter rows of matrix A to different processes
    double local_A[N/size][N];
    MPI_Scatter(A, N*N/size, MPI_DOUBLE, local_A, N*N/size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    printf("%d. Received data\n", rank);

    start_time = MPI_Wtime();
    // Compute local portion of matrix-vector multiplication
    double local_c[N/size];
    for (i = 0; i < N/size; i++) {
        local_c[i] = 0.0;
        for (j = 0; j < N; j++) {
            local_c[i] += local_A[i][j] * b[j];
        }
    }

    // Gather results from all processes
    MPI_Gather(local_c, N/size, MPI_DOUBLE, c, N/size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
   
    end_time = MPI_Wtime();
    MPI_Finalize();

    // Print results in process 0
    if (rank == 0) {
        printf("Elapsed time: %f ms\n", (end_time - start_time) * 1000.0);
        print_results("C = ", c);
    }

    return 0;
}

void print_results(char *prompt, double a[N]) {
    printf("\n%s\n", prompt);
    for (int i = 0; i < N; i++) {
        printf("%f ", a[i]);
    }
    printf("\n");
}
