"""
Lab 03 - Question 2: Search the Defective Coin
Plots number of weighings vs n, alongside log2(n) reference, showing
the algorithm achieves log2(n) + c weighings.
"""
import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("benchmark.csv")

plt.figure(figsize=(8, 5.5))
plt.plot(df["n"], df["weighings_defective_present"], marker='o',
         label="Weighings (defective coin present)", color="#1f77b4")
plt.plot(df["n"], df["log2n"], linestyle="--",
         label="log2(n) (reference)", color="gray")
plt.xscale("log", base=2)
plt.xlabel("n (number of coins, log2 scale)")
plt.ylabel("Number of weighings")
plt.title("Defective Coin Search: Weighings vs n\n(matches log2(n) + c)")
plt.legend()
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig("plot.png", dpi=150)
print("Saved plot.png")
