# Combining k Sorted Arrays of n Elements: Sequential vs Pairwise Merging

This project analyzes and empirically validates the running time of two
methods for merging **k sorted arrays, each of length n**, into a single
sorted array of length **kn**.

## (a) Method 1 — Sequential merging

**Idea:** merge array 1 and array 2, merge that result with array 3, merge
that result with array 4, ..., merge in array k last.

**Analysis.** The `i`-th merge combines a running result of size `(i-1)n`
with a new array of size `n`, which takes `Θ(i·n)` time (a merge of two
sorted arrays takes time linear in the *total* size of its two inputs).
Summing over all `k-1` merges:

```
T(n,k) = Θ(n) · (2 + 3 + 4 + ... + k)
       = Θ(n) · Θ(k²)
       = Θ(k² n)
```

**Worst-case running time: `Θ(k² n)`.**

This is quadratic in `k` — each element from an early array can be "touched"
again and again by every subsequent merge, up to `k-1` times in the worst
case.

## (b) Method 2 — Pairwise (tournament) merging

**Idea:** merge the k arrays in disjoint pairs to get `k/2` arrays of size
`2n`; merge those in pairs to get `k/4` arrays of size `4n`; and so on,
until one array of size `kn` remains — exactly the "combine" phase of
merge sort, but starting from k sorted runs instead of `k` single elements.

**Analysis.** There are `⌈log₂ k⌉` rounds. In every round, the total number
of elements being merged across all pairs is `kn` (every element is touched
exactly once per round, regardless of how many pairs it is split across),
so each round costs `Θ(kn)`. Summing over all rounds:

```
T(n,k) = Θ(kn) · Θ(log k) = Θ(kn log k)
```

**Worst-case running time: `Θ(kn log k)`.**

This matches the standard result for k-way merging with a heap/tournament
approach, and is exactly the same bound merge sort achieves when you think
of "combining k runs of size n" as the top `log k` levels of a merge-sort
recursion tree built on `kn` elements.

**Comparison.** For any `k > 2`, `Θ(kn log k)` is asymptotically much
better than `Θ(k² n)`, since `log k ≪ k`. Method 2 is the right choice
whenever k is more than a small constant.

## Files

| File | Purpose |
|---|---|
| `kway_merge_compare.c` | Implements Method 1 (`method1_sequential`) and Method 2 (`method2_pairwise`), validates correctness, and benchmarks both across two experiments |
| `plot_kway_results.py` | Reads the two result CSVs and produces the growth-curve plots |
| `results_vary_k.csv` | Benchmark data: n fixed at 500, k varied from 2 to 256 |
| `results_vary_n.csv` | Benchmark data: k fixed at 16, n varied from 500 to 128,000 |
| `kway_growth_vs_k.png` | Plots showing `Θ(k² n)` vs `Θ(kn log k)` growth as k varies |
| `kway_growth_vs_n.png` | Plots showing both methods are linear in n for fixed k |

## How the program works

`kway_merge_compare.c`:

1. `merge_two` — a standard two-way merge of sorted arrays, counting
   comparisons.
2. `method1_sequential` — repeatedly merges the running result with the
   next array, as described in (a).
3. `method2_pairwise` — maintains a list of sorted runs; each round merges
   disjoint adjacent pairs (carrying over an unpaired run if the count is
   odd) until one run remains, as described in (b).
4. `make_sorted_arrays` — generates k random arrays of n integers each and
   sorts each individually (`qsort`) to serve as the k sorted inputs.
5. `check_sorted_and_complete` — validates that a method's output is sorted
   and contains exactly the same multiset of elements as the k inputs
   (checked via a sum comparison plus the sortedness/length check).
6. Two experiments are run and written to CSV:
   - **Experiment A** (`results_vary_k.csv`): fixes `n = 500` and varies
     `k ∈ {2,4,8,...,256}`, to see the `k²` vs `k log k` growth directly.
   - **Experiment B** (`results_vary_n.csv`): fixes `k = 16` and varies
     `n` from 500 to 128,000, to confirm both methods are linear in `n`.
   Each configuration is averaged over 5 random trials.

## How to reproduce

```bash
# 1. Compile
gcc -O2 -std=c11 -o kway_merge_compare kway_merge_compare.c

# 2. Run the benchmark (writes results_vary_k.csv and results_vary_n.csv)
./kway_merge_compare

# 3. Plot the results (requires: pip install matplotlib pandas numpy)
python3 plot_kway_results.py
```

This produces:
- `kway_growth_vs_k.png` — comparisons/time vs k (linear and log-log), plus
  scaled `k²n` and `kn·log₂k` reference curves, and a normalized
  `comparisons/(kn)` plot that isolates the `k` (linear) vs `log k`
  (near-flat) growth trends of the two methods.
- `kway_growth_vs_n.png` — comparisons/time vs n at fixed k, confirming
  both methods scale linearly in n.

## Sample results

**Experiment A (n = 500 fixed, k varying)** — comparisons:

| k | Method 1 (sequential) | Method 2 (pairwise) |
|---:|---:|---:|
| 2 | 997 | 997 |
| 16 | 67,383 | 31,971 |
| 64 | 1,037,457 | 191,871 |
| 128 | 4,119,022 | 447,738 |
| 256 | 16,415,588 | 1,023,490 |

Doubling k roughly **quadruples** Method 1's comparison count (consistent
with `Θ(k²n)`) while Method 2's count grows much more slowly (consistent
with `Θ(kn log k)`) — by `k = 256`, Method 1 does about **16x** more
comparisons than Method 2.

**Experiment B (k = 16 fixed, n varying)** — comparisons:

| n | Method 1 (sequential) | Method 2 (pairwise) |
|---:|---:|---:|
| 500 | 67,390 | 31,964 |
| 8,000 | 1,079,874 | 511,970 |
| 128,000 | 17,279,878 | 8,191,970 |

At fixed k, both methods' comparison counts scale linearly with n (doubling
n roughly doubles the comparisons for both), confirming the `n` factor in
both `Θ(k²n)` and `Θ(kn log k)`. Method 1 consistently does almost exactly
`log₂(k) ≈ 4` times more comparisons than Method 2 here, matching the
`k / log k` ratio predicted by the two bounds at `k = 16`.
