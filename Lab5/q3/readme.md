# Quick Sort on N Randomly Generated Elements (C)

A C program that generates `N` random integers, stores them in a file,
sorts them with **Quick Sort**, and writes the sorted result to another file.

## Files

| File          | Purpose                                              |
|---------------|-------------------------------------------------------|
| `q3.c`        | Source code (generation, quick sort, file I/O, timing) |
| `input.txt`   | Generated at runtime — the N unsorted random numbers  |
| `output.txt`  | Generated at runtime — the N numbers, sorted ascending|

## Build

```
gcc -O2 -Wall -Wextra -o q3.exe q3.c
```
(On Linux/macOS, drop the `.exe`: `gcc -O2 -Wall -Wextra -o q3 q3.c`)

## Run

```
.\q3.exe <N> [max_value] [seed]
```

| Argument    | Meaning                                        | Default        |
|-------------|-------------------------------------------------|----------------|
| `N`         | Number of elements to generate (required)        | —              |
| `max_value` | Random values are drawn from `[0, max_value)`     | 100000         |
| `seed`      | RNG seed, for reproducible runs                   | current time   |

Examples:

```
.\q3.exe 20
.\q3.exe 1000000 500 42
```

Sample output:

```
Generated 20 random elements (0-99, seed=42) -> input.txt
Sorted 20 elements in 0.000002 seconds -> output.txt
Sorted correctly: YES
```

## How it works

1. **Generate**: `N` random integers are produced with `rand()` and written
   to `input.txt`, one per line.
2. **Choose a pivot**: for each subarray, a pivot is picked using
   **median-of-three** (first, middle, last element) rather than always
   picking the last element. This is what keeps the algorithm fast even
   on already-sorted or reverse-sorted input.
3. **Partition**: the subarray is rearranged (Lomuto scheme) so every
   element ≤ pivot ends up to its left, and every element > pivot ends
   up to its right. The pivot is now in its final sorted position.
4. **Recurse**: the same process is applied to the left and right
   partitions. To bound worst-case recursion depth at `O(log n)`, the
   function always recurses into the *smaller* partition and loops
   (tail-call style) on the *larger* one instead of recursing on both.
5. **Write**: the sorted array is written to `output.txt`, and a
   correctness check + elapsed time are printed to the console.

The sort is done **in place** — no auxiliary array is allocated for sorting.

## Complexity Analysis

### Time Complexity

| Case    | Complexity   | Why                                                          |
|---------|--------------|---------------------------------------------------------------|
| Best    | O(n log n)   | Pivot splits the array into two roughly equal halves each time |
| Average | O(n log n)   | For random input, partitions are balanced "on average"         |
| Worst   | O(n²)        | Pivot repeatedly splits off only 1 element (highly unbalanced) |

The classic worst case (`O(n²)`) happens when every partition is maximally
unbalanced — e.g. always picking the smallest or largest element as
pivot, which is exactly what happens with a naive "always pick the last
element" strategy on already-sorted input.

**This implementation guards against that** in two ways:
- **Median-of-three pivot selection** makes the pathological "already
  sorted / reverse sorted" worst case very unlikely to be triggered by
  ordinary input.
- **Recurse-on-smaller / loop-on-larger** bounds the recursion *depth*
  to `O(log n)` even if a partition does turn out unbalanced, which
  protects against stack overflow (though the `O(n²)` time bound in a
  true adversarial case still applies — no comparison-based pivot
  selection without randomization/median-of-medians fully eliminates it).

Since input here is **randomly generated**, the practical case is
always the average case: `O(n log n)`.

### Space Complexity

- **O(log n) auxiliary space** on average, from the recursion stack
  (bounded to `O(log n)` by the smaller-partition-recurses rule above).
- **O(n) space** for the array itself — that's the input, not overhead
  added by the algorithm.
- Sorting is done in place; no second array is allocated.

### Stability

Quick Sort is **not stable** — the partitioning step swaps elements
based on value comparisons alone, so two equal elements can end up in
a different relative order than they started in (visible in the
benchmark: `40` and `43` both appear twice in `input.txt`, and their
relative order isn't guaranteed to be preserved in `output.txt`).

### Empirical verification

Timings on this machine (random ints in `[0, 10^9)`), confirming
`O(n log n)` growth on random input:

| N         | Time (s)  |
|-----------|-----------|
| 10,000    | 0.0007    |
| 100,000   | 0.0092    |
| 1,000,000 | 0.1061    |
| 5,000,000 | 0.5349    |

Going from `N=1,000,000` to `N=5,000,000` (5×) takes about 5× longer —
close to linear here because the constant factors of quick sort are so
low, but the `log n` factor grows slowly enough at this scale that it's
hard to distinguish visually from pure linear growth; it becomes more
visible at much larger N.

## Quick Sort vs. Heap Sort

If you're comparing this against a Heap Sort implementation:

| Metric              | Quick Sort              | Heap Sort   |
|---------------------|--------------------------|-------------|
| Best case            | O(n log n)               | O(n log n)  |
| Average case          | O(n log n)               | O(n log n)  |
| Worst case            | O(n²) (rare, mitigated)  | O(n log n)  |
| Auxiliary space        | O(log n) (recursion)   | O(1)        |
| Stable                | No                       | No          |
| Typical real-world speed | Faster (better cache locality, smaller constants) | Slower |

## Complexity Summary

| Metric           | Value                    |
|-------------------|--------------------------|
| Time (best)        | O(n log n)               |
| Time (average)     | O(n log n)               |
| Time (worst)       | O(n²)                    |
| Space (auxiliary)  | O(log n) — recursion stack|
| Stable             | No                       |
| In-place           | Yes                      |