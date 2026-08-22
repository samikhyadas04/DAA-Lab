# Pair Sum from Two Sets — O(n log n)

Given two sets `S1` and `S2` (each of size `n`) and a target value `x`, this
program determines whether there exist `a ∈ S1` and `b ∈ S2` such that
`a + b = x`.

## Algorithm

1. **Sort** `S1` in ascending order and `S2` in descending order — `O(n log n)`.
2. **Two-pointer scan**: start `i = 0` (smallest of `S1`) and `j = 0`
   (largest of `S2`).
   - If `S1[i] + S2[j] == x` → pair found.
   - If the sum is **less than** `x` → advance `i` (increase the sum).
   - If the sum is **greater than** `x` → advance `j` (decrease the sum).
   - Stop when `i` or `j` runs out of bounds.

Each pointer only ever moves forward, so the scan is `O(n)`. Combined with
the two sorts, the total runtime is:

```
O(n log n) + O(n log n) + O(n) = O(n log n)
```

## Files

| File            | Description                                  |
|-----------------|-----------------------------------------------|
| `pair_sum.c`    | Interactive program: reads `n`, `S1`, `S2`, `x` from the user and reports whether a pair exists. |


You'll be prompted for:
1. `n` — the size of each set (`S1` and `S2` must be the same size).
2. The `n` integers of `S1`.
3. The `n` integers of `S2`.
4. The target value `x`.

Numbers can be entered on one line or across multiple lines — whitespace
and newlines are both treated as separators.

### Example

```
Enter the size n of each set (S1 and S2 must be equal size): 6
Enter 6 elements of S1 (space or newline separated):
1 4 45 6 10 -8
Enter 6 elements of S2 (space or newline separated):
6 5 4 -10 20 1
Enter the target value x: 16

Pair found: 10 + 6 = 16
```

If no pair sums to `x`, the program prints:

```
No pair sums to <x>
```

## Complexity Summary

| Step                     | Time complexity |
|---------------------------|-----------------|
| Sort `S1` ascending        | `O(n log n)`    |
| Sort `S2` descending       | `O(n log n)`    |
| Two-pointer scan           | `O(n)`          |
| **Total**                  | **`O(n log n)`** |
| Space (copies of arrays)   | `O(n)`          |

## Notes

- The program uses `long` for the intermediate sum to avoid `int` overflow
  on large inputs.
- `S1` and `S2` are copied internally before sorting, so the original input
  order is preserved (not that it matters here, since both are discarded
  after the program reports its result).