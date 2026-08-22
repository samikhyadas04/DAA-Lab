# Peak Party Occupancy — O(n log n) Algorithm

## Problem

A camera at the door records, for each of `n` guests `p_i`, an entry
time `a_i` and an exit time `b_i` (`b_i > a_i`, all `2n` timestamps
distinct). Determine the time at which the most guests were
simultaneously present, and how many that was.

## Algorithm

**Input representation:** each guest is turned into two **events** on
a shared timeline:

```
(a_i, +1)   "someone walked in"
(b_i, -1)   "someone walked out"
```

giving `2n` events total, stored as an array of `{time, delta}`
structs. An array is the right structure here, since the whole
algorithm is "sort by time, then do one linear scan" — exactly what
an array plus `qsort` gives directly, in `O(n log n)`.

**Steps:**

1. Build the `2n` events from `a[]` and `b[]`. — `O(n)`
2. Sort all events by time. — `O(n log n)`
3. Sweep through the sorted events once, keeping a running occupancy
   counter `current`:
   - apply the event's delta (`+1` or `-1`) to `current`
   - **immediately after an entry event**, compare `current` against
     the best count seen so far, and update if it's a new max. — `O(n)`
4. Report the best count found, and the time at which it first
   occurred.

**Total time complexity:** `O(n) + O(n log n) + O(n) = O(n log n)`.
**Space complexity:** `O(n)` for the event array.

### Correctness sketch

Occupancy only ever **increases** on an entry event and only ever
**decreases** on an exit event, so any new maximum must be achieved
immediately after some entry — never after an exit. It is therefore
enough to check the running count only at entry events, rather than
after every one of the `2n` events. Because all `2n` timestamps are
distinct (given), there's no tie-breaking rule needed when two events
happen "at the same time."

## Time Complexity

| Step                                  | Cost         |
|-----------------------------------------|--------------|
| Build `2n` events                        | `O(n)`       |
| Sort events by time                      | `O(n log n)` |
| Single linear sweep                      | `O(n)`       |
| **Total**                                | **`O(n log n)`** |

**Space:** `O(n)` for the event array — no other auxiliary structures
are needed.

## Notes

- **Input representation matters.** Guests naturally arrive as
  parallel arrays `a[]`/`b[]` (entry/exit times) from the camera log.
  Converting these into a single array of `(time, delta)` events is
  what lets us reduce the problem to "sort once, scan once" — the
  same trick that makes the `O(n log n)` bound achievable; keeping
  `a[]` and `b[]` separate and cross-checking overlaps directly would
  push us back toward `O(n²)`.
- **Only checking entry events is what saves the extra factor.**
  Checking the count after *every* event (not just entries) would
  still be correct but wastes half the comparisons, since exits can
  never produce a new maximum.
- **Ties in the maximum are possible** (multiple non-overlapping time
  windows could all have the same peak occupancy). The algorithm
  reports the *first* time this maximum is reached while sweeping
  left to right; the validator below accounts for this by checking
  that the reported time genuinely achieves the maximum, rather than
  requiring it to match the brute-force reference's arbitrarily
  chosen tie.
- **Correctness was validated, not just argued.** `partytime.c`
  cross-checks the fast algorithm against an `O(n²)` brute-force
  reference (for every candidate time `t = a_i`, count guests with
  `a_j ≤ t < b_j`) on 5 fixed examples (overlapping intervals, no
  overlaps, everyone overlapping, a single guest, negative
  timestamps) plus 2000 randomized trials with up to 50 guests each —
  all passed.

## Example

```
p1: 1–4     p2: 2–5     p3: 3–9     p4: 6–8
```

1. Build events: `(1,+1) (2,+1) (3,+1) (4,-1) (5,-1) (6,+1) (8,-1) (9,-1)`
2. Sort by time (already sorted here).
3. Sweep:

   | time | delta | current | new max? |
   |------|-------|---------|----------|
   | 1    | +1    | 1       | best=1 @ t=1 |
   | 2    | +1    | 2       | best=2 @ t=2 |
   | 3    | +1    | 3       | best=3 @ t=3 |
   | 4    | -1    | 2       | — |
   | 5    | -1    | 1       | — |
   | 6    | +1    | 2       | — (2 < 3) |
   | 8    | -1    | 1       | — |
   | 9    | -1    | 0       | — |

4. Peak occupancy is **3 guests**, first reached at **time 3**
   (guests p1, p2, p3 all present in the window `[3, 4)`).

This matches `partytime.c`'s "Example 1" output:
`peak = 3 guests at time 3`.

## Files

- `partytime.c` — implementation of the algorithm (`max_occupancy`,
  using `qsort` on an `Event` array), plus a brute-force `O(n²)`
  reference implementation and a test harness that:
  - runs 5 fixed, hand-checked examples, and
  - runs 2000 randomized trials (up to 50 guests, distinct
    timestamps) comparing the fast algorithm's peak count against
    brute force, and independently verifying that the reported peak
    time really does achieve that peak occupancy.



Expected output ends with:

```
=================================
OVERALL RESULT: ALL TESTS PASSED
=================================
```

## Using `max_occupancy` in your own code

```c
long a[] = {1, 2, 3, 6};   /* entry times */
long b[] = {4, 5, 9, 8};   /* exit times  */
long peak_time;

int peak = max_occupancy(a, b, 4, &peak_time);
printf("Peak occupancy: %d guests at time %ld\n", peak, peak_time);
/* Peak occupancy: 3 guests at time 3 */
```