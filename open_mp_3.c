#include <stdio.h>
#include <omp.h>

int main(int argc, char *argv[]) {

    int sum = 0;
    
    #pragma omp for ordered schedule(dynamic) 
    for(int n=0; n<100; ++n)  {
        
        #pragma omp ordered
        {
            sum += 1;
            printf("%d\n", sum);
        }
    }

    printf("final: %d\n", sum);

    
    return 0;
}
