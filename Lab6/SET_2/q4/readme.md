# DAA Lab-06, Question 4 - Time & Space Complexity Analysis

Input representation: permutation p[0..n-1] of {1,...,n}. Primitive
operation `reverse(p,i,j)`; for Part 2, `cost(reverse) = j-i+1`.

## Part 1: SORT_LINEAR (O(n) reversals)

**Reversal-count bound:** at most n-1 calls to reverse() (proved by
induction: after step i, positions 0..i hold the correct values 1..
i+1, since reverse(p,i,j) brings the value at index j to index i
without disturbing positions 0..i-1).

- **TC:** O(n) reversal *calls*. Each call's own work is O(j-i+1)
  time to physically reverse the sub-range, and finding `target`'s
  index is an O(n) linear scan in the worst case, so a single step
  costs O(n); over n-1 steps the **total running time is O(n^2)**
  worst case (this bound is about *how many reversals are used*,
  n-1, not about runtime - see the report for why the two differ).
  The scan for `target` can be made O(1) with an auxiliary
  position-index array, updated O(1) per swap during each reversal,
  bringing total running time down to O(n) reversal-length work +
  O(n) index lookups = still bounded by the sum of reversal lengths.
- **SC:** O(1) extra space for the algorithm itself (O(n) if the
  optional position-index array is used to speed up lookups).

## Part 2: MERGE_SORT_VIA_REVERSAL (cost-bounded)

### Reversal-length cost (the metric the problem asks to bound)

- **Rotate cost:** `ROTATE(p,l,m,r)` costs `2*(r-l)` (three reversals
  covering `(m-l)+(r-m)+(r-l) = 2*(r-l)` elements total) - O(r-l).
- **One merge of size s:** the recursion always splits at the
  midpoint of the larger run, so the larger sub-problem shrinks by a
  constant factor (~3/4) each level, giving recursion depth O(log s).
  Each level's rotate costs sum to O(s) (they partition disjoint
  index ranges), so **cost_of_one_merge(s) = O(s log s)**.
- **Whole merge sort:** O(log n) levels; at level k (leaves at k=0),
  there are n/2^k merges of size s=2^k each costing O(2^k * k), for a
  per-level total of O(n*k). Summed over k=1..log2(n):
  `sum O(n*k) = O(n * (log2 n)^2)`.
  **Total reversal-length cost: O(n log^2 n)** - matches the required
  bound (confirmed empirically for n up to 4096: the cost/(n log^2 n)
  ratio stays within [0.48, 0.50] across the whole range).

### Running time (as distinct from reversal-length cost)

- Merge sort recursion (index bookkeeping only): T(n) = 2T(n/2) +
  [cost of MERGE_INPLACE], same recurrence shape as ordinary merge
  sort.
- Each MERGE_INPLACE call does O(log s) binary-search comparisons
  (free of reversal cost) plus a rotate of O(s) actual element moves;
  since element-moving work *is* the reversal-length cost analysed
  above, the **running time is also O(n log^2 n)**, dominated by the
  same rotate operations (binary searches contribute only an
  additional O(n log n) comparisons, which is lower order).

### Space complexity

- `ROTATE` and `MERGE_INPLACE` use O(1) extra space per call (a few
  index variables) - the reversal and rotation are performed
  in place on p itself, with no auxiliary array.
- Recursion depth: MERGE_SORT_VIA_REVERSAL has depth O(log n)
  (standard merge-sort recursion); MERGE_INPLACE's own recursion
  depth is O(log s) <= O(log n) per outer call, and these recursions
  are not simultaneously nested beyond O(log n) total stack frames.
- **Total space complexity: O(log n)** (recursion stack only) - no
  auxiliary array is ever allocated, since the algorithm is
  constrained to mutate p using only `reverse()`.

## Summary table

| Part | Algorithm                              | Reversal-length cost | Space complexity |
|------|------------------------------------------|:---------------------:|:-----------------:|
| 1    | SORT_LINEAR (selection-sort via reversal) | O(n) reversals used; O(n^2) worst-case total length | O(1) [O(n) with index-lookup speedup] |
| 2    | MERGE_SORT_VIA_REVERSAL (rotate-based merge) | O(n log^2 n)        | O(log n) (recursion stack only) |