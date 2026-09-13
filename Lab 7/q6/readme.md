# Maximum Concurrent Scientists — README


## Approach

### Core Idea
Rather than scanning every individual year (which is wasteful when years
span centuries), treat each scientist's lifespan as two **events**:

- a **birth event** (`+1` to the "alive" counter)
- a **death event** (`-1` to the "alive" counter)

Sorting and sweeping through all `2n` events in chronological order lets us
track how many scientists are alive at any point in time, and find the peak.

### Handling the Tie Rule
The problem states: *if A dies the same year B is born, A's death is
considered to happen before B's birth.* This means, in a same-year tie,
the death event must be processed **before** the birth event.

This is achieved by:
1. Sorting the `birth[]` array and `death[]` array **independently**.
2. Merging them two-pointer style (like the merge step of merge sort):
   - If `birth[i] < death[j]` -> process birth (increment counter).
   - Else (`birth[i] >= death[j]`, i.e., birth ties or comes after a pending death) -> process death (decrement counter) first.

This single `>=` comparison automatically enforces the required tie-break
without any special-casing.

### Steps
1. Read `n` scientists, each with a name, birth year, and death year.
2. Extract `birth[]` and `death[]` arrays (name is not needed for the
   computation itself, only for display/context).
3. Sort both arrays independently — O(n log n).
4. Sweep through both arrays with two pointers, maintaining a running
   `current` (alive count) and `maxAlive` / `maxYear` — O(n).
5. Report `maxAlive` and `maxYear`.


---

## Time Complexity

| Step                          | Complexity     |
|--------------------------------|----------------|
| Sort `birth[]`                | O(n log n)     |
| Sort `death[]`                | O(n log n)     |
| Two-pointer merge/sweep        | O(n)           |
| **Total**                      | **O(n log n)** |

### Space Complexity
**O(n)** — for the auxiliary `birth[]` and `death[]` arrays (plus O(n) for
the original scientist records).

### Comparison with Naive Approach

| Approach                          | Time Complexity                          |
|-------------------------------------|-------------------------------------------|
| Year-by-year scan over full range   | O(n x R), where R = span of years covered |
| **Sweep-line (this algorithm)**     | **O(n log n)**, independent of year range |

The sweep-line approach is far more efficient than scanning year-by-year,
especially when the historical range is large (e.g., centuries) but the
number of scientists `n` is comparatively small — its cost depends only on
`n`, never on how many years the data spans.

---

## Important Notes

1. **Tie-break correctness**: the `>=` in the merge step is what encodes
   "death before birth" in a tie year — this is the crux of the algorithm's
   correctness, not an incidental detail.
2. **Reports the first year** the maximum is reached; to report *all* years
   at the peak (or the full interval of years sustaining the max), track
   every point where `current == maxAlive` during the sweep instead of
   stopping at the first occurrence.
3. **Assumes death year >= birth year** for every entry — real-world/editorial
   input should be validated for this.
4. **No living scientists** in the book simplifies things: every scientist
   contributes a well-defined, finite (birth, death) pair, so both arrays
   are always fully populated and the sweep always terminates.
5. **Name field is auxiliary**: it isn't used by the algorithm itself, only
   for input readability/display — the mathematical core only needs the two
   year arrays.