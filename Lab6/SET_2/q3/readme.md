# DAA Lab-06, Question 3 - Time & Space Complexity Analysis

Input representation: vectors A (length m), B (length n, n >= m),
represented as complex-number arrays for the FFT (real inputs carry
imaginary part 0).

## Recursive FFT: T(L) recurrence

Let T(L) be the time to FFT a vector of length L (a power of 2).

- **Divide:** split into even-indexed / odd-indexed halves of size
  L/2 each - Theta(L) to build the two halves.
- **Conquer:** two recursive calls, each on size L/2 -> 2*T(L/2).
- **Combine:** the butterfly loop touches every one of the L output
  slots once -> Theta(L).

`T(L) = 2*T(L/2) + Theta(L)`, `T(1) = O(1)`.

By the Master Theorem (a=2, b=2, f(L)=Theta(L), so `L^log_b(a) = L =
f(L)`, case 2): **T(L) = O(L log L)**.

## Time complexity of the full convolution

Since n >= m, the padded length `L = next power of two >= m+n-1` is
`O(n)` (within a factor of 2 of n).

- 2 forward FFTs of length L: `O(n log n)` each
- 1 point-wise multiplication of L pairs: `O(n)`
- 1 inverse FFT of length L: `O(n log n)`

**Total time complexity: O(n log n)**, versus Theta(n*m) for the
naive definition (which is worse whenever m = omega(log n), i.e. for
essentially every practical input size).

## Space complexity

- The padded arrays `fa`, `fb` each have length L = O(n):
  **O(n) space**.
- Each level of the FFT recursion allocates two new arrays of half
  the size of its input (`even`, `odd`); across all O(log L) levels
  of recursion this totals `L + L/2 + L/4 + ... = O(L) = O(n)`
  auxiliary space (a total, not per-level, bound, since arrays from
  completed recursive calls are freed before the caller returns).
- The recursion depth is `O(log n)`, contributing an additional
  `O(log n)` stack space, dominated by the O(n) array space above.

**Total space complexity: O(n)**.

(An in-place, iterative bit-reversal-permutation version of the FFT
reduces the constant factor by avoiding the repeated even/odd array
allocations, but does not change the asymptotic O(n) space or
O(n log n) time bounds.)

## Summary table

| Component                  | Time complexity | Space complexity |
|------------------------------|:----------------:|:-----------------:|
| Forward/inverse FFT (length L=O(n)) | O(n log n) each | O(n) |
| Point-wise multiply           | O(n)             | O(n) (result array) |
| **Overall convolution**       | **O(n log n)**   | **O(n)** |
| (naive definition, for comparison) | Theta(n*m)  | O(n) (output only) |