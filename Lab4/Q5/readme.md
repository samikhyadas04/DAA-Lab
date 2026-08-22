# Merge Overlapping Intervals — O(n log n) Algorithm

## Problem

Given a list `I` of `n` intervals `(x_i, y_i)`, merge all overlapping
(or touching) intervals and return the resulting disjoint intervals.

```
I = {(1,3), (2,6), (8,10), (7,18)}   -->   {(1,6), (7,18)}
```

## Algorithm

**Input / output representation:** both the input and output are
arrays of a simple `Interval { long x, y; }` struct — the natural
representation for "a list of (start, end) pairs." An array is exactly
what the algorithm needs: it can be sorted in `O(n log n)` with
`qsort`, and merging can produce at most `n` output intervals, so a
caller-supplied array of size `n` is sufficient to hold the result.

**Steps:**

1. Sort the `n` intervals by their start value `x_i`. — `O(n log n)`
2. Sweep through the sorted intervals once, maintaining a "current"
   merged interval `[curX, curY]`:
   - if the next interval's start `≤ curY`, it overlaps or touches
     the current one → extend: `curY = max(curY, next.y)`
   - otherwise the current merged interval is complete → emit it,
     then start a new current interval from the next one.
3. Emit whatever current interval remains at the end.

**Total time complexity:** `O(n log n)` (sorting dominates the `O(n)`
sweep).
**Space complexity:** `O(n)` for the output list (plus the sort's own
overhead).

### Merge rule

Two intervals are merged if they share **any** point, including a
shared endpoint — e.g. `(1,4)` and `(4,7)` merge into `(1,7)`. This
matches the given example: `(1,3)` and `(2,6)` clearly overlap and
merge into `(1,6)`; `(7,18)` absorbs `(8,10)` once sorted, but stays
separate from `(1,6)` because `7 > 6` — they share no point.

### Correctness sketch

Once sorted by start, any interval that can still be merged into the
current running interval must appear immediately next in the sorted
order (its start can't be less than any interval already fully
processed and closed off, since we process in increasing order of
start). So a single left-to-right sweep, extending or closing the
current interval, is enough to find every merge — no need to
re-scan or backtrack.

## Time Complexity

| Step                                   | Cost         |
|-------------------------------------------|--------------|
| Sort `n` intervals by start                | `O(n log n)` |
| Single linear sweep, merging/emitting      | `O(n)`       |
| **Total**                                  | **`O(n log n)`** |

**Space:** `O(n)` for the output array (merged output has at most `n`
intervals).

## Notes

- **Input/output representation matters.** Arrays let both the sort
  step and the sweep step run in their stated time bounds; a
  representation without random access (e.g. a linked list) would
  cost extra time to sort and offer no benefit for the sequential
  sweep.
- **Touching counts as overlapping.** This is a deliberate design
  choice (also the standard convention for this problem) — without
  it, adjacent-but-touching intervals like `(1,4)` and `(4,7)` would
  incorrectly remain separate even though they share the point `4`.
- **Unsorted or duplicate input is handled transparently** — sorting
  in step 1 normalizes the order, so callers don't need to pre-sort
  or de-duplicate their interval list.
- **Correctness was validated, not just argued.** `mergeintervals.c`
  cross-checks the fast algorithm against a brute-force reference
  that repeatedly merges any overlapping pair until no pair overlaps
  (an `O(n³)`-ish fixed-point approach, fine for testing), and also
  independently checks that the fast algorithm's own output is
  well-formed (sorted, pairwise disjoint, and maximal — no further
  merge possible). This runs on:
  - the exact example from the problem statement,
  - 7 additional fixed cases (disjoint intervals, touching endpoints,
    nested intervals, everything collapsing into one, a single
    interval, unsorted/duplicate input, and negative coordinates),
  - 2000 randomized trials with up to 40 intervals each.

  All cases passed.

## Example (worked, from the problem statement)

```
I = {(1,3), (2,6), (8,10), (7,18)}
```

1. Sort by start: `[(1,3), (2,6), (7,18), (8,10)]`
2. Sweep:
   - current = `(1,3)`
   - next `(2,6)`: `2 ≤ 3` → overlaps → extend current to `(1,6)`
   - next `(7,18)`: `7 > 6` → gap → emit `(1,6)`, start new current `(7,18)`
   - next `(8,10)`: `8 ≤ 18` → overlaps → extend current to `(7, max(18,10))` = `(7,18)`
   - end of list → emit `(7,18)`
3. Result: `{(1,6), (7,18)}` ✓ — matches the expected output exactly.

## Files

- `mergeintervals.c` — implementation of the algorithm
  (`merge_intervals`, using `qsort` on an `Interval` array), plus a
  brute-force fixed-point reference implementation
  (`brute_force_merge`), a well-formedness checker for the fast
  algorithm's own output, and a test harness covering the cases
  listed above.



Expected output ends with:

```
=================================
OVERALL RESULT: ALL TESTS PASSED
=================================
```

## Using `merge_intervals` in your own code

```c
Interval in[]  = { {1,3}, {2,6}, {8,10}, {7,18} };
Interval out[4];   /* capacity must be >= n */

int k = merge_intervals(in, 4, out);
for (int i = 0; i < k; i++) {
    printf("(%ld,%ld) ", out[i].x, out[i].y);
}
/* (1,6) (7,18) */
```