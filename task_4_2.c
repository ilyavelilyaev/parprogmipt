#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define COUNT 10000000


/// The idea is that first process divides the array in equal parts, send start and end of the part to each remaining process.
/// Each other process counts the sum from start to end and send back to first process the result.
/// First process sums up everything and prints out the result.

int main(int argc, char *argv[]) {

    int *array = (int *)malloc(COUNT * sizeof(int));
    for (int i = 0; i < COUNT; i++) {
        array[i] = 1;
    }

    MPI_Init(&argc, &argv);

    int idx;
    MPI_Comm_rank(MPI_COMM_WORLD, &idx);

    int n;
    MPI_Comm_size(MPI_COMM_WORLD, &n);

    if (n == 1) {
        printf("Cannot operate on one thread\n");

        MPI_Finalize();

        return 1;
    }

    //range[0] - start idx, range[1] - end idx
    int *range = (int *)malloc(sizeof(int) * 2);
    range[0] = 0;
    range[1] = 0;


    /// First process, the main. Will divide the array into subarrays and sum up.
    if (idx == 0) {

        int workers = n - 1;
        int delta = COUNT / workers;

        for (int i = 0; i < workers; i++) {
            range[0] = i * delta;
            range[1] = (i + 1) * delta;

            if (i == workers - 1) {
                range[1] = COUNT;
            }

            MPI_Send(range, 2, MPI_INT, i + 1, 0, MPI_COMM_WORLD);
        }

        int result = 0;

        for (int i = 0; i < workers; i ++) {
            int current = 0;
            MPI_Recv(&current, 1, MPI_INT, i + 1, 1, MPI_COMM_WORLD, NULL);
            result += current;
        }

        printf("Finished: %d\n", result);

        MPI_Finalize();

        return 0;
    }

    /// Other processes:

    MPI_Recv(range, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, NULL);

    int current = 0;
    for (int i = range[0]; i < range[1]; i++) {
        current += array[i];
    }

    MPI_Send(&current, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    
    MPI_Finalize();
    
    return 0;
    
}
