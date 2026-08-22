# Point Covered By Most Intervals — O(n log n) Algorithm

## Problem

Given a set `S` of `n` intervals `[l_i, r_i]` on a line (endpoints are
**inclusive** — a point counts as being in an interval if it equals
either endpoint), find a point `p` that lies in the largest number of
intervals.

```
S = {(10,40), (20,60), (50,90), (15,70)}
```

No point lies in all 4 intervals, but `p = 50` (or, as shown below,
`p = 20`) lies in 3 of them.

## Algorithm

**Input representation:** intervals are given as an array of
`Interval { long l, r; }` structs — the natural representation for "a
list of `[left, right]` pairs." From these we build a single array of
`Event { long x; int delta; }` structs (one `+1` for each `l_i`, one
`-1` for each `r_i`). An array is the right structure because the
whole algorithm reduces to "sort by `(coordinate, tie-break)`, then
scan once," which is exactly what an array plus a custom `qsort`
comparator gives directly, in `O(n log n)`.

**Steps:**

1. Build `2n` events from `l[]` and `r[]`: `(l_i, +1)` and `(r_i, -1)`. — `O(n)`
2. Sort the events by coordinate `x`, with a **tie-break rule**: when
   two events share the same `x`, all **start** events (`+1`) at
   that `x` are processed before any **end** event (`-1`) at that
   `x`. — `O(n log n)`
3. Sweep through the sorted events once, keeping a running coverage
   counter `current`:
   - apply the event's delta to `current`
   - **immediately after a start event**, compare `current` against
     the best seen so far, and update if it's a new max. — `O(n)`
4. Report the best count found, and the coordinate at which it first
   occurred.

**Total time complexity:** `O(n) + O(n log n) + O(n) = O(n log n)`.
**Space complexity:** `O(n)` for the event array.

### Why the tie-break rule matters

This is the key difference from a "half-open interval" version of
this problem (e.g. arrival/departure times where the departure
instant itself isn't counted). Here, endpoints are **inclusive on
both sides**: if `A = [1,5]` and `B = [5,10]`, the point `x = 5` is
inside *both* `A` and `B`. If we processed `A`'s end event before
`B`'s start event at `x = 5`, the sweep would never register a
moment where both are counted simultaneously. Processing all starts
before same-coordinate ends guarantees the running counter equals the
true coverage count at that exact `x`, right after the starts are
applied.

### Why checking only after start events suffices

`count(x)` — the number of intervals containing `x` — is a step
function that only ever **increases** at some `l_i` and only ever
**decreases immediately after** some `r_i` (never *at* `r_i`, since
`r_i` is inclusive). So on any stretch between consecutive
breakpoints, `count(x)` is constant and therefore equal to its value
at the left end of that stretch — which is always some `l_i`. Hence
the overall maximum is always attained at (at least) one interval's
left endpoint, and it's enough to check the running count only at
start events.

## Time Complexity

| Step                                       | Cost         |
|-----------------------------------------------|--------------|
| Build `2n` events                              | `O(n)`       |
| Sort events (with tie-break)                   | `O(n log n)` |
| Single linear sweep                            | `O(n)`       |
| **Total**                                      | **`O(n log n)`** |

**Space:** `O(n)` for the event array — no other auxiliary structures
are needed.

## Notes

- **Input representation matters.** Turning `[l_i, r_i]` pairs into a
  single sortable array of `(coordinate, delta)` events is what
  reduces the problem to "sort once, scan once" — the same trick
  used for interval-overlap counting problems in general.
- **The tie-break rule is the crux of correctness for this specific
  (inclusive-endpoint) version of the problem.** Getting it backwards
  (ends before starts on a tie) would silently undercount coverage
  at any point where one interval's `r` coincides with another's `l`.
- **Ties in the maximum are expected.** Multiple points can achieve
  the same peak coverage (in the worked example, both `p = 20` and
  `p = 50` cover 3 intervals). The algorithm reports the first such
  point found while sweeping left to right; the validator accounts
  for this by checking that the reported point genuinely achieves
  the maximum, rather than requiring it to match a specific
  brute-force answer.
- **Correctness was validated, not just argued.** `maxpoint.c`
  cross-checks the fast algorithm against an `O(n²)` brute-force
  reference (try every `l_i` as a candidate point, count containment
  directly) on:
  - the exact example from the problem statement,
  - 7 additional fixed cases — including intervals that touch
    exactly at one inclusive endpoint (the tricky tie case), a
    degenerate single-point interval (`l == r`), several intervals
    meeting at one shared point, and negative coordinates,
  - 2000 randomized trials with up to 50 intervals each, deliberately
    generated with short lengths (0–20) to make coincident endpoints
    common and stress-test the tie-break rule.

  All cases passed.

## Example (worked, from the problem statement)

```
S = {(10,40), (20,60), (50,90), (15,70)}
```

1. Build events: `(10,+1) (15,+1) (20,+1) (40,-1) (50,+1) (60,-1) (70,-1) (90,-1)`
   (no ties here, so the tie-break rule doesn't come into play for
   this particular example).
2. Sort by coordinate (already sorted above).
3. Sweep:

   | x  | delta | current | new max? |
   |----|-------|---------|----------|
   | 10 | +1    | 1       | best=1 @ p=10 |
   | 15 | +1    | 2       | best=2 @ p=15 |
   | 20 | +1    | 3       | best=3 @ p=20 |
   | 40 | -1    | 2       | — |
   | 50 | +1    | 3       | — (3 not > 3) |
   | 60 | -1    | 2       | — |
   | 70 | -1    | 1       | — |
   | 90 | -1    | 0       | — |

4. Maximum coverage is **3 intervals**, first reached at **p = 20**
   (intervals `(10,40)`, `(20,60)`, `(15,70)` all contain 20). The
   problem statement's `p = 50` also achieves 3 (intervals
   `(20,60)`, `(50,90)`, `(15,70)`) — both are valid answers, since
   the maximum isn't unique.

## Files

- `maxpoint.c` — implementation of the algorithm
  (`max_point_coverage`, using `qsort` on an `Event` array with a
  custom start-before-end tie-break comparator), plus a brute-force
  `O(n²)` reference implementation and a test harness covering the
  cases listed above.


Expected output ends with:

```
=================================
OVERALL RESULT: ALL TESTS PASSED
=================================
```

## Using `max_point_coverage` in your own code

```c
Interval in[] = { {10,40}, {20,60}, {50,90}, {15,70} };
long p;

int max_count = max_point_coverage(in, 4, &p);
printf("Max coverage: %d intervals at p = %ld\n", max_count, p);
/* Max coverage: 3 intervals at p = 20 */
```s