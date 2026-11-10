#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>

#define N 100000000

int main(int argc, char *argv[]) {
    if (argc != 2 ||
        (strcmp(argv[1], "adjacent") != 0 &&
         strcmp(argv[1], "padded") != 0)) {
        printf("Usage: %s adjacent|padded\n", argv[0]);
        return 1;
    }

    int stride = (strcmp(argv[1], "padded") == 0) ? 8 : 1;

    volatile long *counters = aligned_alloc(64, 128);

    if (counters == NULL) {
        perror("Memory allocation failed");
        return 1;
    }

    memset((void *)counters, 0, 128);

    double start = omp_get_wtime();

    #pragma omp parallel num_threads(2)
    {
        int tid = omp_get_thread_num();

        for (long i = 0; i < N; i++) {
            counters[tid * stride]++;
        }
    }

    double end = omp_get_wtime();

    printf("Layout: %s\n", argv[1]);
    printf("Threads: 2\n");
    printf("Iterations per thread: %d\n", N);
    printf("Counter 0: %ld\n", counters[0]);
    printf("Counter 1: %ld\n", counters[stride]);
    printf("Execution time: %.6f seconds\n", end - start);

    free((void *)counters);
    return 0;
}
