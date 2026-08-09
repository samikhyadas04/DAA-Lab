"""
plot_results.py
----------------
Reads results.csv (produced by mergesort_compare.c) and produces a
2x2 figure:
  (1) running time  vs n   (linear axes)
  (2) comparisons   vs n   (linear axes)
  (3) running time  vs n   (log-log axes)   -> shows n log n slope
  (4) comparisons/n vs n   (semi-log x)     -> should look like log n

This makes the Theta(n log n) growth of BOTH algorithms visually
obvious, while also showing the constant-factor gap between the
2-way and 3-way versions.
"""

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

df = pd.read_csv("results.csv")
n = df["n"].to_numpy()

fig, axes = plt.subplots(2, 2, figsize=(13, 10))

# ---- (1) time vs n, linear ----
ax = axes[0, 0]
ax.plot(n, df["time_2way"], "o-", label="2-way merge sort")
ax.plot(n, df["time_3way"], "s-", label="3-way merge sort")
ax.set_xlabel("n (input size)")
ax.set_ylabel("time (seconds)")
ax.set_title("Running time vs n")
ax.legend()
ax.grid(True, alpha=0.3)

# ---- (2) comparisons vs n, linear, with n*log2(n) and n*log3(n) reference ----
ax = axes[0, 1]
ax.plot(n, df["comp_2way"], "o-", label="2-way merge sort (comparisons)")
ax.plot(n, df["comp_3way"], "s-", label="3-way merge sort (comparisons)")

# scale reference curves to match the last data point so shapes can be compared
ref2 = n * np.log2(n)
ref3 = n * np.log2(n)  # both are Theta(n log n); log base only changes constant
scale2 = df["comp_2way"].iloc[-1] / ref2[-1]
scale3 = df["comp_3way"].iloc[-1] / ref3[-1]
ax.plot(n, ref2 * scale2, "k--", alpha=0.6, label="c1 * n log2(n) (reference)")
ax.plot(n, ref3 * scale3, "gray", linestyle=":", alpha=0.8,
        label="c2 * n log2(n) (reference)")

ax.set_xlabel("n (input size)")
ax.set_ylabel("# comparisons")
ax.set_title("Comparisons vs n (with n log n reference curves)")
ax.legend()
ax.grid(True, alpha=0.3)

# ---- (3) time vs n, log-log ----
ax = axes[1, 0]
ax.loglog(n, df["time_2way"], "o-", label="2-way merge sort")
ax.loglog(n, df["time_3way"], "s-", label="3-way merge sort")
ax.set_xlabel("n (log scale)")
ax.set_ylabel("time in seconds (log scale)")
ax.set_title("Running time vs n (log-log)\n slope ~1 confirms O(n log n), not polynomial")
ax.legend()
ax.grid(True, which="both", alpha=0.3)

# ---- (4) comparisons / n vs n, semi-log x ----
ax = axes[1, 1]
ax.semilogx(n, df["comp_2way"] / n, "o-", label="comparisons / n  (2-way)")
ax.semilogx(n, df["comp_3way"] / n, "s-", label="comparisons / n  (3-way)")
ax.set_xlabel("n (log scale)")
ax.set_ylabel("comparisons / n")
ax.set_title("Comparisons / n vs n\n(a straight line here means growth ~ log n)")
ax.legend()
ax.grid(True, alpha=0.3)

fig.suptitle("2-way vs 3-way Merge Sort: Order-of-Growth Comparison", fontsize=15)
fig.tight_layout(rect=[0, 0, 1, 0.96])
fig.savefig("mergesort_growth_comparison.png", dpi=150)
print("Saved mergesort_growth_comparison.png")
