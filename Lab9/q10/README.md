# Q10 – Greedy Superstring Conjecture (open problem)

Given strings `S = {s_1..s_n}`, find the shortest string containing all of them. NP-hard. This folder implements the **greedy algorithm**, an **exact solver for small n**, and experiments to build intuition – it does not (and cannot) "solve" the conjecture.

## Input representation
`n`, then `n` strings. Overlap `ov(a,b)` = longest suffix of `a` that is a prefix of `b`. First, strings that are substrings of others (and duplicates) are removed – this does not change the optimum.

## Algorithms
* **Greedy:** repeatedly merge the ordered pair with maximum overlap (`a + b[ov:]`) until one string remains. (Different tie-breaking rules exist; Nikolaev showed all tie-breakings are equivalent for the *conjecture*.)
* **Exact (Held–Karp):** `SCS = Σ|s_i| − (max total overlap along a Hamiltonian path)`; bitmask DP `dp[mask][last]`, usable for `n ≤ 16`.
* **Bad family** (`./q10 --family K`): `{ c(ab)^K, (ba)^K, (ab)^K c }`. Greedy first joins the pair with overlap `2K` and then must concatenate with no overlap (`≈ 4K`), while the optimum chains them with overlaps `2K−1` (`≈ 2K`). The measured ratio: K=3 → 1.40, K=10 → 1.75, K=50 → 1.942, K=200 → 1.985, i.e. → 2 (the classical lower bound).

## Complexity
Let `N = Σ|s_i|`, `ℓ` = longest string.
| | Time | Space |
|---|---|---|
| Substring removal | `O(n² · ℓ²)` (naive `strstr`) | `O(N)` |
| Greedy (as coded) | `n−1` rounds × `n²` overlaps × `O(N²)` worst → **`O(n³N²)`** loose bound, tiny in practice | `O(N)` |
| Greedy (efficient) | near-linear in `N` using suffix trees / Aho–Corasick with bucketed overlaps (Ukkonen, 1990) | `O(N)` |
| Exact Held–Karp | **`O(2ⁿ n² + n²ℓ²)`** | **`O(2ⁿ · n)`** |

## Status of the conjecture (checked 9 Oct 2026)
* **Conjecture** (Tarhio–Ukkonen, 1988): greedy is a 2-approximation. The ratio is known to be **≥ 2** (the family above). Before 2026 the best proven upper bound for greedy was 3.425 (Englert–Matsakis–Veselý, STOC 2022); the conjecture is proven for strings of length ≤ 4.
* **New claim:** Hiroki Shibata, *"Disproving the Greedy Superstring Conjecture"*, arXiv:2609.01365 (Sept 2026): the approximation ratio of greedy is **at least 9/4** on instances of equal even length `k ≥ 10`. A tracker record states the counterexample was found with an AI system and that the paper is a **preprint, not peer reviewed**, provisional status. So treat it as "claimed disproof, awaiting community verification".
* The explicit counterexample construction is **not implemented here**; if you fetch it from the paper you can feed it to `./q10` – the Held–Karp solver can only certify instances with ≤ 16 strings after reduction, so the ratio check on large instances needs a known optimum from the paper.

## Validation (`./q10 --selftest`)
3000 random small instances: the greedy output always contains every input string, is never shorter than the Held–Karp optimum, and the worst random ratio is 1.25 (random data is far from adversarial).

## Build & run
```
gcc -O2 -o q10 solution.c
./q10 < sample_input.txt     # CATGC CTAAGT GCTA TTCA ATGCATC
./q10 --family 50
./q10 --selftest
```
Sample output: `Greedy superstring (16 chars): GCTAAGTTCATGCATC`, optimal 16, ratio 1.0.
