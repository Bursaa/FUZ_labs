import numpy as np
import matplotlib.pyplot as plt

plt.style.use("dark_background")
# Wczytaj dane z pliku CSV (rozdzielane przecinkami)
data1 = np.loadtxt("data/degree_dist_alpha_4.txt", delimiter=" ")
data2 = np.loadtxt("data/degree_dist_alpha_10.txt", delimiter=" ")


# Rozdzielenie danych na kolumny
def plot_histogram(data, alpha):
    x = data[:, 0]
    sim = data[:, 1]
    poisson = data[:, 2]

    # Filtrowanie tylko tych wartości, gdzie sim > 0
    mask = sim > 0
    x_filtered = x[mask]
    sim_filtered = sim[mask]
    poisson_filtered = poisson[mask]

    # Rysowanie wykresu
    fig, ax = plt.subplots(figsize=(10, 6))

    # Wykres słupkowy dla simulation
    ax.bar(
        x_filtered,
        sim_filtered,
        width=0.8,
        label="simulation",
        alpha=0.7,
    )

    # Linia dla Poisson – na tych samych x co simulation (po odfiltrowaniu)
    ax.plot(x_filtered, poisson_filtered, label="poisson", color="red", linewidth=2)

    # Opisy i siatka
    ax.set_xlabel("Stopień")
    ax.set_ylabel("Liczność")
    fig.suptitle(rf"Rozkład stopni dla $\alpha$ = {alpha}: symulacja vs Poisson")
    fig.legend()
    fig.tight_layout()
    fig.savefig(f"plots/degree_dist_alpha_{alpha}.pdf")


plot_histogram(data1, 4)
plot_histogram(data2, 10)
plt.show()
