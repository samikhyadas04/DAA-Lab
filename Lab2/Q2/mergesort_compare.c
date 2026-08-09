/*
 * mergesort_compare.c
 * ---------------------------------------------------------
 * Compares standard (2-way) merge sort with a modified
 * 3-way merge sort.
 *
 * For each input size n, both algorithms are run on the
 * same randomly generated array (a fresh copy for each
 * algorithm so neither benefits from the other's work).
 * We record:
 *   - wall-clock running time (seconds)
 *   - number of key comparisons performed
 *
 * Results are written to results.csv with columns:
 *   n,time_2way,comp_2way,time_3way,comp_3way
 *
 * Build:
 *   gcc -O2 -o mergesort_compare mergesort_compare.c
 * Run:
 *   ./mergesort_compare
 * ---------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* ================= 2-way (standard) merge sort ================= */

static void merge2(int *arr, int l, int m, int r, long long *cmp) {
    int n1 = m - l + 1;
    int n2 = r - m;
    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));
    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        (*cmp)++;
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    free(L);
    free(R);
}

static void mergeSort2(int *arr, int l, int r, long long *cmp) {
    if (l >= r) return;
    int m = l + (r - l) / 2;
    mergeSort2(arr, l, m, cmp);
    mergeSort2(arr, m + 1, r, cmp);
    merge2(arr, l, m, r, cmp);
}

/* ================= 3-way (modified) merge sort ================= */

/* Merge three sorted runs: [l, m1], [m1+1, m2], [m2+1, r] */
static void merge3(int *arr, int l, int m1, int m2, int r, long long *cmp) {
    int n1 = m1 - l + 1;
    int n2 = m2 - m1;
    int n3 = r - m2;

    int *A = (int *)malloc(n1 * sizeof(int));
    int *B = (int *)malloc(n2 * sizeof(int));
    int *C = (int *)malloc(n3 * sizeof(int));

    for (int i = 0; i < n1; i++) A[i] = arr[l + i];
    for (int i = 0; i < n2; i++) B[i] = arr[m1 + 1 + i];
    for (int i = 0; i < n3; i++) C[i] = arr[m2 + 1 + i];

    int i = 0, j = 0, k = 0, idx = l;

    while (i < n1 && j < n2 && k < n3) {
        if (A[i] <= B[j]) {
            (*cmp)++;
            if (A[i] <= C[k]) { (*cmp)++; arr[idx++] = A[i++]; }
            else { (*cmp)++; arr[idx++] = C[k++]; }
        } else {
            (*cmp)++;
            if (B[j] <= C[k]) { (*cmp)++; arr[idx++] = B[j++]; }
            else { (*cmp)++; arr[idx++] = C[k++]; }
        }
    }
    /* At most one of the three runs remains; finish with 2-way merges */
    while (i < n1 && j < n2) {
        (*cmp)++;
        if (A[i] <= B[j]) arr[idx++] = A[i++];
        else arr[idx++] = B[j++];
    }
    while (j < n2 && k < n3) {
        (*cmp)++;
        if (B[j] <= C[k]) arr[idx++] = B[j++];
        else arr[idx++] = C[k++];
    }
    while (i < n1 && k < n3) {
        (*cmp)++;
        if (A[i] <= C[k]) arr[idx++] = A[i++];
        else arr[idx++] = C[k++];
    }
    while (i < n1) arr[idx++] = A[i++];
    while (j < n2) arr[idx++] = B[j++];
    while (k < n3) arr[idx++] = C[k++];

    free(A); free(B); free(C);
}

static void mergeSort3(int *arr, int l, int r, long long *cmp) {
    if (l >= r) return;
    int len = r - l + 1;
    if (len < 3) {
        /* fall back to a 2-way split/merge for tiny partitions */
        int m = l + (r - l) / 2;
        mergeSort3(arr, l, m, cmp);
        mergeSort3(arr, m + 1, r, cmp);
        merge2(arr, l, m, r, cmp);
        return;
    }
    int third = len / 3;
    int m1 = l + third - 1;
    int m2 = l + 2 * third - 1;
    mergeSort3(arr, l, m1, cmp);
    mergeSort3(arr, m1 + 1, m2, cmp);
    mergeSort3(arr, m2 + 1, r, cmp);
    merge3(arr, l, m1, m2, r, cmp);
}

/* ================= helpers ================= */

static int *make_random_array(int n, unsigned seed) {
    int *a = (int *)malloc(n * sizeof(int));
    srand(seed);
    for (int i = 0; i < n; i++) a[i] = rand();
    return a;
}

static int is_sorted(int *a, int n) {
    for (int i = 1; i < n; i++)
        if (a[i - 1] > a[i]) return 0;
    return 1;
}

int main(void) {
    int sizes[] = {1000, 2000, 4000, 8000, 16000, 32000, 64000,
                    128000, 256000, 512000, 1000000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    int trials = 5; /* average over multiple random arrays per size */

    FILE *fp = fopen("results.csv", "w");
    if (!fp) { perror("fopen"); return 1; }
    fprintf(fp, "n,time_2way,comp_2way,time_3way,comp_3way\n");

    for (int s = 0; s < num_sizes; s++) {
        int n = sizes[s];
        double total_time2 = 0, total_time3 = 0;
        long long total_cmp2 = 0, total_cmp3 = 0;

        for (int t = 0; t < trials; t++) {
            unsigned seed = (unsigned)(n * 1000 + t);

            int *base = make_random_array(n, seed);
            int *a2 = (int *)malloc(n * sizeof(int));
            int *a3 = (int *)malloc(n * sizeof(int));
            for (int i = 0; i < n; i++) { a2[i] = base[i]; a3[i] = base[i]; }

            long long cmp2 = 0, cmp3 = 0;

            clock_t start = clock();
            mergeSort2(a2, 0, n - 1, &cmp2);
            clock_t end = clock();
            double time2 = (double)(end - start) / CLOCKS_PER_SEC;

            start = clock();
            mergeSort3(a3, 0, n - 1, &cmp3);
            end = clock();
            double time3 = (double)(end - start) / CLOCKS_PER_SEC;

            if (!is_sorted(a2, n) || !is_sorted(a3, n)) {
                fprintf(stderr, "ERROR: sort failed for n=%d\n", n);
                return 1;
            }

            total_time2 += time2;
            total_time3 += time3;
            total_cmp2 += cmp2;
            total_cmp3 += cmp3;

            free(base); free(a2); free(a3);
        }

        double avg_time2 = total_time2 / trials;
        double avg_time3 = total_time3 / trials;
        long long avg_cmp2 = total_cmp2 / trials;
        long long avg_cmp3 = total_cmp3 / trials;

        printf("n=%8d  2way: t=%.6fs cmp=%lld  |  3way: t=%.6fs cmp=%lld\n",
               n, avg_time2, avg_cmp2, avg_time3, avg_cmp3);

        fprintf(fp, "%d,%.6f,%lld,%.6f,%lld\n",
                n, avg_time2, avg_cmp2, avg_time3, avg_cmp3);
        fflush(fp);
    }

    fclose(fp);
    printf("\nResults written to results.csv\n");
    return 0;
}
