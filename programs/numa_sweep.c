#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define SIZE (512ULL * 1024 * 1024)
#define PASSES 4
#define STRIDE 64

int main() {
    char *buffer = aligned_alloc(64, SIZE);

    if (buffer == NULL) {
        perror("Memory allocation failed");
        return 1;
    }

    // First-touch initialization
    memset(buffer, 1, SIZE);

    double start = omp_get_wtime();

    unsigned long long sum = 0;

    for (int pass = 0; pass < PASSES; pass++) {
        #pragma omp parallel for reduction(+:sum) schedule(static)
        for (size_t i = 0; i < SIZE; i += STRIDE) {
            sum += (unsigned char)buffer[i];
        }
    }

    double end = omp_get_wtime();

    printf("Memory size: 512 MB\n");
    printf("Passes: %d\n", PASSES);
    printf("Stride: %d bytes\n", STRIDE);
    printf("Checksum: %llu\n", sum);
    printf("Execution time: %.6f seconds\n", end - start);

    free(buffer);
    return 0;
}
