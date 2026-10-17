# Q5. Lexicographically Smallest Connected Component Permutation

**File:** `q5_lex_smallest_perm.c`  |  **Compile:** `gcc -O2 -o q5 q5_lex_smallest_perm.c`

## Problem
Given a permutation A of 1..n and a list of allowed swaps (u, v), find the lexicographically smallest
permutation reachable using any number of allowed swaps.

## Input representation
**Edge list of swaps over positions 1..n, processed with a DSU.** Swap (u, v) means the values at
**positions** u and v may be exchanged.

| Structure | Meaning |
|---|---|
| `A[i]` | the permutation |
| `parent[]`, `sz[]` | DSU over positions |
| `P[]` = (root(i), i) | positions tagged with their component |
| `Q[]` = (root(i), A[i]) | values tagged with the component of their position |

**Input format**
```
n
A1 A2 ... An
m
u1 v1
...
um vm        (1-indexed positions)
```

## Key idea
Swapping along paths of edges can realise **any** permutation of the elements inside a connected
component. So for each component, the lexicographically smallest arrangement puts its smallest values at its
earliest positions, i.e. sorted values placed into sorted positions. Components are independent.

## Algorithm
```
union(u, v) for every allowed swap
P[i] = (find(i), i);   Q[i] = (find(i), A[i])
sort P by (root, position);  sort Q by (root, value)
for k in 0..n-1:  result[ P[k].position ] = Q[k].value
```
After sorting by root, each component occupies the same contiguous block in `P` and `Q`. Within a block,
`P` lists positions ascending and `Q` lists values ascending, so the k-th entries pair the k-th smallest
position with the k-th smallest value.

## Complexity
| Step | Cost |
|---|---|
| Unions | O(m·α(n)) |
| Computing roots | O(n·α(n)) |
| Two sorts of n pairs | O(n log n) |
| **Total time** | **O(n log n + m·α(n))** |
| **Space** | **O(n)** |

## Sample
Input
```
5
5 4 3 2 1
2
1 2
3 4
```
Components: {1,2}, {3,4}, {5}. Output: `4 5 2 3 1`. Positions 1 and 2 hold {5,4}, which sorts to 4 5.
Positions 3 and 4 hold {3,2}, which sorts to 2 3. Position 5 is isolated and stays 1.
