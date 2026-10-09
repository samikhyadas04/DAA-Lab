# Q7 – Minimise Deviation in Array (Two-Way Greedy with Max-Heap)

Operations: odd → `×2`, even → `÷2`. Deviation = `max − min`.

## Input representation
`n`, then `n` positive integers (`long long`). A **max-heap** of the current values and a scalar `mn` for the current minimum.

## Algorithm
1. **Normalise upward:** every odd element is doubled. Now every element is at its largest reachable value, and the only useful move left is "halve an even number" (an odd element can never be reduced again without leaving its candidate set `{x, 2x}`; an even element's candidates are `x, x/2, … , odd part`).
2. Set `best = max − min`. Repeat: pop the maximum `top`; `best = min(best, top − mn)`; if `top` is odd stop (it can no longer shrink, so the maximum can never decrease); else `top /= 2`, update `mn`, push back.

**Why correct:** to improve the deviation the maximum must go down, and only the current maximum can lower it, so halving it is the only move that matters; the minimum only moves down, which is accounted for by `mn`. Every element is visited in decreasing order of its candidate set, so every useful configuration is examined.

## Complexity
Each element can be halved at most `log₂ M + 1` times (`M` = largest value).
| | |
|---|---|
| Heap build | `O(n)` (or `O(n log n)` with pushes) |
| ≤ `n(log₂ M + 1)` iterations × `O(log n)` | `O(n log M · log n)` |
| **Time** | **`O(n log n + n log M log n)`** |
| **Space** | **`O(n)`** |

## Validation (`./q7 --selftest`)
20000 random arrays (n ≤ 5, values ≤ 200) vs a brute force that enumerates *every* combination of reachable values: **0 failures**.

## Build & run
```
gcc -O2 -o q7 solution.c
./q7 < sample_input.txt      # 4 1 5 20
```
Output: `Minimum deviation = 3`.
