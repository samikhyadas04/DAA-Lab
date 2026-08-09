"""
plot_kway_results.py
---------------------
Reads results_vary_k.csv and results_vary_n.csv (produced by
kway_merge_compare.c) and produces two figures:

  fig1: growth as a function of k (n fixed)
        - Method 1 (sequential merge)  ~ Theta(k^2 * n)
        - Method 2 (pairwise merge)    ~ Theta(k * n * log2 k)

  fig2: growth as a function of n (k fixed)
        - both methods are linear in n (Theta(n) for fixed k)
"""

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# ---------------- Figure 1: vary k, fixed n ----------------
dfk = pd.read_csv("results_vary_k.csv")
k = dfk["k"].to_numpy()
n_fixed = dfk["n"].iloc[0]

fig1, axes = plt.subplots(2, 2, figsize=(13, 10))

ax = axes[0, 0]
ax.plot(k, dfk["cmp_method1"], "o-", label="Method 1 (sequential)")
ax.plot(k, dfk["cmp_method2"], "s-", label="Method 2 (pairwise)")
ax.set_xlabel("k (number of arrays)")
ax.set_ylabel("# comparisons")
ax.set_title(f"Comparisons vs k  (n={n_fixed} fixed)")
ax.legend()
ax.grid(True, alpha=0.3)

ax = axes[0, 1]
ax.loglog(k, dfk["cmp_method1"], "o-", label="Method 1 (sequential)")
ax.loglog(k, dfk["cmp_method2"], "s-", label="Method 2 (pairwise)")
# reference curves scaled to match at largest k
ref_quad = k.astype(float) ** 2 * n_fixed
ref_kn_logk = k.astype(float) * n_fixed * np.log2(k)
ref_quad *= dfk["cmp_method1"].iloc[-1] / ref_quad[-1]
ref_kn_logk *= dfk["cmp_method2"].iloc[-1] / ref_kn_logk[-1]
ax.loglog(k, ref_quad, "k--", alpha=0.6, label="c1 * k^2 n (reference)")
ax.loglog(k, ref_kn_logk, "gray", linestyle=":", label="c2 * k n log2(k) (reference)")
ax.set_xlabel("k (log scale)")
ax.set_ylabel("# comparisons (log scale)")
ax.set_title("Comparisons vs k (log-log)\nslope ~2 for Method 1, ~1 (+log) for Method 2")
ax.legend()
ax.grid(True, which="both", alpha=0.3)

ax = axes[1, 0]
ax.plot(k, dfk["time_method1"], "o-", label="Method 1 (sequential)")
ax.plot(k, dfk["time_method2"], "s-", label="Method 2 (pairwise)")
ax.set_xlabel("k (number of arrays)")
ax.set_ylabel("time (seconds)")
ax.set_title(f"Running time vs k  (n={n_fixed} fixed)")
ax.legend()
ax.grid(True, alpha=0.3)

ax = axes[1, 1]
ax.plot(k, dfk["cmp_method1"] / (k.astype(float) * n_fixed), "o-",
        label="comparisons / (k n)  Method 1")
ax.plot(k, dfk["cmp_method2"] / (k.astype(float) * n_fixed), "s-",
        label="comparisons / (k n)  Method 2")
ax.set_xlabel("k")
ax.set_ylabel("comparisons / (k n)")
ax.set_title("Normalized comparisons vs k\nMethod 1 grows ~linearly in k (=> k^2 n total)\n"
             "Method 2 grows ~log(k) (=> k n log k total)")
ax.legend()
ax.grid(True, alpha=0.3)

fig1.suptitle("k-way Merge: Method 1 (Sequential) vs Method 2 (Pairwise) — varying k",
              fontsize=14)
fig1.tight_layout(rect=[0, 0, 1, 0.95])
fig1.savefig("kway_growth_vs_k.png", dpi=150)
print("Saved kway_growth_vs_k.png")

# ---------------- Figure 2: vary n, fixed k ----------------
dfn = pd.read_csv("results_vary_n.csv")
n = dfn["n"].to_numpy()
k_fixed = dfn["k"].iloc[0]

fig2, axes2 = plt.subplots(1, 2, figsize=(13, 5))

ax = axes2[0]
ax.plot(n, dfn["time_method1"], "o-", label="Method 1 (sequential)")
ax.plot(n, dfn["time_method2"], "s-", label="Method 2 (pairwise)")
ax.set_xlabel("n (elements per array)")
ax.set_ylabel("time (seconds)")
ax.set_title(f"Running time vs n  (k={k_fixed} fixed)\nboth linear in n")
ax.legend()
ax.grid(True, alpha=0.3)

ax = axes2[1]
ax.loglog(n, dfn["cmp_method1"], "o-", label="Method 1 (sequential)")
ax.loglog(n, dfn["cmp_method2"], "s-", label="Method 2 (pairwise)")
ax.set_xlabel("n (log scale)")
ax.set_ylabel("# comparisons (log scale)")
ax.set_title(f"Comparisons vs n (log-log, k={k_fixed} fixed)\nslope ~1 confirms linear in n")
ax.legend()
ax.grid(True, which="both", alpha=0.3)

fig2.suptitle("k-way Merge: Method 1 vs Method 2 — varying n (k fixed)", fontsize=14)
fig2.tight_layout(rect=[0, 0, 1, 0.93])
fig2.savefig("kway_growth_vs_n.png", dpi=150)
print("Saved kway_growth_vs_n.png")
