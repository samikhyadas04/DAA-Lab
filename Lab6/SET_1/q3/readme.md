# DAA Lab-6, Q3: Longest Common Subsequence (DP) — Time & Space Complexity

## Time Complexity
Building the dp table requires filling (m+1) x (n+1) cells, each in O(1) time, giving O(m · n). Reconstructing the subsequence by backtracking through the table takes at most O(m + n). Overall time complexity is **O(m · n)**.

## Space Complexity
A 2-D dp table of size (m+1) x (n+1) is maintained, giving a space complexity of **O(m · n)**. (The length-only version can be optimized to O(min(m, n)), but reconstructing the actual subsequence requires the full table.)