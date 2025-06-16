from turtle import color
from matplotlib.colors import BoundaryNorm, ListedColormap
import matplotlib.pyplot as plt
import numpy as np
import os

from sympy import true

plt.style.use("dark_background")


def read_zad1_data(filename):
    configs = []
    energies = []

    with open(filename, "r", encoding="utf-8") as f:
        for line in f:
            tokens = line.strip().split()
            if not tokens:
                continue
            *spins, energy = tokens
            spins = [int(s) for s in spins]
            energy = int(energy)
            configs.append(spins)
            energies.append(energy)

    return configs, energies


def read_results_with_info(filename):
    values = []
    mean = None
    expected = None

    with open(filename, "r", encoding="utf-8") as f:
        for line in f:
            if line.startswith("Średnia"):
                mean = float(line.split(":")[1].strip())
            elif line.startswith("Oczekiwana"):
                expected = float(line.split(":")[1].strip())
            else:
                try:
                    values.append(int(line.strip()))
                except ValueError:
                    continue

    return values, mean, expected


def plot_histogram(filename, title):
    data, mean, expected = read_results_with_info(filename)
    if not data:
        print(f"Brak danych w {filename}")
        return

    plt.figure(figsize=(8, 5))
    plt.hist(data, bins=20, edgecolor="black", alpha=0.7, label="Wyniki z symulacji")

    if mean is not None:
        plt.axvline(
            mean,
            color="#1f77b4",
            linestyle="--",
            linewidth=2,
            label=f"Średnia: {mean:.2f}",
        )
    if expected is not None:
        plt.axvline(
            expected,
            color="#d62728",
            linestyle="-.",
            linewidth=2,
            label=f"Oczekiwana: {expected:.2f}",
        )

    plt.title(title)
    plt.xlabel("Liczba stabilnych konfiguracji")
    plt.ylabel("Liczba powtórzeń")
    plt.legend()
    plt.tight_layout()
    out_name = os.path.splitext(os.path.basename(filename))[0] + ".pdf"
    plt.savefig(f"plots/{out_name}")
    print(f"Zapisano: plots/{out_name}")


def plot_spin_heatmap(filename, title, save_pdf=None):
    configs, energies = read_zad1_data(filename)
    configs = np.array(configs)
    energies = np.array(energies)

    idx = np.argsort(energies)
    configs_sorted = configs[idx]
    energies_sorted = energies[idx]

    cmap = ListedColormap(["blue", "red"])
    norm = BoundaryNorm(boundaries=[-1.5, 0, 1.5], ncolors=2)

    N = len(energies_sorted)
    # Dopasuj wielkość figury, np. wysokość 0.3 cala na konfigurację, max 20 cali
    height = min(20, max(4, N * 0.3))
    plt.figure(figsize=(4, height))

    im = plt.imshow(
        configs_sorted, cmap=cmap, norm=norm, aspect="equal", interpolation="none"
    )

    cbar = plt.colorbar(im, ticks=[-1, 1])
    cbar.set_label("Spin")

    # Ustaw ticki co ~N/20 lub minimum 1 (max 20 ticków)
    max_ticks = 20
    step = max(1, N // max_ticks)
    yticks_positions = np.arange(0, N, step)
    yticks_labels = [f"{energies_sorted[i]:.2f}" for i in yticks_positions]

    plt.yticks(yticks_positions, yticks_labels, fontsize=8, fontfamily="monospace")
    plt.xticks(np.arange(configs_sorted.shape[1]), fontsize=10)

    plt.xlabel("Pozycja spinu")
    plt.ylabel("Konfiguracja (posortowana wg energii)")
    plt.title(title)
    plt.tight_layout()
    out_name = os.path.splitext(os.path.basename(filename))[0] + ".pdf"
    plt.savefig(f"plots/{out_name}")
    print(f"Zapisano: plots/{out_name}")


# 📁 Upewnij się, że folder na obrazki istnieje
os.makedirs("plots", exist_ok=True)
plot_spin_heatmap("data/zad1_ferro_N5.txt", "Heatmapa spinów (ferro, N=5)")
plot_spin_heatmap("data/zad1_ferro_N6.txt", "Heatmapa spinów (ferro, N=6)")
plot_spin_heatmap("data/zad1_antiferro_N5.txt", "Heatmapa spinów (antiferro, N=5)")
plot_spin_heatmap("data/zad1_antiferro_N6.txt", "Heatmapa spinów (antiferro, N=6)")

# ✅ Przykładowe użycia
plot_histogram("data/zad2_stabilne_N9.txt", "Jednospinowo stabilne konfiguracje (N=9)")
plot_histogram(
    "data/zad2_stabilne_N12.txt", "Jednospinowo stabilne konfiguracje (N=12)"
)
plot_histogram(
    "data/zad2_stabilne_N15.txt", "Jednospinowo stabilne konfiguracje (N=15)"
)

plot_histogram(
    "data/zad3_dwu_stabilne_N9.txt", "Dwuspinowo stabilne konfiguracje (N=9)"
)
plot_histogram(
    "data/zad3_dwu_stabilne_N12.txt", "Dwuspinowo stabilne konfiguracje (N=12)"
)
plot_histogram(
    "data/zad3_dwu_stabilne_N15.txt", "Dwuspinowo stabilne konfiguracje (N=15)"
)
