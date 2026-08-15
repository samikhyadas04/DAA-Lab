/*
 * Lab 03 - Question 5: Multiply Special-Pattern Square Matrices, O(n^2)
 * DAA - Dr. Ajaya Kumar Dash
 *
 * STRUCTURE
 * ---------
 * n = 2^k. A matrix M of this type is defined recursively:
 *     M = [ M1  M2 ]        where M1, M2 are (n/2 x n/2) matrices of the
 *         [ M2  M1 ]        SAME recursive pattern, all the way down to
 *                            single integers (1x1) at the leaves.
 *
 * KEY FACT #1 - Compact representation:
 * Such a matrix is fully determined by only n independent numbers, not
 * n^2. In fact, if g[0..n-1] is that list of n numbers ("the generator"),
 * one can show by induction that:
 *
 *          M[i][j] = g[ i XOR j ]                 (i, j in 0..n-1)
 *
 * (Proof sketch: for n=2, M=[[g0,g1],[g1,g0]] matches i XOR j in {0,1}.
 *  Inductively, if M1's entries are g1[i XOR j] and M2's are g2[i XOR j]
 *  for i,j in 0..n/2-1, then for the full n x n matrix, the top bit of i
 *  XOR j selects M1 vs M2 exactly as the block structure requires, and
 *  the generator is simply g = g1 followed by g2.)
 *
 * KEY FACT #2 - Addition is O(n), not O(n^2):
 * Adding two matrices of this pattern (same size) is the SAME as adding
 * their two generators element-by-element: (A+B)[i][j] = a[i^j]+b[i^j].
 * So we only ever add length-n generator arrays, costing O(n).
 *
 * DIVIDE AND CONQUER MULTIPLICATION
 * ----------------------------------
 * For A = [[A1,A2],[A2,A1]], B = [[B1,B2],[B2,B1]]:
 *     A*B = [ A1B1+A2B2   A1B2+A2B1 ]   = [ P  Q ]
 *           [ A2B1+A1B2   A2B2+A1B1 ]     [ Q  P ]
 * The product is ALSO of the same pattern! So we recursively compute:
 *     P = A1*B1 + A2*B2      (using this SAME algorithm recursively)
 *     Q = A1*B2 + A2*B1
 * and the generator of A*B is simply (generator of P) ++ (generator of Q).
 *
 * COMPLEXITY
 * ----------
 * Let T(n) = time to multiply two n x n patterned matrices (represented
 * by length-n generators). Computing P and Q needs 4 recursive
 * multiplications of size n/2, plus O(n) additions/concatenation:
 *
 *     T(n) = 4*T(n/2) + O(n)      T(1) = O(1)
 *
 * By the Master Theorem (a=4, b=2, f(n)=n=O(n^(log2 4 - eps))), this is
 * case 1, giving  T(n) = Theta(n^2).
 *
 * This beats naive full-matrix multiplication, which would take O(n^3)
 * if you materialized and multiplied the full n x n dense matrices, and
 * is optimal since simply WRITING the full output matrix already costs
 * Theta(n^2).
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* multiply two generators a,b of length n (n = power of 2), returning
   the generator (length n) of their product patterned matrix. */
double *patternedMultiply(double *a, double *b, int n) {
    double *result = malloc(sizeof(double) * n);
    if (n == 1) {
        result[0] = a[0] * b[0];
        return result;
    }
    int half = n / 2;
    double *a1 = a, *a2 = a + half;
    double *b1 = b, *b2 = b + half;

    double *a1b1 = patternedMultiply(a1, b1, half);
    double *a2b2 = patternedMultiply(a2, b2, half);
    double *a1b2 = patternedMultiply(a1, b2, half);
    double *a2b1 = patternedMultiply(a2, b1, half);

    /* P = A1B1 + A2B2 (top-left / bottom-right block generator) */
    /* Q = A1B2 + A2B1 (off-diagonal block generator)            */
    for (int i = 0; i < half; i++) {
        result[i]        = a1b1[i] + a2b2[i];   /* P, first half of gen */
        result[half + i] = a1b2[i] + a2b1[i];   /* Q, second half       */
    }

    free(a1b1); free(a2b2); free(a1b2); free(a2b1);
    return result;
}

/* Expand a length-n generator into a full n x n dense matrix using
   M[i][j] = g[i XOR j], for verification purposes / display. */
double *expand(double *g, int n) {
    double *M = malloc(sizeof(double) * n * n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            M[i * n + j] = g[i ^ j];
    return M;
}

/* Naive O(n^3) dense multiply, for correctness verification only. */
double *naiveMultiplyDense(double *A, double *B, int n) {
    double *C = calloc((size_t) n * n, sizeof(double));
    for (int i = 0; i < n; i++)
        for (int k = 0; k < n; k++) {
            double aik = A[i * n + k];
            for (int j = 0; j < n; j++)
                C[i * n + j] += aik * B[k * n + j];
        }
    return C;
}

double maxDiff(double *A, double *B, int n) {
    double m = 0;
    for (int i = 0; i < n * n; i++) {
        double d = A[i] - B[i];
        if (d < 0) d = -d;
        if (d > m) m = d;
    }
    return m;
}

int main(void) {
    srand(3);
    int sizes[] = {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096};
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);

    FILE *fp = fopen("benchmark.csv", "w");
    fprintf(fp, "n,dc_time_sec,naive_dense_time_sec\n");

    int allCorrect = 1;
    for (int s = 0; s < numSizes; s++) {
        int n = sizes[s];
        srand(3 + s);
        double *ga = malloc(sizeof(double) * n);
        double *gb = malloc(sizeof(double) * n);
        for (int i = 0; i < n; i++) { ga[i] = (rand() % 10) - 5; gb[i] = (rand() % 10) - 5; }

        clock_t t0 = clock();
        double *gc = patternedMultiply(ga, gb, n);
        clock_t t1 = clock();
        double dcTime = (double)(t1 - t0) / CLOCKS_PER_SEC;

        /* Verify against naive O(n^3) dense multiplication for sizes
           where materializing full n x n matrices is still feasible. */
        if (n <= 1024) {
            double *A = expand(ga, n);
            double *B = expand(gb, n);
            t0 = clock();
            double *Cnaive = naiveMultiplyDense(A, B, n);
            t1 = clock();
            double naiveTime = (double)(t1 - t0) / CLOCKS_PER_SEC;

            double *Cdc = expand(gc, n);
            double diff = maxDiff(Cnaive, Cdc, n);
            if (diff >= 1e-6) allCorrect = 0;

            printf("n=%5d  D&C(O(n^2))=%.5fs  naive-dense(O(n^3))=%.5fs  diff=%.2e  %s\n",
                   n, dcTime, naiveTime, diff, (diff < 1e-6) ? "CORRECT" : "MISMATCH!");
            fprintf(fp, "%d,%.6f,%.6f\n", n, dcTime, naiveTime);

            free(A); free(B); free(Cnaive); free(Cdc);
        } else {
            printf("n=%5d  D&C(O(n^2))=%.5fs  naive-dense skipped (too large)\n", n, dcTime);
            fprintf(fp, "%d,%.6f,\n", n, dcTime);
        }

        free(ga); free(gb); free(gc);
    }
    fclose(fp);

    printf("\n%s\n", allCorrect ? "All verified sizes matched naive dense O(n^3) exactly."
                                 : "SOME MISMATCHES FOUND - see above.");
    printf("Results written to benchmark.csv\n");
    return 0;
}
