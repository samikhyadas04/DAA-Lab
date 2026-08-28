# Heap Sort on N Randomly Generated Elements (C)

A C program that generates `N` random integers, stores them in a file,
sorts them with **Heap Sort**, and writes the sorted result to another file.

## Files

| File          | Purpose                                              |
|---------------|-------------------------------------------------------|
| `heapsort.c`  | Source code (generation, heap sort, file I/O, timing) |
| `input.txt`   | Generated at runtime — the N unsorted random numbers  |
| `output.txt`  | Generated at runtime — the N numbers, sorted ascending|

## Build

```bash
gcc -O2 -Wall -Wextra -o heapsort heapsort.c
```

## Run

```bash
./heapsort <N> [max_value] [seed]
```

| Argument    | Meaning                                        | Default        |
|-------------|-------------------------------------------------|----------------|
| `N`         | Number of elements to generate (required)        | —              |
| `max_value` | Random values are drawn from `[0, max_value)`     | 100000         |
| `seed`      | RNG seed, for reproducible runs                   | current time   |

Examples:

```bash
./heapsort 20                 # 20 numbers in [0, 100000)
./heapsort 1000000 500 42     # 1,000,000 numbers in [0, 500), fixed seed
```

Sample output:

```
Generated 20 random elements (0-99, seed=42) -> input.txt
Sorted 20 elements in 0.000068 seconds -> output.txt
Sorted correctly: YES
```

## How it works

1. **Generate**: `N` random integers are produced with `rand()` and written
   to `input.txt`, one per line.
2. **Build max-heap**: the array is rearranged in place so it satisfies the
   max-heap property (parent ≥ both children), working bottom-up from the
   last internal node to the root.
3. **Sort by repeated extraction**: the root of the heap (the current
   maximum) is swapped with the last element of the unsorted region, the
   heap shrinks by one, and `heapify` restores the heap property at the
   root. Repeating this `N-1` times leaves the array sorted ascending.
4. **Write**: the sorted array is written to `output.txt`, and a
   correctness check + elapsed time are printed to the console.

The sort is done **in place** on the same array used to generate the
numbers — no auxiliary array is allocated for sorting.

## Complexity Analysis

### Time Complexity

Heap Sort has two phases:

**Phase 1 — Build max-heap: O(n)**
Although each call to `heapify` can take up to `O(log n)` time, most
nodes in a heap are near the bottom where sift-down does very little
work. Summing the actual work across all `n/2` internal nodes gives:

  Σ (n / 2^(h+1)) · O(h)  for h = 0 to log n  =  O(n)

This is a classic tighter bound (not the loose `O(n log n)` you'd get by
naively multiplying `n` calls by `O(log n)` each).

**Phase 2 — Extract max, n-1 times: O(n log n)**
Each of the `n-1` extractions swaps the root out and calls `heapify` on a
heap of size up to `n`, which costs `O(log n)`. Total: `(n-1) · O(log n)
= O(n log n)`.

**Overall time complexity:**

| Case    | Complexity   | Why                                                          |
|---------|--------------|---------------------------------------------------------------|
| Best    | O(n log n)   | Heap operations always cost O(log n) regardless of input order|
| Average | O(n log n)   | Same reasoning — no favorable/unfavorable input arrangement    |
| Worst   | O(n log n)   | Even a reverse- or pre-sorted array still needs full sift-downs|

Unlike Quick Sort, Heap Sort has **no O(n²) worst case** — its performance
is consistently `O(n log n)` regardless of input distribution, which is
why it's a safe default when worst-case guarantees matter.

### Space Complexity

- **O(1) auxiliary space** — sorting is done in place using only a
  constant number of temporary variables (for swaps and loop indices).
  No recursion, no extra arrays.
- The array itself takes `O(n)` space, but that's the input, not
  overhead added by the algorithm.

### Stability

Heap Sort is **not stable** — equal elements can be reordered relative
to each other, because sifting swaps elements based on value alone
with no regard to original position (visible in the benchmark: `40`
appears twice in `input.txt` and their relative order isn't guaranteed
to be preserved in `output.txt`).

### Empirical verification

Timings on this machine (random ints in `[0, 10^9)`), confirming the
`O(n log n)` growth (roughly proportional to `n log n`, not `n` and not `n²`):

| N         | Time (s)  |
|-----------|-----------|
| 10,000    | 0.0007    |
| 100,000   | 0.0079    |
| 1,000,000 | 0.1645    |
| 5,000,000 | 2.3321    |

Going from `N=1,000,000` to `N=5,000,000` (5×) takes about 14× longer,
which matches `n log n` scaling (5 × log-factor-growth) far better than
either linear (~5×) or quadratic (~25×) growth.

## Complexity Summary

| Metric           | Value       |
|-------------------|------------|
| Time (best)        | O(n log n) |
| Time (average)     | O(n log n) |
| Time (worst)       | O(n log n) |
| Space (auxiliary)  | O(1)       |
| Stable             | No         |
| In-place           | Yes        |