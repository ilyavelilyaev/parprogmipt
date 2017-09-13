#include <omp.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Usage: $ ./a.out N n\nN - row members count\nn - threads count\n");
        return 1;
    }

    int N; //row members count
    int n; //threads count

    sscanf(argv[1], "%d", &N);
    sscanf(argv[2], "%d", &n);

        // Declaring shared sum variable.
    double sum = 0;

        // Parallel block with n threads. Explicitely showint that sum is shared.
    #pragma omp parallel num_threads(n) shared(sum)
    {
            // Declaring private thread sum variable. Each threads stores its sum in it.
        double threadSum = 0;

            // Dividing for cycle into chunks and paralleling in threads.
        #pragma omp for
        for (int i = 1; i <= N; i++) {
                // Increasing thread private sum by new element.
            threadSum += (1.0 / i);
        }

            // Showing that adding to shared sum should be atomic (thread-safe).
            // Then adding our thread calculated sum to shared sum
        #pragma omp atomic
        sum += threadSum;

    }

        // Printing result
    printf("Result: %lg\n", sum);

    return 0;
}
