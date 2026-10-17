# Q4. Reverse Time-Step DSU Application

**File:** `q4_reverse_dsu.c`  |  **Compile:** `gcc -O2 -o q4 q4_reverse_dsu.c`

## Problem
A vertex-weighted undirected graph has its vertices deleted one by one in order P = [p1, ..., pV].
Deleting a vertex also removes its incident edges. After each deletion, output the maximum component
weight sum among the remaining components.

## Input representation
**Adjacency list** (arrays `head[]`, `nxt[]`, `to[]` with 2E entries), because when a vertex is re-inserted we
must scan its neighbours. The other structures:

| Structure | Meaning |
|---|---|
| `w[v]` | vertex weight |
| `active[v]` | whether v is currently present (in the reversed process) |
| `parent[]`, `sz[]`, `sum[root]` | DSU with component weight sums |
| `ans[i]` | answer after i deletions |

**Input format**
```
V E
w1 ... wV
u1 v1
...
uE vE
p1 p2 ... pV        (deletion order; vertices are 1..V)
```

## Key idea: reverse time
DSU supports **union** but not deletion. Deleting p1, ..., pV in order is the same as **inserting**
pV, pV-1, ..., p1 into an empty graph, which uses only unions.

## Algorithm
```
ans[V] = 0                              // all vertices deleted
best = 0
for i = V down to 2:
    v = p_i;  active[v] = 1             // re-insert p_i
    for each neighbour u of v with active[u]:
        union(v, u)                     // sums are added
    best = max(best, sum[find(v)])
    ans[i-1] = best                     // state after deleting p_1..p_{i-1}
print ans[1..V]
```
After deleting p1..p_{i-1}, the remaining vertices are p_i..p_V. That is exactly the graph built after
re-inserting p_V down to p_i.

`best` is a running maximum because, while inserting, components only merge and sums never decrease
(**assumes non-negative weights**). Vertices that have not been inserted yet are not in any component.

## Complexity
| Step | Cost |
|---|---|
| Build adjacency list | O(V + E) |
| Each vertex is activated once | O(V) |
| Each undirected edge is scanned twice (once from each end), one DSU op each | O(E·α(V)) |
| **Total time** | **O(V + E·α(V))** |
| **Space** | **O(V + E)** |

A naive approach (recompute the components after every deletion) would cost O(V·(V + E)).

## Sample
Input
```
4 3
1 2 3 4
1 2
2 3
3 4
2 1 3 4
```
Output: `7, 7, 4, 0`. After deleting 2, the components {1} and {3,4} remain with best sum 7. After deleting
1, the best is still 7. After deleting 3, only {4} remains with sum 4.
