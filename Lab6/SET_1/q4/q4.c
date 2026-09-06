/*
 * DAA Lab-6, Q4
 * Matrix Chain Multiplication using Dynamic Programming
 * Given dimensions of N-1 matrices in arr[], find the minimum number
 * of scalar multiplications needed to multiply the complete chain.
 */

#include <stdio.h>
#include <limits.h>

int matrixChainOrder(int arr[], int n) {
    /* dp[i][j] = minimum scalar multiplications to compute matrix
       product from matrix i to matrix j */
    int dp[n][n];

    for (int i = 1; i < n; i++)
        dp[i][i] = 0;

    /* chain length */
    for (int len = 2; len < n; len++) {
        for (int i = 1; i < n - len + 1; i++) {
            int j = i + len - 1;
            dp[i][j] = INT_MAX;

            for (int k = i; k <= j - 1; k++) {
                int cost = dp[i][k] + dp[k + 1][j] +
                           arr[i - 1] * arr[k] * arr[j];
                if (cost < dp[i][j])
                    dp[i][j] = cost;
            }
        }
    }

    return dp[1][n - 1];
}

int main() {
    int n;

    printf("Enter N (number of matrices + 1): ");
    scanf("%d", &n);

    int arr[n];
    printf("Enter %d dimensions:\n", n);
    for (int i = 0; i < n; i++)
        scanf("%d", &arr[i]);

    int result = matrixChainOrder(arr, n);

    printf("Minimum number of scalar multiplications = %d\n", result);

    return 0;
}