# Pair-Sum Across Two Sets — O(n log n) Algorithm

## Problem

Given two sets `S1` and `S2` (each of size `n`) and a number `x`,
determine whether there exists a pair of elements `(a, b)` with
`a ∈ S1`, `b ∈ S2`, such that `a + b = x`.

## Algorithm

**Input representation:** `S1` and `S2` are stored as plain arrays of
numbers. This is the representation the algorithm needs, since it
relies on sorting (random access) and binary search (a total order on
the elements) — exactly what an array provides in `O(1)` per access.

**Steps:**

1. Sort `S1` in ascending order. — `O(n log n)`
2. For every element `b` in `S2`, binary-search the sorted `S1` for
   the value `x - b`.
   - If found, the pair `(x - b, b)` sums to `x` — report it.
   - Each binary search is `O(log n)`, and there are `n` elements in
     `S2`, so this step is `O(n log n)`.
3. If no element of `S2` produces a hit, no such pair exists.

**Total time complexity:** `O(n log n) + O(n log n) = O(n log n)`.
**Space complexity:** `O(n)` (in-place sort of a copy of `S1`, no
extra data structures).

### Correctness sketch

`a + b = x` for some `a ∈ S1, b ∈ S2` if and only if for some
`b ∈ S2`, the value `x - b` is present in `S1`. Sorting `S1` lets us
test membership of `x - b` in `O(log n)` via binary search instead of
`O(n)` via linear scan, which is what gives the `O(n log n)` bound
instead of the naive `O(n²)` brute-force approach.

## Time Complexity

| Step                                      | Cost         |
|--------------------------------------------|--------------|
| Sort `S1`                                   | `O(n log n)` |
| `n` binary searches, `O(log n)` each        | `O(n log n)` |
| **Total**                                   | **`O(n log n)`** |

**Space:** `O(n)` — sorting is done in place on a copy of `S1`; no
auxiliary data structures are needed.

*(Note: a hash-set of `S1` would give expected `O(n)` time, but this
exercise specifically calls for the deterministic sort + binary-search
approach.)*

## Notes

- **Input representation matters.** Arrays are the natural choice
  here because the algorithm needs (a) random access for sorting and
  (b) a total order for binary search — both efficient on arrays. A
  linked list, for example, would break the `O(log n)` binary-search
  step.
- **Correctness was validated, not just argued.** `pairsum.c`
  cross-checks the fast algorithm against an `O(n²)` brute-force
  reference on 5 fixed examples plus 2000 randomized trials
  (including negative numbers and `n = 1` edge cases) — all passed.
- **Only one pair is reported** (the first hit found while scanning
  `S2`). The algorithm could easily be extended to collect *all*
  valid pairs, still in `O(n log n + k)` time where `k` is the number
  of pairs found, by continuing the scan instead of stopping at the
  first hit.

## Example

```
S1 = [8, 4, 5, 2, 10]
S2 = [3, 8, 2, 6, 1]
x  = 10
```

1. Sort `S1` → `[2, 4, 5, 8, 10]`
2. Scan `S2`:
   - `b = 3` → need `7` → not in `S1`
   - `b = 8` → need `2` → **found!** → pair is `(2, 8)`, since `2 + 8 = 10`

Output: `pair found -> 2 + 8 = 10` — exactly what `pairsum.c` prints
for this test case (see "Example 1" in the program's output).




Expected output ends with:

```
=================================
OVERALL RESULT: ALL TESTS PASSED
=================================
```

Each test case prints `S1`, `S2`, `x`, the pair found by the fast
`O(n log n)` algorithm (if any), and confirms it matches the
brute-force reference.

## Using `pair_sum_exists` in your own code

```c
long S1[] = {8, 4, 5, 2, 10};
long S2[] = {3, 8, 2, 6, 1};
long a, b;

/* Note: S1 is sorted in place by this call. */
int found = pair_sum_exists(S1, 5, S2, 5, /* x = */ 10, &a, &b);

if (found) {
    printf("%ld + %ld = 10\n", a, b); /* e.g. 2 + 8 = 10 */
} else {
    printf("No pair sums to 10\n");
}
```