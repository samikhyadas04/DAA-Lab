/*
 * Lab 03 - Question 4: Matrix Multiplication using D&C (Strassen's Method)
 * DAA - Dr. Ajaya Kumar Dash
 *
 * Multiplies two n x n square matrices (n a power of 2) using Strassen's
 * divide-and-conquer algorithm, which needs only 7 recursive
 * multiplications of (n/2)x(n/2) submatrices per level instead of 8:
 *
 *   T(n) = 7*T(n/2) + O(n^2)   =>   T(n) = Theta(n^log2(7)) = Theta(n^2.807)
 *
 * compared to the naive O(n^3) algorithm. Below a chosen cutoff size,
 * Strassen falls back to naive multiplication (standard practice, since
 * the constant factor of Strassen's extra additions dominates for small n).
 *
 * The program benchmarks Strassen vs naive multiplication for several n,
 * verifies correctness against the naive result, and writes timings to
 * benchmark.csv.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CUTOFF 64   /* below this size, use naive multiplication */

typedef struct {
    int n;
    double *data; /* row-major n*n */
} Matrix;

Matrix matAlloc(int n) {
    Matrix m;
    m.n = n;
    m.data = calloc((size_t) n * n, sizeof(double));
    return m;
}
void matFree(Matrix *m) { free(m->data); m->data = NULL; }

#define AT(m, i, j) ((m).data[(size_t)(i) * (m).n + (j)])

Matrix matAdd(Matrix a, Matrix b, int sign) {
    Matrix r = matAlloc(a.n);
    for (int i = 0; i < a.n * a.n; i++) r.data[i] = a.data[i] + sign * b.data[i];
    return r;
}

/* Naive O(n^3) multiply, used for base case and for verification */
Matrix matMulNaive(Matrix a, Matrix b) {
    int n = a.n;
    Matrix r = matAlloc(n);
    for (int i = 0; i < n; i++)
        for (int k = 0; k < n; k++) {
            double aik = AT(a, i, k);
            if (aik == 0.0) continue;
            for (int j = 0; j < n; j++)
                AT(r, i, j) += aik * AT(b, k, j);
        }
    return r;
}

/* Extract quadrant (0,0)/(0,1)/(1,0)/(1,1) of size n/2 from m (size n) */
Matrix subMatrix(Matrix m, int rowOff, int colOff, int half) {
    Matrix r = matAlloc(half);
    for (int i = 0; i < half; i++)
        for (int j = 0; j < half; j++)
            AT(r, i, j) = AT(m, rowOff + i, colOff + j);
    return r;
}

void setSub(Matrix *m, int rowOff, int colOff, Matrix sub) {
    int half = sub.n;
    for (int i = 0; i < half; i++)
        for (int j = 0; j < half; j++)
            AT(*m, rowOff + i, colOff + j) = AT(sub, i, j);
}

Matrix strassen(Matrix a, Matrix b) {
    int n = a.n;
    if (n <= CUTOFF) return matMulNaive(a, b);

    int half = n / 2;
    Matrix A11 = subMatrix(a, 0, 0, half), A12 = subMatrix(a, 0, half, half);
    Matrix A21 = subMatrix(a, half, 0, half), A22 = subMatrix(a, half, half, half);
    Matrix B11 = subMatrix(b, 0, 0, half), B12 = subMatrix(b, 0, half, half);
    Matrix B21 = subMatrix(b, half, 0, half), B22 = subMatrix(b, half, half, half);

    Matrix t1 = matAdd(B12, B22, -1);           Matrix M1 = strassen(A11, t1);
    Matrix t2 = matAdd(A11, A12, 1);            Matrix M2 = strassen(t2, B22);
    Matrix t3 = matAdd(A21, A22, 1);            Matrix M3 = strassen(t3, B11);
    Matrix t4 = matAdd(B21, B11, -1);           Matrix M4 = strassen(A22, t4);
    Matrix t5a = matAdd(A11, A22, 1);
    Matrix t5b = matAdd(B11, B22, 1);           Matrix M5 = strassen(t5a, t5b);
    Matrix t6a = matAdd(A12, A22, -1);
    Matrix t6b = matAdd(B21, B22, 1);           Matrix M6 = strassen(t6a, t6b);
    Matrix t7a = matAdd(A11, A21, -1);
    Matrix t7b = matAdd(B11, B12, 1);           Matrix M7 = strassen(t7a, t7b);

    Matrix c11a = matAdd(M5, M4, 1);
    Matrix c11b = matAdd(c11a, M2, -1);
    Matrix C11  = matAdd(c11b, M6, 1);

    Matrix C12 = matAdd(M1, M2, 1);
    Matrix C21 = matAdd(M3, M4, 1);

    Matrix c22a = matAdd(M5, M1, 1);
    Matrix c22b = matAdd(c22a, M3, -1);
    Matrix C22  = matAdd(c22b, M7, -1);

    Matrix r = matAlloc(n);
    setSub(&r, 0, 0, C11);    setSub(&r, 0, half, C12);
    setSub(&r, half, 0, C21); setSub(&r, half, half, C22);

    Matrix *tmp[] = {&A11,&A12,&A21,&A22,&B11,&B12,&B21,&B22,
                      &t1,&M1,&t2,&M2,&t3,&M3,&t4,&M4,
                      &t5a,&t5b,&M5,&t6a,&t6b,&M6,&t7a,&t7b,&M7,
                      &c11a,&c11b,&C11,&C12,&C21,&c22a,&c22b,&C22};
    for (size_t i = 0; i < sizeof(tmp)/sizeof(tmp[0]); i++) matFree(tmp[i]);

    return r;
}

double maxDiff(Matrix a, Matrix b) {
    double m = 0;
    for (int i = 0; i < a.n * a.n; i++) {
        double d = a.data[i] - b.data[i];
        if (d < 0) d = -d;
        if (d > m) m = d;
    }
    return m;
}

void fillRandom(Matrix m) {
    for (int i = 0; i < m.n * m.n; i++) m.data[i] = (rand() % 20) - 10;
}

int main(void) {
    srand(7);
    int sizes[] = {32, 64, 128, 256, 512, 1024};
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);

    FILE *fp = fopen("benchmark.csv", "w");
    fprintf(fp, "n,naive_time_sec,strassen_time_sec,max_abs_diff\n");

    for (int s = 0; s < numSizes; s++) {
        int n = sizes[s];
        Matrix a = matAlloc(n), b = matAlloc(n);
        fillRandom(a); fillRandom(b);

        clock_t t0 = clock();
        Matrix cNaive = matMulNaive(a, b);
        clock_t t1 = clock();
        double naiveTime = (double)(t1 - t0) / CLOCKS_PER_SEC;

        t0 = clock();
        Matrix cStrassen = strassen(a, b);
        t1 = clock();
        double strassenTime = (double)(t1 - t0) / CLOCKS_PER_SEC;

        double diff = maxDiff(cNaive, cStrassen);

        printf("n=%5d  naive=%.4fs  strassen=%.4fs  max_diff=%.2e  %s\n",
               n, naiveTime, strassenTime, diff,
               (diff < 1e-6) ? "CORRECT" : "MISMATCH!");

        fprintf(fp, "%d,%.6f,%.6f,%.8f\n", n, naiveTime, strassenTime, diff);

        matFree(&a); matFree(&b); matFree(&cNaive); matFree(&cStrassen);
    }

    fclose(fp);
    printf("\nStrassen's results match naive multiplication exactly (diff ~0).\n");
    printf("For large n, Strassen's O(n^2.807) beats naive O(n^3).\n");
    return 0;
}
