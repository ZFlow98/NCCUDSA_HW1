#!/usr/bin/env python3

import csv
import statistics
import sys
from collections import defaultdict

import matplotlib.pyplot as plt


EXPECTED_IMPLEMENTATIONS = ("CircularQueue", "NaiveShiftQueue")
EXPECTED_SIZES = (2**9, 2**12, 2**15)


def read_results(path):
    times = defaultdict(list)
    checksums = defaultdict(set)

    with open(path, newline="", encoding="utf-8") as stream:
        reader = csv.DictReader(stream)
        expected = {"implementation", "n", "run", "milliseconds", "checksum"}
        if set(reader.fieldnames or []) != expected:
            raise ValueError(f"CSV columns must be {sorted(expected)}")

        for row in reader:
            implementation = row["implementation"]
            n = int(row["n"])
            run = int(row["run"])
            milliseconds = float(row["milliseconds"])
            checksum = int(row["checksum"])

            if implementation not in EXPECTED_IMPLEMENTATIONS:
                raise ValueError(f"unexpected implementation: {implementation}")
            if n not in EXPECTED_SIZES:
                raise ValueError(f"unexpected n: {n}")
            if run not in (1, 2, 3):
                raise ValueError(f"unexpected run number: {run}")
            if milliseconds <= 0:
                raise ValueError(
                    f"{implementation}, n={n}, run={run} measured "
                    f"{milliseconds} ms. The log-scale plot requires a positive "
                    "time. Please rerun the benchmark or contact the TA if the "
                    "problem persists."
                )

            times[(implementation, n)].append(milliseconds)
            checksums[n].add(checksum)

    for implementation in EXPECTED_IMPLEMENTATIONS:
        for n in EXPECTED_SIZES:
            values = times[(implementation, n)]
            if len(values) != 3:
                raise ValueError(
                    f"expected 3 runs for {implementation}, n={n}; got {len(values)}"
                )

    for n, values in checksums.items():
        if len(values) != 1:
            raise ValueError(f"checksum mismatch between implementations for n={n}")

    return times


def main():
    if len(sys.argv) != 3:
        raise SystemExit("usage: plot_results.py results.csv performance.png")

    input_path, output_path = sys.argv[1], sys.argv[2]
    times = read_results(input_path)

    fig, ax = plt.subplots(figsize=(9, 5.5))
    styles = {
        "CircularQueue": {"marker": "o", "label": "CircularQueue"},
        "NaiveShiftQueue": {"marker": "s", "label": "NaiveShiftQueue"},
    }

    print("implementation,n,median_milliseconds")
    for implementation in EXPECTED_IMPLEMENTATIONS:
        medians = []
        for n in EXPECTED_SIZES:
            median = statistics.median(times[(implementation, n)])
            medians.append(median)
            print(f"{implementation},{n},{median:.6f}")

        ax.plot(EXPECTED_SIZES, medians, linewidth=2, **styles[implementation])
        for n, median in zip(EXPECTED_SIZES, medians):
            ax.annotate(
                f"{median:.3f}",
                (n, median),
                textcoords="offset points",
                xytext=(0, 8),
                ha="center",
            )

    ax.set_xscale("log", base=2)
    ax.set_yscale("log")
    ax.set_xticks(EXPECTED_SIZES)
    ax.set_xticklabels([r"$2^9$", r"$2^{12}$", r"$2^{15}$"])
    ax.set_xlabel("queue length n (log scale, base 2)")
    ax.set_ylabel("time for n rounds (milliseconds, log scale)")
    ax.set_title("CircularQueue vs. NaiveShiftQueue")
    ax.grid(True, which="both", linestyle=":", alpha=0.6)
    ax.legend()
    fig.tight_layout()
    fig.savefig(output_path, dpi=180)


if __name__ == "__main__":
    main()
