/*
 * DAA Lab-6, Q2
 * 0/1 Knapsack Problem using Dynamic Programming (Bottom-Up Tabulation)
 * Given n items with weight[] and profit[], and capacity W,
 * find the maximum profit obtainable.
 */

#include <stdio.h>
#include <stdlib.h>

int max(int a, int b) {
    return (a > b) ? a : b;
}

int knapsack(int n, int weight[], int profit[], int W) {
    int dp[n + 1][W + 1];

    for (int i = 0; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            if (i == 0 || w == 0) {
                dp[i][w] = 0;
            } else if (weight[i - 1] <= w) {
                dp[i][w] = max(profit[i - 1] + dp[i - 1][w - weight[i - 1]],
                                dp[i - 1][w]);
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    return dp[n][W];
}

int main() {
    int n, W;

    printf("Enter number of items: ");
    scanf("%d", &n);

    int weight[n], profit[n];

    printf("Enter weights of items:\n");
    for (int i = 0; i < n; i++)
        scanf("%d", &weight[i]);

    printf("Enter profits of items:\n");
    for (int i = 0; i < n; i++)
        scanf("%d", &profit[i]);

    printf("Enter knapsack capacity: ");
    scanf("%d", &W);

    int result = knapsack(n, weight, profit, W);

    printf("Maximum profit = %d\n", result);

    return 0;
}