# Q8 – Minimum Number of Meeting Rooms (Interval Partitioning)

Meetings are half-open `[s, e)`: `[0,30]` and `[30,40]` may share a room.

## Input representation
`n`, then `n` lines `s e`. Array of structs `{s, e, id}`; a **min-heap of (end time, room id)** for rooms currently in use.

## Algorithm (main, also produces the assignment)
Sort meetings by start time. For each meeting: if the room that frees up earliest has `end ≤ start`, reuse it (pop, then push the new end); otherwise open a new room. The number of rooms ever opened is the answer.

**Why optimal:** a new room is opened only when *all* rooms in use overlap the current start, i.e. there are `rooms+1` meetings alive at one instant, so no schedule can use fewer – the answer equals the maximum overlap depth.

**Second method (also in code):** sort all starts and all ends separately and sweep with two pointers (each start before its matching end counts +1, each end ≤ start counts −1); the maximum running count is the answer.

## Complexity
| | |
|---|---|
| Sort | `O(n log n)` |
| Heap phase | `n` × `O(log n)` |
| **Time** | **`O(n log n)`** (both methods) |
| **Space** | **`O(n)`** (heap / copy arrays) |

## Validation (`./q8 --selftest`)
20000 random instances: heap method = two-pointer sweep = an `O(n²)` "max overlap at every start" count, and the printed room assignment is conflict-free → **0 failures**.

## Build & run
```
gcc -O2 -o q8 solution.c
./q8 < sample_input.txt      # [0,30) [5,10) [15,20) [30,40) [10,25) [12,18)
```
Output: `Minimum rooms = 4 (sweep check = 4)` followed by the meeting → room table.
