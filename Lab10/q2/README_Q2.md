# Q2. Dynamic Connected Components in a Grid (Number of Islands II)

**File:** `q2_islands_grid.c`  |  **Compile:** `gcc -O2 -o q2 q2_islands_grid.c`

## Problem
An R × C grid starts as all water. Land cells are added one at a time. After each addition, output the
number of 4-directionally connected islands.

## Input representation
**Flattened grid + DSU.** Cell (r, c) maps to the single id `id = r*C + c`.

| Array | Meaning |
|---|---|
| `land[id]` | 1 if the cell is land |
| `parent[id]`, `sz[id]` | DSU over the R·C flattened ids |
| `dr[] = {-1,1,0,0}`, `dc[] = {0,0,-1,1}` | the 4 neighbour offsets (no adjacency list needed) |

**Input format**
```
R C K
r1 c1
...
rK cK        (0-indexed)
```

## Algorithm
```
count = 0
for each (r, c):
    if land[id] already:  print count; continue      // duplicate
    land[id] = 1;  count++                            // new island
    for each of the 4 neighbours that is land:
        if union(id, neighbour) merged two different sets:
            count--                                   // two islands became one
    print count
```
Each successful union reduces the island count by exactly 1. A union of two cells already in the same set
changes nothing.

## Complexity
| Step | Cost |
|---|---|
| Initialise DSU | O(R·C) |
| One land addition: at most 4 unions | O(4·α(R·C)) = O(α(R·C)) amortised |
| **Total time** | **O(R·C + K·α(R·C))** |
| **Space** | **O(R·C)** |

## Sample
Input
```
3 3 6
0 0
0 1
1 2
2 1
1 1
1 1
```
Output: islands = `1, 1, 2, 3, 1, 1`. Adding (1,1) joins all three separate islands, and the repeated
(1,1) changes nothing.
