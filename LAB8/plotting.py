import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib.colors import ListedColormap, BoundaryNorm
from scipy.stats import linregress


plt.style.use("dark_background")


def load_avalanche_sizes_from_csv(filename):
    # Wczytanie danych z pliku CSV
    data = pd.read_csv(filename)

    # Zakładając, że kolumna z rozmiarami lawin nazywa się 'avalanche_size'
    # Jeśli ma inną nazwę, należy ją odpowiednio zmienić
    avalanche_sizes = data["avalanche_size"].values

    return avalanche_sizes


def load_grid_evolution(filename):
    frames = []
    grains_added_list = []
    current_frame = []
    current_grain_count = None

    with open(filename, "r") as f:
        for line in f:
            line = line.strip()
            if not line:
                if current_frame:
                    frames.append(current_frame)
                    if current_grain_count is not None:
                        grains_added_list.append(current_grain_count)
                    else:
                        grains_added_list.append(-1)  # fallback
                    current_frame = []
            elif line.startswith("# t ="):
                # Parsujemy liczbę ziaren
                try:
                    current_grain_count = int(line.split("=")[1].strip())
                except ValueError:
                    current_grain_count = -1
            elif line.startswith("#"):
                continue
            else:
                row = list(map(int, line.split(",")))
                current_frame.append(row)

        # Jeśli ostatnia ramka nie została dodana
        if current_frame:
            frames.append(current_frame)
            grains_added_list.append(
                current_grain_count if current_grain_count is not None else -1
            )

    return frames, grains_added_list


def plot_avalanche_histogram(avalanche_sizes):
    size_counts = {}
    for size in avalanche_sizes:
        if size in size_counts:
            size_counts[size] += 1
        else:
            size_counts[size] = 1

    sizes = np.array(list(size_counts.keys()))  # rozmiary lawin
    counts = np.array(list(size_counts.values()))  # liczba wystąpień
    prob = counts / np.sum(counts)
    log_sizes = np.log(sizes)
    log_prob = np.log(prob)
    tau, _, _, _ = np.linalg.lstsq(log_sizes[:, np.newaxis], log_prob, rcond=None)

    tau = tau[0]
    # Obliczenie błędu standardowego
    residuals = log_prob - tau * log_sizes
    dof = len(log_sizes) - 1  # degrees of freedom (1 parametr)
    res_var = np.sum(residuals**2) / dof
    std_tau = np.sqrt(res_var / np.sum(log_sizes**2))
    fitted_prob = sizes**tau
    print(f"Slope: {tau:.4f} ± {std_tau:.4f}")
    plt.figure(figsize=(8, 6))
    plt.scatter(sizes, prob, color="orange", label="P(s) z symulacji")
    plt.plot(
        sizes, fitted_prob, label=rf"Regresja: $s^{{{tau:.4f} \pm {std_tau:.4f}}}$"
    )
    plt.title("Histogram rozmiarów lawin")
    plt.xlabel("Rozmiar lawiny (liczba zsypań)")
    plt.ylabel("Prawdopodobieństwo lawiny")
    plt.yscale("log")
    plt.xscale("log")
    plt.legend()
    plt.tight_layout()
    plt.savefig("plots/histogram_lawin.pdf")
    plt.show()


def animate(grid_evo, grains_added_list, N, fps):
    # Customowy colormap dla wartości 0–4
    colors = ["black", "blue", "green", "orange", "red"]
    cmap = ListedColormap(colors)
    bounds = [0, 1, 2, 3, 4, 5]
    norm = BoundaryNorm(bounds, cmap.N)

    # Tworzenie wykresu
    fig, ax = plt.subplots()
    fig.suptitle("Ewolucja gridu (model piasku)")
    img = ax.imshow(np.array(grid_evo[0]), cmap=cmap, norm=norm)
    ax.set_xlabel("x")
    ax.set_ylabel("y")

    # Colorbar z wartościami 0–4
    cbar = fig.colorbar(img, ax=ax, ticks=[0, 1, 2, 3, 4])
    cbar.set_label("Liczba ziaren")

    text_elements = []

    def update(frame):
        nonlocal text_elements
        for txt in text_elements:
            txt.remove()
        text_elements.clear()

        data = np.array(grid_evo[frame])
        img.set_data(data)
        ax.set_title(f"Grains added = {grains_added_list[frame]}")

        for i in range(data.shape[0]):
            for j in range(data.shape[1]):
                text = ax.text(
                    j,
                    i,
                    str(data[i][j]),
                    ha="center",
                    va="center",
                    color="white",
                    fontsize=8,
                    fontweight="bold",
                )
                text_elements.append(text)

        return (img, *text_elements)

    # Animacja
    ani = animation.FuncAnimation(
        fig, update, frames=range(0, len(grid_evo), 1), interval=1000 // fps, blit=True
    )

    # Zapis do GIF-a
    ani.save(f"plots/animacja_grid_N={N}.gif", writer="pillow", fps=fps)
    ani.save(f"plots/animacja_grid_N={N}.mp4", writer="ffmpeg", fps=fps)

    plt.close()


# Wczytanie danych
filename = "data/output_N10_grid_all.csv"
grid_evo, grains_added_list = load_grid_evolution(filename)
print("animation rendering start N=1")
animate(grid_evo, grains_added_list, N=11, fps=1)

# Wczytanie danych z pliku CSV
filename = "data\output_N20_avalanche.csv"  # Zmień nazwę pliku na odpowiednią
avalanche_sizes = load_avalanche_sizes_from_csv(filename)
plot_avalanche_histogram(avalanche_sizes)

filename = "data/output_N20_grid_all.csv"
grid_evo, grains_added_list = load_grid_evolution(filename)
print("animation rendering start N=20")
animate(grid_evo, grains_added_list, N=20, fps=20)
