# Q1. Online Graph-Bipartiteness Testing

**File:** `q1_bipartite_online.c`  |  **Compile:** `gcc -O2 -o q1 q1_bipartite_online.c`

## Problem
Edges of an undirected graph with V vertices arrive one by one. Add an edge if it keeps the graph
bipartite (no odd cycle). Otherwise report it and skip it.

## Input representation
**Edge stream.** Edges arrive online, so no adjacency list or matrix is stored. Only a DSU is kept:

| Array | Meaning |
|---|---|
| `parent[v]` | DSU parent of v |
| `rnk[v]` | rank, for union by rank |
| `par[v]` | parity (colour difference, 0 or 1) between v and `parent[v]` |

**Input format**
```
V E
u1 v1
...
uE vE        (vertices are 1..V)
```

## Algorithm
Idea: a graph is bipartite iff it has a valid 2-colouring. Parity to the root gives a colour for every vertex.

```
find(x):  returns (root, parity of x relative to root), with path compression
          (parities are XOR-ed along the path)

for each edge (u, v):
    (ru, pu) = find(u);  (rv, pv) = find(v)
    if ru == rv:
        if pu == pv:  REJECT      // same colour inside one component -> odd cycle
        else:         ADD         // already consistent
    else:
        attach smaller-rank root under larger-rank root
        par[child] = pu XOR pv XOR 1      // forces u and v to opposite colours
        ADD
```
Self-loops (u = v) have `pu == pv` and the same root, so they are correctly rejected (odd cycle of length 1).

## Complexity
| Step | Cost |
|---|---|
| `find` with path compression and union by rank | O(α(V)) amortised |
| Per edge (2 finds + at most 1 union) | O(α(V)) amortised |
| **Total time** | **O(V + E·α(V))**, practically O(V + E) |
| **Space** | **O(V)** for the three arrays |

`find` is recursive, but union by rank keeps the tree height at O(log V), so recursion depth is safe.

## Sample
Input
```
5 6
1 2
2 3
3 1
3 4
4 5
5 1
```
Output (key lines): edges (3,1) and (5,1) are **REJECTED**. The others are added, and the 2-colouring is
`1:0 2:1 3:0 4:1 5:0`.
