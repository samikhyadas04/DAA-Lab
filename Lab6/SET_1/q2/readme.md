# DAA Lab-6, Q2: 0/1 Knapsack (DP) — Time & Space Complexity

## Time Complexity
The dp table has (n+1) rows and (W+1) columns, and each cell is filled in O(1) time. Hence the time complexity is **O(n · W)**.

## Space Complexity
A 2-D dp table of size (n+1) x (W+1) is used, giving a space complexity of **O(n · W)**. (This can be optimized to O(W) using a 1-D rolling array, but the standard tabulation version above uses O(n · W) space.)