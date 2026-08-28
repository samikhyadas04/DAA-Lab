

## Why avoid sorting?

To get the median you only need **one or two specific values** (the middle
element(s) in sorted order) — you don't need the entire list arranged in
order. A full sort computes the correct position for *every* element, which
is wasted work if the median is all you want.

- Sorting: **O(N log N)**
- Quickselect: **O(N) on average**

## Algorithm: Quickselect

Quickselect reuses the **partition** step from Quicksort:

1. Pick a pivot element (here, chosen **uniformly at random** from the
   current sub-array — this defeats adversarial/already-sorted inputs that
   would break a fixed "first" or "last" element pivot choice).
2. Partition the array around the pivot: everything `< pivot` goes to the
   left, everything `>= pivot` goes to the right. After partitioning, the
   pivot sits at its **final sorted index** `p`.
3. Compare `p` to the target rank `k`:
   - If `p == k` → the pivot **is** the answer, stop.
   - If `k < p` → the answer is in the left partition; recurse **only**
     there.
   - If `k > p` → the answer is in the right partition; recurse **only**
     there.

The key difference from Quicksort: Quickselect throws away the half of the
array it doesn't need and recurses into a single side, instead of recursing
into both halves.

### Finding the median specifically

- **Odd length N:** median = element at sorted index `k = N // 2`.
  → one `quickselect(arr, k)` call.
- **Even length N:** median = average of elements at sorted indices
  `N // 2 - 1` and `N // 2`.
  → `quickselect(arr, N // 2)` partitions the array so that the entire
  left partition is `<=` the result and the entire right partition is
  `>=` it. The second value (`N // 2 - 1`) is then just the **maximum of
  that already-partitioned left partition**, found with one linear scan —
  so we avoid a second full quickselect call.

## Complexity Analysis

### Time Complexity

| Case | Complexity | Why |
|---|---|---|
| **Best case** | O(N) | Pivot always splits the array so the target index is found almost immediately (e.g., pivot ≈ median every time). Work: `N + N/2 + N/4 + ... ≈ 2N`. |
| **Average case** | **O(N)** | With a random pivot, the expected sub-array size shrinks by a constant fraction each round. Solving the recurrence `T(N) = T(N/2) + O(N)` gives `T(N) = O(N)` (geometric series, dominated by the first term). |
| **Worst case** | O(N²) | Only happens if the pivot is unluckily the smallest/largest element on *every single call* (e.g., N, then N−1, then N−2, ...). Random pivot selection makes this **exponentially unlikely** in practice — it is not a realistic concern for random or real-world data. |

**Why the average case is O(N) and not O(N log N):** unlike Quicksort,
Quickselect only ever recurses into **one branch**, not both. The work per
level of recursion is proportional to the size of the current sub-array
(`N`, then roughly `N/2`, then `N/4`, ...), which forms a **geometric
series that sums to O(N)** total — the partitioning work does *not*
get multiplied by `log N` the way it does in Quicksort (which recurses into
both halves at every level).

If a **guaranteed worst-case O(N)** bound is required (e.g., for
adversarial/untrusted input, real-time systems), the pivot can be chosen
using the **Median-of-Medians** algorithm instead of `random.choice`. This
guarantees the pivot always lands within the middle 30–70% of the data,
bounding the recursion so that `T(N) = T(N/5) + T(7N/10) + O(N)` solves to
`T(N) = O(N)` even in the worst case — at the cost of a larger constant
factor, making it slower in practice than randomized Quickselect for
typical inputs.

### Space Complexity

- **O(N)** in this implementation, because `find_median` copies the input
  list (`arr = list(numbers)`) so the caller's original list is left
  untouched, and partitioning is done in place on that copy.
- **O(1) extra space** is achievable if you're allowed to mutate the
  caller's list directly (skip the copy) — partitioning itself only needs
  a few index/swap variables, no auxiliary array.
- Recursion depth (implicit call stack space) is **O(log N)** on average
  and **O(N)** in the worst case; the provided implementation uses an
  **iterative loop** instead of recursion specifically to keep this at
  **O(1)** stack space regardless of case.

### Summary Table

| Metric | Sorting-based median | Quickselect median (this implementation) |
|---|---|---|
| Time (average) | O(N log N) | **O(N)** |
| Time (worst case) | O(N log N) | O(N²) (random pivot makes this astronomically rare) |
| Space | O(N) or O(1) in-place | O(N) copy (O(1) possible) |
| Guaranteed worst-case O(N) variant | — | Median-of-Medians pivot selection |


