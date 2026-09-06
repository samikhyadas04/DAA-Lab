# DAA Lab-06, Question 2 - Time & Space Complexity Analysis

Input representation: n x n matrix stored as a 2D array (row-major,
O(1) random access to any entry).

## (i) Matrix addition
- **TC:** Theta(n^2) - every one of the n^2 entries is touched
  exactly once. Optimal, since all n^2 outputs must be written.
- **SC:** O(n^2) for the output matrix C (O(1) extra scalar space if
  addition is done into one of the inputs in place).

## (ii) Matrix multiplication
- **TC:** O(n^3) for the naive triple loop - n^2 output entries, each
  costing O(n). Strassen's divide-and-conquer algorithm lowers this
  to O(n^log2 7) = O(n^2.807); Galactic-algorithm bounds go as low as
  ~O(n^2.371), impractical for the small n used here.
- **SC:** O(n^2) for the output matrix C. Strassen additionally needs
  O(n^2) space for the intermediate sub-matrix products (still O(n^2)
  overall, not asymptotically worse).

## (iii) Is the matrix a zero matrix?
- **TC:** O(n^2) worst case (only one non-zero entry can force a full
  scan); Omega(n^2) is also a lower bound for the same reason.
- **SC:** O(1) extra space.

## (iv) Is the matrix symmetric?
- **TC:** O(n^2) - n(n-1)/2 comparisons of M[i][j] against M[j][i].
- **SC:** O(1) extra space.

## (v) Determinant (Gaussian elimination)
- **TC:** O(n^3) - n elimination steps (columns), each doing O(n^2)
  work (an O(n) row update repeated O(n) times):
  sum_{col=0}^{n-1} O((n-col)^2) = O(n^3).
  (Cofactor/Laplace expansion is O(n!) and is avoided.)
- **SC:** O(n^2) for the working copy of the matrix that is reduced
  to upper-triangular form (the input matrix itself is preserved).

## (vi) Transpose in situ
- **TC:** O(n^2) - exactly n(n-1)/2 swaps.
- **SC:** O(1) extra space - purely in place (only valid in place
  because the matrix is square).

## (vii) Eigenvalues & eigenvectors (Jacobi, symmetric matrices)
- **TC:** O(n^3) per full sweep (O(n^2) rotations, each touching
  O(n) entries of A and O(n) entries of V). No polynomial worst-case
  bound exists in general for n > 4 (eigenvalues are roots of the
  degree-n characteristic polynomial, and by the Abel-Ruffini theorem
  there is no general closed-form/radical solution), so the algorithm
  is inherently iterative. In practice Jacobi converges quadratically,
  and empirically a small, near-constant number of sweeps suffices,
  giving an effective cost of about O(n^3 log n) to convergence.
- **SC:** O(n^2) for the working copy of A and the accumulated
  eigenvector matrix V.

## Summary table

| Part | Operation                | Time complexity | Space complexity |
|------|---------------------------|:----------------:|:-----------------:|
| i    | Matrix addition             | O(n^2)                        | O(n^2) (output) |
| ii   | Matrix multiplication       | O(n^3) [O(n^2.807) Strassen]  | O(n^2) |
| iii  | Zero-matrix check           | O(n^2)                        | O(1) |
| iv   | Symmetric check             | O(n^2)                        | O(1) |
| v    | Determinant                 | O(n^3)                        | O(n^2) |
| vi   | Transpose in situ           | O(n^2)                        | O(1) |
| vii  | Eigenvalue/eigenvector      | O(n^3) per sweep (iterative)  | O(n^2) |