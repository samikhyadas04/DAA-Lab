/* ============================================================
   DAA Lab-06, Question 2
   2D square matrix (n x n) operations and their worst-case
   computational complexities.

   Input representation: a matrix is stored as a 2D array
   double M[n][n] (row-major, contiguous memory) so that any
   entry M[i][j] is accessed in O(1).
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define N 4          /* size of the square matrices used to validate */
#define EPS 1e-9

typedef double Mat[N][N];

void printMatrix(const char *label, Mat M) {
    printf("%s:\n", label);
    for (int i = 0; i < N; i++) {
        printf("  [");
        for (int j = 0; j < N; j++)
            printf("%8.3f%s", M[i][j], (j == N - 1) ? "" : ", ");
        printf("]\n");
    }
}

/* ---------- (i) Matrix addition ------------------------------ O(n^2) */
void matAdd(Mat A, Mat B, Mat C) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            C[i][j] = A[i][j] + B[i][j];
}

/* ---------- (ii) Matrix multiplication ------------------------ O(n^3)
   Naive triple loop. (Strassen's algorithm gives O(n^2.81); see report.) */
void matMul(Mat A, Mat B, Mat C) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double sum = 0.0;
            for (int k = 0; k < N; k++)
                sum += A[i][k] * B[k][j];
            C[i][j] = sum;
        }
    }
}

/* ---------- (iii) Is the matrix a zero matrix? ----------------- O(n^2) */
int isZeroMatrix(Mat M) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (fabs(M[i][j]) > EPS) return 0;
    return 1;
}

/* ---------- (iv) Is the matrix symmetric? ----------------------- O(n^2)
   (Only the upper triangle needs to be compared against the lower
   triangle, which is n(n-1)/2 comparisons -- still Theta(n^2).)     */
int isSymmetric(Mat M) {
    for (int i = 0; i < N; i++)
        for (int j = i + 1; j < N; j++)
            if (fabs(M[i][j] - M[j][i]) > EPS) return 0;
    return 1;
}

/* ---------- (v) Determinant via Gaussian elimination ------------ O(n^3)
   Cofactor expansion is O(n!) and is avoided. We reduce M to an
   upper-triangular form with partial pivoting; the determinant is
   then the product of the diagonal (with a sign flip per row swap). */
double determinant(Mat Min) {
    Mat M;
    memcpy(M, Min, sizeof(Mat));
    double det = 1.0;

    for (int col = 0; col < N; col++) {
        /* partial pivoting: find the row with max abs value in this column */
        int pivotRow = col;
        for (int r = col + 1; r < N; r++)
            if (fabs(M[r][col]) > fabs(M[pivotRow][col])) pivotRow = r;

        if (fabs(M[pivotRow][col]) < EPS) return 0.0; /* singular */

        if (pivotRow != col) {
            for (int c = 0; c < N; c++) {
                double t = M[col][c]; M[col][c] = M[pivotRow][c]; M[pivotRow][c] = t;
            }
            det = -det; /* row swap flips sign */
        }

        for (int r = col + 1; r < N; r++) {
            double factor = M[r][col] / M[col][col];
            for (int c = col; c < N; c++)
                M[r][c] -= factor * M[col][c];
        }
        det *= M[col][col];
    }
    return det;
}

/* ---------- (vi) Transpose in situ (in place) -------------------- O(n^2)
   Only valid/meaningful in place for a SQUARE matrix: swap M[i][j]
   with M[j][i] for i<j, touching each off-diagonal pair once.       */
void transposeInSitu(Mat M) {
    for (int i = 0; i < N; i++)
        for (int j = i + 1; j < N; j++) {
            double t = M[i][j]; M[i][j] = M[j][i]; M[j][i] = t;
        }
}

