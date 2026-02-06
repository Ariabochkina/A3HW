#!/usr/bin/env python3
"""
Plot Graph 1 (N_t vs F_t0) and Graph 2 (E(N_t) with uncertainty band E(N_t) +/- sigma_Nt).
Reads data/experiment_results.csv, writes figures to data/ or report/.
"""
import argparse
import os
import sys

try:
    import pandas as pd
    import matplotlib
    matplotlib.rcParams["axes.unicode_minus"] = False
    import matplotlib.pyplot as plt
    import numpy as np
except ImportError:
    print("Требуются pandas и matplotlib. pip install pandas matplotlib", file=sys.stderr)
    raise

def load_and_aggregate(csv_path: str):
    df = pd.read_csv(csv_path)
    agg = df.groupby(["stream_size", "B", "step_pct", "step_idx"], as_index=False).agg(
        prefix_size=("prefix_size", "first"),
        F_t0=("F_t0", "first"),
        E_Nt=("N_t", "mean"),
        sigma_Nt=("N_t", "std"),
    ).fillna(0)
    return agg

def plot_graph1(agg: pd.DataFrame, out_dir: str, stream_sizes: list):
    """Graph 1: N_t vs F_t0. X = step (moment t), Y = unique count. Two lines: F_t0 and N_t (E(N_t))."""
    for stream_size in stream_sizes:
        sub = agg[agg["stream_size"] == stream_size].sort_values("step_pct")
        if sub.empty:
            continue
        fig, ax = plt.subplots(figsize=(8, 5))
        ax.plot(sub["step_pct"], sub["F_t0"], label=r"$F_t^0$ (точное)", color="C0", linewidth=2)
        ax.plot(sub["step_pct"], sub["E_Nt"], label=r"$N_t$ (оценка HLL, среднее)", color="C1", linewidth=2, linestyle="--")
        ax.set_xlabel("Обработанная доля потока (%)")
        ax.set_ylabel("Число уникальных элементов")
        ax.set_title(f"График 1: оценка HLL и точная мощность (размер потока = {stream_size})")
        ax.legend()
        ax.grid(True, alpha=0.3)
        ax.set_xlim(0, 100)
        fig.tight_layout()
        path = os.path.join(out_dir, f"graph1_stream{stream_size}.png")
        fig.savefig(path, dpi=150)
        plt.close(fig)
        print(f"Сохранено: {path}")

def plot_graph2(agg: pd.DataFrame, out_dir: str, stream_sizes: list):
    """Graph 2: E(N_t) and shaded area E(N_t) - sigma_Nt, E(N_t) + sigma_Nt."""
    for stream_size in stream_sizes:
        sub = agg[agg["stream_size"] == stream_size].sort_values("step_pct")
        if sub.empty:
            continue
        fig, ax = plt.subplots(figsize=(8, 5))
        x = sub["step_pct"]
        mean_ = sub["E_Nt"]
        sigma_ = sub["sigma_Nt"]
        ax.plot(x, mean_, label=r"$E(N_t)$", color="C1", linewidth=2)
        ax.fill_between(x, mean_ - sigma_, mean_ + sigma_, alpha=0.3, color="C1")
        ax.plot(x, sub["F_t0"], label=r"$F_t^0$ (точное)", color="C0", linewidth=2, linestyle="--")
        ax.set_xlabel("Обработанная доля потока (%)")
        ax.set_ylabel("Число уникальных элементов")
        ax.set_title(f"График 2: статистики HLL (размер потока = {stream_size})")
        ax.legend()
        ax.grid(True, alpha=0.3)
        ax.set_xlim(0, 100)
        fig.tight_layout()
        path = os.path.join(out_dir, f"graph2_stream{stream_size}.png")
        fig.savefig(path, dpi=150)
        plt.close(fig)
        print(f"Сохранено: {path}")

def main():
    p = argparse.ArgumentParser(description="Plot HLL experiment results")
    p.add_argument("csv", nargs="?", default="data/experiment_results.csv", help="Input CSV")
    p.add_argument("-o", "--out-dir", default="data", help="Output directory for figures")
    p.add_argument("--stream-sizes", type=int, nargs="+", default=[10000, 50000, 100000],
                   help="Stream sizes to plot")
    args = p.parse_args()
    if not os.path.isfile(args.csv):
        print(f"Файл не найден: {args.csv}", file=sys.stderr)
        sys.exit(1)
    os.makedirs(args.out_dir, exist_ok=True)
    agg = load_and_aggregate(args.csv)
    stream_sizes = [s for s in args.stream_sizes if s in agg["stream_size"].values]
    if not stream_sizes:
        stream_sizes = sorted(agg["stream_size"].unique().tolist())
    plot_graph1(agg, args.out_dir, stream_sizes)
    plot_graph2(agg, args.out_dir, stream_sizes)
    print("Готово.")

if __name__ == "__main__":
    main()
