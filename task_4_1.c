#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

/// The idea is to send and receive two messages
/// First is empty, just measure time.
/// Second message contains time, measured by previous send and receive,
/// adding previously received time.

int main(int argc, char *argv[]) {

    MPI_Init(&argc, &argv);

    int idx;
    MPI_Comm_rank(MPI_COMM_WORLD, &idx);

    int n;
    MPI_Comm_size(MPI_COMM_WORLD, &n);

    double startTime, endTime;

    //time[0] - sending time, time[1] - receiving time
    double *time = (double *)malloc(sizeof(double) * 2);
    time[0] = 0;
    time[1] = 0;

    /// First process, just sending. Nothing to receive.
    if (idx == 0) {

        //Sending empty message.
        startTime = MPI_Wtime();
        MPI_Send(NULL, 0, MPI_CHAR, idx + 1, 0, MPI_COMM_WORLD);
        endTime = MPI_Wtime();

        //Calculating elapsed time on sending, printing out.
        time[0] = endTime - startTime;
        printf("I'm %d, elapsed time: sending: %f receiving: %f\n", idx, time[0], time[1]);

        //Sending result to next process
        MPI_Send(time, 2, MPI_DOUBLE, idx + 1, 1, MPI_COMM_WORLD);

        MPI_Finalize();

        return 0;
    }

    /// Other processes:


    // Receiving empty message, measuring time.
    startTime = MPI_Wtime();
    MPI_Recv(NULL, 0, MPI_CHAR, idx - 1, 0, MPI_COMM_WORLD, NULL);
    endTime = MPI_Wtime();

    // Calculating elapsed time on receiving
    double receivingTime = endTime - startTime;

    // Sending empty message, measuring time, if not last
    double sendingTime = 0;

    if (idx != n - 1) {
        startTime = MPI_Wtime();
        MPI_Send(NULL, 0, MPI_CHAR, idx + 1, 0, MPI_COMM_WORLD);
        endTime = MPI_Wtime();

        // Calculating elapsed time on sending
        sendingTime = endTime - startTime;
    }

    // Receiving time from previous process
    MPI_Recv(time, 2, MPI_DOUBLE, idx - 1, 1, MPI_COMM_WORLD, NULL);
    
    // Incrementing time to include current measures
    time[0] += sendingTime;
    time[1] += receivingTime;

    // Printing out current results.
    printf("I'm %d, elapsed time: sending: %f receiving: %f\n", idx, time[0], time[1]);

    //If not last, sending result to next process

    if (idx != n - 1) {
        MPI_Send(time, 2, MPI_DOUBLE, idx + 1, 1, MPI_COMM_WORLD);
    } else {
        // if last, calculate average, print out.
        printf("Finished!\nAverage sending time: %f\nAverage receiving time: %f\n", time[0] / (n - 1), time[1] / (n - 1));
    }

    MPI_Finalize();

    return 0;

}
