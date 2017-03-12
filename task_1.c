#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define THREADS 4
#define COUNT 10000000

int *array;
int result = 0;

pthread_mutex_t mutex;

struct range {
    int begin;
    int end;
};

void sum(void *arg) {
    struct range ran = *((struct range *)arg);
    int res = 0;
    for (int i = ran.begin; i < ran.end; i++) {
        res += array[i];
    }
    
    pthread_mutex_lock(&mutex);
    result += res;
    pthread_mutex_unlock(&mutex);
}

void setupArray() {
    array = (int *)malloc(COUNT * sizeof(int));
    for (int i = 0; i < COUNT; i++) {
        array[i] = 1;
    }
}

int main(int argc, char **argv) {
    pthread_mutex_init(&mutex, NULL);
    setupArray();
    
    pthread_t *threads = (pthread_t *)malloc(THREADS * sizeof(pthread_t));
    struct range *ranges = (struct range *)malloc(THREADS * sizeof(struct range));
    
    int step = COUNT / THREADS;
    
    for (int i = 0; i < THREADS; i++) {
        ranges[i].begin = i * step;
        ranges[i].end = (i + 1 < THREADS) ? (i + 1) * step : COUNT;
        
        pthread_create(threads + i, NULL, &sum, ranges + i);
    }
    
    for (int i = 0; i < THREADS; i++) {
        pthread_join(*(threads + i), NULL);
    }
    
    printf("%d", result);
    
    return 0;
}
