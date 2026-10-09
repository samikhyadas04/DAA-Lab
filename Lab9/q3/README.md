# Q3 – Minimum Refuelling Stops (Reverse / "Regret" Greedy)

Units: 1 unit of fuel = 1 unit of distance. Start with fuel `F`, target `D`, stations `(d_i, f_i)`.

## Input representation
`F D n`, then `n` lines `d_i f_i`. Stations are stored in an array of structs and **sorted by distance**. A **max-heap** stores refuel amounts of the stations already passed.

## Algorithm
`reach = F`. While `reach < D`:
1. push every station with `d_i ≤ reach` into the max-heap (we *postpone* deciding whether we stopped there);
2. if the heap is empty → unreachable (`-1`);
3. otherwise retroactively stop at the passed station with the **largest** refuel: `reach += pop()`, `stops++`.

**Why it is optimal (exchange argument):** after `k` stops the set of reachable distances is maximised by using the `k` largest refuels among stations that were reachable; any solution that stops at a smaller station while a larger passed station is unused can swap them without hurting reach.

**"Minimum initial fuel" reading (problem title):** `min_stops(F)` is non-increasing in `F`, so `./q3 --minfuel K` binary-searches the smallest `F` that finishes within `K` stops.

## Complexity
| | |
|---|---|
| Sort | `O(n log n)` |
| Main loop | each station is pushed once and popped at most once: `O(n log n)` |
| **Time** | **`O(n log n)`** |
| `--minfuel` | `O(n log n · log D)` (binary search over `F ∈ [0, D]`, stations already sorted) |
| **Space** | **`O(n)`** (heap) |

## Validation (`./q3 --selftest`)
5000 random instances compared with an independent `O(n²)` DP (`dp[k]` = farthest distance reachable with exactly `k` stops): **0 failures**.

## Build & run
```
gcc -O2 -o q3 solution.c
./q3 < sample_input.txt              # F=10 D=100, stations (10,60) (20,30) (30,30) (60,40)
./q3 --minfuel 1 < sample_input.txt
```
Output: `Minimum refuelling stops = 2` and, with `--minfuel 1`, `Minimum initial fuel to finish with <= 1 stops = 40`.
