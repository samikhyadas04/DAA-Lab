#include <stdio.h>
#include <limits.h>

#define MAX 20

int dp[MAX][MAX];       
int bracket[MAX][MAX];  

// Recursively print optimal parenthesization for matrices i..j
void printOptimalParens(int i, int j) {
    if (i == j) {
        printf("A%d", i);
        return;
    }
    printf("(");
    printOptimalParens(i, bracket[i][j]);
    printOptimalParens(bracket[i][j] + 1, j);
    printf(")");
}


int matrixChainOrder(int p[], int n) {

    int matrices = n - 1;

    // Base case: cost of multiplying a single matrix is 0
    for (int i = 1; i <= matrices; i++)
        dp[i][i] = 0;

    for (int len = 2; len <= matrices; len++) {
        for (int i = 1; i <= matrices - len + 1; i++) {
            int j = i + len - 1;
            dp[i][j] = INT_MAX;

            for (int k = i; k < j; k++) {
                int cost = dp[i][k] + dp[k + 1][j] + p[i - 1] * p[k] * p[j];
                if (cost < dp[i][j]) {
                    dp[i][j] = cost;
                    bracket[i][j] = k;
                }
            }
        }
    }

    return dp[1][matrices];
}

int main() {
    int n;

    printf("Enter number of matrices: ");
    scanf("%d", &n);

    int p[MAX];
    printf("Enter dimension array of size %d\n", n + 1);
    printf("(Matrix i has dimensions p[i-1] x p[i]):\n");
    for (int i = 0; i <= n; i++) {
        printf("p[%d] = ", i);
        scanf("%d", &p[i]);
    }

    int minCost = matrixChainOrder(p, n + 1);

    printf("\nMinimum number of scalar multiplications: %d\n", minCost);

    printf("Optimal Parenthesization: ");
    printOptimalParens(1, n);
    printf("\n");

    printf("\nCost table (dp[i][j]):\n");
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (j < i)
                printf("%6s", "-");
            else
                printf("%6d", dp[i][j]);
        }
        printf("\n");
    }

    return 0;
}