/* ---------- (vii) Eigenvalues & eigenvectors --------------------
   Input representation choice: we restrict this routine to SYMMETRIC
   matrices, for which all eigenvalues are real and an orthogonal
   basis of eigenvectors always exists. We use the classical CYCLIC
   JACOBI EIGENVALUE ALGORITHM: repeatedly zero out the largest
   off-diagonal entry with a Givens (plane) rotation.
   Cost: O(n^2) to pick the largest off-diagonal entry (or O(1) if we
   sweep row-by-row), O(n) to apply one rotation to all rows/cols, and
   O(n^2) rotations are applied until convergence -> O(n^3) per full
   sweep, and a small constant number of sweeps suffices in practice
   (no closed-form/polynomial bound exists in general for n > 4,
   since eigenvalues are roots of the characteristic polynomial).    */
void jacobiEigen(Mat Ain, double eigenvalues[N], Mat eigenvectors, int maxSweeps) {
    Mat A; memcpy(A, Ain, sizeof(Mat));
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            eigenvectors[i][j] = (i == j) ? 1.0 : 0.0;

    for (int sweep = 0; sweep < maxSweeps; sweep++) {
        double off = 0.0;
        for (int i = 0; i < N; i++)
            for (int j = i + 1; j < N; j++)
                off += A[i][j] * A[i][j];
        if (off < 1e-12) break;

        for (int p = 0; p < N; p++) {
            for (int q = p + 1; q < N; q++) {
                if (fabs(A[p][q]) < 1e-14) continue;
                double theta = (A[q][q] - A[p][p]) / (2.0 * A[p][q]);
                double t = (theta >= 0 ? 1.0 : -1.0) /
                           (fabs(theta) + sqrt(theta * theta + 1.0));
                double c = 1.0 / sqrt(t * t + 1.0);
                double s = t * c;

                double app = A[p][p], aqq = A[q][q], apq = A[p][q];
                A[p][p] = c * c * app - 2 * s * c * apq + s * s * aqq;
                A[q][q] = s * s * app + 2 * s * c * apq + c * c * aqq;
                A[p][q] = A[q][p] = 0.0;

                for (int k = 0; k < N; k++) {
                    if (k != p && k != q) {
                        double akp = A[k][p], akq = A[k][q];
                        A[k][p] = A[p][k] = c * akp - s * akq;
                        A[k][q] = A[q][k] = s * akp + c * akq;
                    }
                }
                for (int k = 0; k < N; k++) {
                    double vkp = eigenvectors[k][p], vkq = eigenvectors[k][q];
                    eigenvectors[k][p] = c * vkp - s * vkq;
                    eigenvectors[k][q] = s * vkp + c * vkq;
                }
            }
        }
    }
    for (int i = 0; i < N; i++) eigenvalues[i] = A[i][i];
}

int main(void) {
    Mat A = {{4, 1, 2, 0}, {1, 3, 0, 1}, {2, 0, 5, 1}, {0, 1, 1, 2}}; /* symmetric */
    Mat B = {{1, 0, 2, 1}, {0, 1, 0, 0}, {2, 0, 1, 0}, {1, 0, 0, 1}};
    Mat C;

    printMatrix("Matrix A", A);
    printMatrix("Matrix B", B);

    matAdd(A, B, C);
    printMatrix("(i) A + B", C);

    matMul(A, B, C);
    printMatrix("(ii) A * B", C);

    printf("(iii) Is A the zero matrix? %s\n", isZeroMatrix(A) ? "Yes" : "No");

    printf("(iv) Is A symmetric? %s\n", isSymmetric(A) ? "Yes" : "No");
    printf("     Is B symmetric? %s\n", isSymmetric(B) ? "Yes" : "No");

    printf("(v) det(A) = %.4f\n", determinant(A));

    Mat T; memcpy(T, B, sizeof(Mat));
    transposeInSitu(T);
    printMatrix("(vi) Transpose of B (in situ)", T);

    double eigVals[N];
    Mat eigVecs;
    jacobiEigen(A, eigVals, eigVecs, 100);
    printf("(vii) Eigenvalues of (symmetric) A:\n  [");
    for (int i = 0; i < N; i++) printf("%.4f%s", eigVals[i], (i == N - 1) ? "" : ", ");
    printf("]\n");
    printMatrix("      Eigenvectors of A (columns)", eigVecs);

    return 0;
}