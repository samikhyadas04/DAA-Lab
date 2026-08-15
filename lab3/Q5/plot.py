"""
Lab 03 - Question 5: Special-Pattern Matrix Multiplication
Plots the O(n^2) D&C algorithm vs O(n^3) naive dense multiplication.
"""
import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("benchmark.csv")

plt.figure(figsize=(8, 5.5))
plt.plot(df["n"], df["dc_time_sec"], marker='o', label="Pattern-exploiting D&C: O(n^2)", color="#1f77b4")

naive_df = df.dropna(subset=["naive_dense_time_sec"])
plt.plot(naive_df["n"], naive_df["naive_dense_time_sec"], marker='s',
         label="Naive dense multiply: O(n^3)", color="#d62728")

plt.xscale("log", base=2)
plt.yscale("log")
plt.xlabel("n (matrix dimension, log2 scale)")
plt.ylabel("Time (seconds, log scale)")
plt.title("Special-Pattern Matrix Multiplication:\nO(n^2) D&C vs O(n^3) Naive")
plt.legend()
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig("plot.png", dpi=150)
print("Saved plot.png")
