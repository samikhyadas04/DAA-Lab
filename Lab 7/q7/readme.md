# Matrix Chain Multiplication (DP) — README

This program computes the **minimum number of scalar multiplications** needed
to multiply a chain of matrices, and reconstructs the **optimal
parenthesization** (multiplication order), using dynamic programming.

---

## Time Complexity

### Overall: **O(n³)**

The algorithm fills a 2D table `dp[i][j]` where `i, j` range over `1..n`
(`n` = number of matrices). For each `(i, j)` pair, it tries every possible
split point `k` between `i` and `j`.

| Component            | Complexity | Reason                                             |
|-----------------------|-----------|-----------------------------------------------------|
| Number of subproblems | O(n²)     | All pairs `(i, j)` with `1 ≤ i ≤ j ≤ n`              |
| Work per subproblem   | O(n)      | Trying every split point `k` from `i` to `j−1`       |
| **Total**             | **O(n³)** | O(n²) subproblems × O(n) work each                   |

### Space Complexity: **O(n²)**

Two `n × n` tables are used:
- `dp[i][j]` — stores minimum multiplication cost for matrices `i..j`
- `bracket[i][j]` — stores the optimal split point `k` (needed to reconstruct
  the parenthesization; without it you'd only know the *cost*, not the *order*)

### Why DP instead of brute force?

Brute-force evaluation of all possible parenthesizations has time complexity
equal to the **Catalan number** `C(n-1)`, which grows as:

```
Ω(4ⁿ / n^1.5)
```

This is **exponential**. Dynamic programming reduces this to **O(n³)** by
storing and reusing solutions to overlapping subproblems (each `dp[i][j]` is
computed once and reused many times across larger subproblems), instead of
recomputing them repeatedly.

| Approach      | Time Complexity     |
|---------------|----------------------|
| Brute force   | Exponential — O(4ⁿ / n^1.5) |
| Dynamic Programming | **O(n³)** |

---

## Important Notes

1. **Dimension array size**: For `n` matrices, the dimension array `p[]` must
   have `n + 1` elements. Matrix `i` has dimensions `p[i-1] × p[i]`.
   - Example: for 4 matrices, you need 5 values in `p[]`.

2. **Valid input required**: Consecutive matrices must be multiplication-
   compatible, i.e., the column count of matrix `i` must equal the row count
   of matrix `i+1`. This is automatically satisfied if you only ever input
   `p[0..n]` correctly (the program doesn't separately validate matrix
   dimensions beyond this).

3. **`MAX` limit**: The array size is fixed at `MAX = 20` (supports up to 19
   matrices). Increase this constant if you need to handle larger chains —
   but remember the table size grows as O(n²), so very large `n` will use
   significant memory (though still practical compared to brute force).

4. **1-indexing**: The `dp` and `bracket` tables are 1-indexed (`i, j` run
   from `1` to `n`) to align naturally with matrix numbering (`A1, A2, ...,
   An`). Index `0` of these tables is unused.

5. **`dp[i][i] = 0`**: A single matrix requires no multiplication, so the
   diagonal of the `dp` table is initialized to 0. This is the base case of
   the recurrence.

6. **The `bracket` table is essential**: It records, for every subproblem
   `(i, j)`, the index `k` where the optimal split occurred. Without storing
   this, you could compute the *minimum cost* but would have **no way** to
   recover *which* parenthesization achieves it — you'd need to redo the
   optimization to find it.

7. **Order of loop iteration matters**: The DP fills the table by increasing
   **chain length** (`len = 2` to `n`), not row-by-row or column-by-column.
   This guarantees that when computing `dp[i][j]`, all smaller subproblems
   `dp[i][k]` and `dp[k+1][j]` (which have shorter chain lengths) have
   already been computed.

8. **This program only computes the optimal order — it does not perform
   actual matrix multiplication.** To extend it:
   - Store real matrices (2D arrays) alongside their dimensions.
   - Modify `printOptimalParens` to actually multiply matrices instead of
     printing `A_i`, following the same recursive split at `bracket[i][j]`.

9. **Integer overflow risk**: For matrices with large dimensions, intermediate
   costs (`p[i-1] * p[k] * p[j]`) can overflow a 32-bit `int`. Consider using
   `long long` for the `dp` table and cost calculations if you expect large
   dimension values.

10. **Not related to matrix multiplication algorithms** (like Strassen's):
    This problem only decides the **order** in which to multiply an existing
    chain of matrices to minimize scalar multiplication cost — it doesn't
    change the algorithm used to multiply two matrices together.

---

## Complexity Summary

| Metric             | Complexity |
|---------------------|-----------|
| Time                | O(n³)     |
| Space               | O(n²)     |
| Brute-force time (for comparison) | O(4ⁿ / n^1.5) |