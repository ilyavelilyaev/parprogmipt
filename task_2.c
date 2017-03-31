#include <mpi.h>
#include <stdio.h>


int main(int argc, char *argv[]) {
    
    
    MPI_Init(&argc, &argv);
    
    int n;
    MPI_Comm_rank(MPI_COMM_WORLD, &n);
    printf("I'm %d\n", n);
    
    MPI_Finalize();
    
    return 0;
}
