#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define COUNT 10000000


/// The idea is that first process divides the array in equal parts, send start and end of the part to each remaining process.
/// Each other process counts the sum from start to end and send back to first process the result.
/// First process sums up everything and prints out the result.

/// To count effeciency, first process will count the sum sequently, and send the time to second. Second works as first process in previous task.

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

    if (n < 3) {
        printf("Cannot operate on less then 3 threads\n");

        MPI_Finalize();

        return 1;
    }

    /// First. Will count sequently and send time to second process.
    if (idx == 0) {
        int result = 0;

        double startTime = MPI_Wtime();

        for (int i = 0; i < COUNT; i++) {
            result += array[i];
        }

        double endTime = MPI_Wtime();

        printf("Finished consequently: %d\n", result);

        double time = endTime - startTime;

        MPI_Send(&time, 1, MPI_DOUBLE, 1, 2, MPI_COMM_WORLD);

        MPI_Finalize();

        return 0;
    }

    //range[0] - start idx, range[1] - end idx
    int *range = (int *)malloc(sizeof(int) * 2);
    range[0] = 0;
    range[1] = 0;


    /// Second process, the main. Will divide the array into subarrays and sum up.
    if (idx == 1) {

        int workers = n - 2;
        int delta = COUNT / workers;


        double startTime = MPI_Wtime();

        for (int i = 0; i < workers; i++) {
            range[0] = i * delta;
            range[1] = (i + 1) * delta;

            if (i == workers - 1) {
                range[1] = COUNT;
            }

            MPI_Send(range, 2, MPI_INT, i + 2, 0, MPI_COMM_WORLD);
        }

        int result = 0;

        for (int i = 0; i < workers; i ++) {
            int current = 0;
            MPI_Recv(&current, 1, MPI_INT, i + 2, 1, MPI_COMM_WORLD, NULL);
            result += current;
        }

        double endTime = MPI_Wtime();

        printf("Finished concurrent: %d\n", result);

        double consTime;

        MPI_Recv(&consTime, 1, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD, NULL);

        double concTime = endTime - startTime;

        double speedup = consTime / concTime;
        double efficiency = speedup / (n - 1);

        printf("Efficiency: %f\nSpeedup: %f\n", efficiency, speedup);

        MPI_Finalize();

        return 0;
    }

    /// Other processes:

    MPI_Recv(range, 2, MPI_INT, 1, 0, MPI_COMM_WORLD, NULL);

    int current = 0;
    for (int i = range[0]; i < range[1]; i++) {
        current += array[i];
    }

    MPI_Send(&current, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
    
    MPI_Finalize();
    
    return 0;
    
}
