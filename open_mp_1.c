#include <omp.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

    #pragma omp parallel sections
    {
        {
            printf("Hello world 1\n"); //position undefined
            printf("Hello world 2\n"); //position undefined
        }
        #pragma omp section
        {
            printf("Hello world 3\n"); //position undefined
            printf("Hello world 4\n"); //should alwas go after 3, but not immediately as other threads can print
        }
        #pragma omp section
        {
            printf("Hello world 5\n"); //position undefined
        }
    }

    return 0;
}
