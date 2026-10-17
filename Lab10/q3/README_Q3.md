# Q3. Maximum Weight Component with Size Constraint

**File:** `q3_max_weight_component.c`  |  **Compile:** `gcc -O2 -o q3 q3_max_weight_component.c`

## Problem
Each vertex v has weight w_v. Weighted edges are added sequentially. After each edge, report the maximum
total vertex weight among connected components whose size is at most K.

## Input representation
**Edge stream + DSU with per-root aggregates + max-heap.**

| Structure | Meaning |
|---|---|
| `parent[]`, `sz[root]` | DSU and component size |
| `sum[root]` | total vertex weight of the component |
| max-heap of `(sum, root, size)` | candidate valid components (lazy deletion) |

Edge weights are read but not used. The answer depends only on connectivity and vertex weights.

**Input format**
```
V E K
w1 w2 ... wV
u1 v1 wt1
...
uE vE wtE     (vertices are 1..V)
```

## Why a plain running maximum is wrong
Components only grow, so a component that exceeds K can never become valid again. The best valid component
can also merge into a component larger than K, so **the answer can decrease**. Example: weights
`5 4 10 1`, K = 3. After edges (1,2) and (3,4) the best is 11. After edge (2,3) everything is one component of
size 4 > 3, so no component is valid.

## Algorithm (lazy-deletion heap)
```
push every singleton (w_v, v, 1) if 1 <= K
for each edge (u, v):
    a = find(u), b = find(v)
    if a != b:
        union by size; sum[a] += sum[b]; sz[a] += sz[b]
        if sz[a] <= K: push (sum[a], a, sz[a])
    while heap not empty and top is STALE:
        pop                       // stale = root no longer a root, or size changed
    print top.sum, or "none" if the heap is empty
```
**Validity test:** an entry is current iff `parent[root] == root` and `sz[root] == entry.size`.
Sizes strictly increase on each merge, so size identifies the version of a component.

## Complexity
| Step | Cost |
|---|---|
| DSU work per edge | O(α(V)) amortised |
| Heap pushes: V singletons + at most E merges | at most V + E entries |
| Each entry is pushed once and popped at most once | O(log(V+E)) each |
| **Total time** | **O((V + E)·log(V + E))** |
| **Space** | **O(V + E)** (DSU arrays + heap) |

## Sample
Input
```
4 3 3
5 4 10 1
1 2 1
3 4 1
2 3 1
```
Output: `10`, `11`, `none` (the last because the merged component has size 4 > K = 3).
