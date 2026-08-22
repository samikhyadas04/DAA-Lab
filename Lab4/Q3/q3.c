#include <stdio.h>
#include <stdlib.h>

/* Compare function for qsort */
int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

/* Binary search for target from index 'left' onward */
int binarySearch(int arr[], int n, int left, int target) {
    int low = left;
    int high = n - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (arr[mid] == target)
            return 1;
        else if (arr[mid] < target)
            low = mid + 1;
        else
            high = mid - 1;
    }

    return 0;
}

/*
   Recursively choose k-1 elements.
   Once k-1 elements are chosen, binary search
   for the final required element.
*/
int kSum(int arr[], int n, int k, int T, int start, long long sum) {

    /* We need to choose k-1 elements */
    if (k == 1) {
        int target = T - sum;

        return binarySearch(arr, n, start, target);
    }

    for (int i = start; i <= n - k; i++) {

        if (kSum(arr, n, k - 1, T, i + 1,
                 sum + arr[i])) {
            return 1;
        }
    }

    return 0;
}

int main() {
    int n, k, T;

    printf("Enter n: ");
    scanf("%d", &n);

    int *S = (int *)malloc(n * sizeof(int));

    printf("Enter %d integers:\n", n);
    for (int i = 0; i < n; i++)
        scanf("%d", &S[i]);

    printf("Enter k: ");
    scanf("%d", &k);

    printf("Enter T: ");
    scanf("%d", &T);

    /* Sort the set */
    qsort(S, n, sizeof(int), compare);

    if (k <= 0 || k > n) {
        printf("Invalid value of k.\n");
    }
    else if (kSum(S, n, k, T, 0, 0)) {
        printf("YES: %d integers add up to %d.\n", k, T);
    }
    else {
        printf("NO: No %d integers add up to %d.\n", k, T);
    }

    free(S);

    return 0;
}