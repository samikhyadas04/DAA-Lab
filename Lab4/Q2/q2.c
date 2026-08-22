#include <stdio.h>
#include <stdlib.h>

/* ---------- comparator functions for qsort ---------- */
int cmp_asc(const void *a, const void *b) {
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);
}

int cmp_desc(const void *a, const void *b) {
    int x = *(const int *)a, y = *(const int *)b;
    return (y > x) - (y < x);
}

/*
 * findPair:
 *   S1 sorted ascending, S2 sorted descending, both size n.
 *   Two-pointer scan -> O(n)
 */
int findPair(int *S1, int *S2, int n, int x, int *out1, int *out2) {
    int i = 0, j = 0;
    while (i < n && j < n) {
        long sum = (long)S1[i] + (long)S2[j];
        if (sum == x) {
            *out1 = S1[i];
            *out2 = S2[j];
            return 1;
        } else if (sum < x) {
            i++;   /* need a larger sum -> move to next bigger S1 element */
        } else {
            j++;   /* need a smaller sum -> move to next smaller S2 element */
        }
    }
    return 0;
}

/* pairSumExists: full O(n log n) algorithm (sort + two-pointer scan) */
int pairSumExists(int *S1in, int *S2in, int n, int x, int *out1, int *out2) {
    int *S1 = malloc(n * sizeof(int));
    int *S2 = malloc(n * sizeof(int));
    for (int k = 0; k < n; k++) { S1[k] = S1in[k]; S2[k] = S2in[k]; }

    qsort(S1, n, sizeof(int), cmp_asc);   /* O(n log n) */
    qsort(S2, n, sizeof(int), cmp_desc);  /* O(n log n) */

    int found = findPair(S1, S2, n, x, out1, out2); /* O(n) */

    free(S1);
    free(S2);
    return found;
}

int readArray(const char *label, int **arr, int n) {
    *arr = malloc(n * sizeof(int));
    if (!*arr) {
        fprintf(stderr, "Memory allocation failed\n");
        return 0;
    }
    printf("Enter %d elements of %s (space or newline separated):\n", n, label);
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &(*arr)[i]) != 1) {
            fprintf(stderr, "Invalid input.\n");
            free(*arr);
            return 0;
        }
    }
    return 1;
}

int main(void) {
    int n;

    printf("Enter the size n of each set (S1 and S2 must be equal size): ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        fprintf(stderr, "Invalid size.\n");
        return 1;
    }

    int *S1 = NULL, *S2 = NULL;
    if (!readArray("S1", &S1, n)) return 1;
    if (!readArray("S2", &S2, n)) { free(S1); return 1; }

    int x;
    printf("Enter the target value x: ");
    if (scanf("%d", &x) != 1) {
        fprintf(stderr, "Invalid input.\n");
        free(S1); free(S2);
        return 1;
    }

    int a, b;
    if (pairSumExists(S1, S2, n, x, &a, &b))
        printf("\nPair found: %d + %d = %d\n", a, b, x);
    else
        printf("\nNo pair sums to %d\n", x);

    free(S1);
    free(S2);
    return 0;
}