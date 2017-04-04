#include <mpi.h>
#include <stdio.h>


int main(int argc, char *argv[]) {
    
    
    MPI_Init(&argc, &argv);
    
    int idx;
    MPI_Comm_rank(MPI_COMM_WORLD, &idx);

    int n;
    MPI_Comm_size(MPI_COMM_WORLD, &n);
    
    if (idx == 0) {
        MPI_Send(NULL, 0, MPI_CHAR, idx + 1, 0, MPI_COMM_WORLD);
        
        printf("I'm %d\n", idx);
        
        MPI_Finalize();
        
        return 0;
    }
    
    
    MPI_Recv(NULL, 0, MPI_CHAR, idx - 1, 0, MPI_COMM_WORLD, NULL);
    
    printf("I'm %d\n", idx);
    
    if (idx != n - 1) {
        MPI_Send(NULL, 0, MPI_CHAR, idx + 1, 0, MPI_COMM_WORLD);
    }
    
    MPI_Finalize();
    
    return 0;
   
}
