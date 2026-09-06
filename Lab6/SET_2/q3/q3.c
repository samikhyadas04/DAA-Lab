/* ============================================================
   DAA Lab-06, Question 3
   Convolution of two vectors A (length m) and B (length n), n >= m:

        C[k] = sum_{j=0}^{m-1} A[j] * B[k-j]      (0-indexed, B[.]
                                                     treated as 0
                                                     outside its range)

   The naive definition costs Theta(n*m). We instead compute the
   convolution with the Fast Fourier Transform (Cooley-Tukey), which
   IS the O(n log n) divide-and-conquer algorithm requested:

        C = IFFT( FFT(A_padded) . FFT(B_padded) )

   where "." is point-wise multiplication and both vectors are
   zero-padded to a length L = 2^ceil(log2(m+n-1)) (a power of two,
   required by the radix-2 FFT recursion; n >= m is exactly the
   assumption we exploit, since we only ever need L >= m+n-1).

   Input representation: vectors are stored as arrays of `double
   complex` (real inputs are represented with zero imaginary part),
   which is the representation the FFT recursion operates on.
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>

#define PI 3.14159265358979323846

/* ---------- Recursive radix-2 Cooley-Tukey FFT ------- O(L log L)
   Divide: split into even-indexed and odd-indexed halves (size L/2
            each) -- this is the "divide" step.
   Conquer: recursively FFT each half.
   Combine: combine the two half-FFTs in O(L) using the butterfly
            formula. Recurrence: T(L) = 2T(L/2) + O(L) => O(L log L)
   invert = 0 for forward FFT, 1 for inverse FFT.                */
void fft(double complex *a, int L, int invert) {
    if (L == 1) return;

    double complex *even = malloc((L / 2) * sizeof(double complex));
    double complex *odd  = malloc((L / 2) * sizeof(double complex));
    for (int i = 0; i < L / 2; i++) {
        even[i] = a[2 * i];
        odd[i]  = a[2 * i + 1];
    }

    fft(even, L / 2, invert);
    fft(odd, L / 2, invert);

    double angleSign = invert ? 2.0 * PI : -2.0 * PI;
    for (int k = 0; k < L / 2; k++) {
        double complex w = cexp(I * angleSign * k / L);
        double complex t = w * odd[k];
        a[k]         = even[k] + t;
        a[k + L / 2] = even[k] - t;
    }

    free(even);
    free(odd);
}

/* Wraps fft() to perform the 1/L scaling needed for the inverse. */
void ifft(double complex *a, int L) {
    fft(a, L, 1);
    for (int i = 0; i < L; i++) a[i] /= L;
}

int nextPowerOfTwo(int x) {
    int L = 1;
    while (L < x) L <<= 1;
    return L;
}

/* ---------- O(n log n) divide-and-conquer convolution ------------ */
double *convolve(double A[], int m, double B[], int n, int *outLen) {
    int resultLen = m + n - 1;
    int L = nextPowerOfTwo(resultLen);

    double complex *fa = calloc(L, sizeof(double complex));
    double complex *fb = calloc(L, sizeof(double complex));
    for (int i = 0; i < m; i++) fa[i] = A[i] + 0.0 * I;
    for (int i = 0; i < n; i++) fb[i] = B[i] + 0.0 * I;

    fft(fa, L, 0);
    fft(fb, L, 0);
    for (int i = 0; i < L; i++) fa[i] *= fb[i]; /* point-wise multiply */
    ifft(fa, L);

    double *C = malloc(resultLen * sizeof(double));
    for (int k = 0; k < resultLen; k++)
        C[k] = creal(fa[k]);   /* rounding note: see report for integer inputs */

    free(fa);
    free(fb);
    *outLen = resultLen;
    return C;
}

/* ---------- brute-force O(n*m) reference, used only to validate ---- */
double *convolveBruteForce(double A[], int m, double B[], int n, int *outLen) {
    int resultLen = m + n - 1;
    double *C = calloc(resultLen, sizeof(double));
    for (int k = 0; k < resultLen; k++)
        for (int j = 0; j < m; j++)
            if (k - j >= 0 && k - j < n)
                C[k] += A[j] * B[k - j];
    *outLen = resultLen;
    return C;
}

void printVec(const char *label, double v[], int len) {
    printf("%s: [", label);
    for (int i = 0; i < len; i++) printf("%.3f%s", v[i], (i == len - 1) ? "" : ", ");
    printf("]\n");
}

int main(void) {
    /* m <= n, as required by the problem statement */
    double A[] = {1, 2, 3};                  /* length m = 3 */
    double B[] = {1, 0, -1, 2, 1, 3, -2, 1};  /* length n = 8 */
    int m = sizeof(A) / sizeof(A[0]);
    int n = sizeof(B) / sizeof(B[0]);

    printVec("A", A, m);
    printVec("B", B, n);

    int lenFFT, lenBrute;
    double *cFFT   = convolve(A, m, B, n, &lenFFT);
    double *cBrute = convolveBruteForce(A, m, B, n, &lenBrute);

    printVec("C (FFT divide-and-conquer, O(n log n))", cFFT, lenFFT);
    printVec("C (brute force O(n*m), for validation)", cBrute, lenBrute);

    double maxDiff = 0.0;
    for (int i = 0; i < lenFFT; i++) {
        double d = fabs(cFFT[i] - cBrute[i]);
        if (d > maxDiff) maxDiff = d;
    }
    printf("Max |FFT - brute force| difference = %.10f (should be ~0)\n", maxDiff);

    free(cFFT);
    free(cBrute);
    return 0;
}