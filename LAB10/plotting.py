import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
from matplotlib.animation import FuncAnimation
import networkx as nx

plt.style.use("dark_background")


def power_law(k, A, alpha):
    return A * k ** (-alpha)


def plot_degree_distribution(graph_name: str):
    data = np.loadtxt(f"data/{graph_name}_degree_distribution.txt", delimiter=" ")
    x = data[:, 0]
    distr = data[:, 1]

    nonzero = (x > 0) & (distr > 0)
    x_fit = x[nonzero]
    distr_fit = distr[nonzero]

    # Dopasowanie rozkładu potęgowego
    popt, _ = curve_fit(power_law, x_fit, distr_fit)
    A, alpha = popt

    # Wartości dopasowanej funkcji
    fitted_powerlaw = power_law(x_fit, A, alpha)

    # Wykres
    fig, ax = plt.subplots(figsize=(10, 6))

    # Słupki dla danych symulacyjnych
    ax.bar(x, distr, width=1.0, label="symulacja", alpha=0.7)

    # Wykres funkcji potęgowej
    ax.plot(
        x_fit,
        fitted_powerlaw,
        label=rf"$A \cdot k^{{-\alpha}}$, $\alpha$ = {alpha:.2f}",
        color="#FF4C4C",
        linewidth=2,
    )

    # Skale logarytmiczne i opisy
    ax.set_xlabel("Stopień")
    ax.set_ylabel("Liczność")
    ax.set_yscale("log")
    ax.set_xscale("log")
    fig.suptitle(rf"Rozkład stopni dla {graph_name}")
    fig.legend()
    fig.tight_layout()
    fig.savefig(f"plots/{graph_name}_degree_distribution.pdf")


def plot_graph(graph_name: str):
    """
    Wczytuje listę sąsiedztwa z pliku i rysuje graf.

    Plik powinien mieć format:
    0: 1 2 3
    1: 0 4
    ...
    """

    adj_list = load_neighbours_list(f"data/{graph_name}.txt")
    # Tworzenie grafu
    G = build_graph(adj_list)

    fig, ax = plt.subplots(figsize=(10, 10))
    # Rysowanie grafu
    pos = nx.spring_layout(G, seed=42)  # stały układ
    nx.draw(
        G,
        pos,
        node_color="#FF4C4C",
        edge_color="white",
        font_color="white",
        node_size=100,
        font_size=10,
        ax=ax,
    )

    fig.suptitle(f"{graph_name}", color="white")
    fig.savefig(f"plots/{graph_name}_visualization.pdf")


def load_neighbours_list(filename):
    """Wczytuje listę sąsiedztwa z pliku."""
    adj_list = {}
    with open(filename, "r") as f:
        for line in f:
            if ":" in line:
                node_str, neighbors_str = line.strip().split(":")
                node = int(node_str)
                neighbors = list(map(int, neighbors_str.strip().split()))
                adj_list[node] = neighbors
    return adj_list


def build_graph(adj_list):
    """Buduje graf z listy sąsiedztwa."""
    G = nx.Graph()
    for node, neighbors in adj_list.items():
        for neighbor in neighbors:
            G.add_edge(node, neighbor)
    return G


def animate_graph(graph_name_prefix, first_index: int = 1):
    """
    Tworzy animację wyświetlającą kolejne grafy wczytywane z plików
    {graph_name_prefix}_0.txt, {graph_name_prefix}_1.txt, ...
    """
    # Szukanie wszystkich plików pasujących do wzorca
    files = [f"data/{graph_name_prefix}_{i}.txt" for i in range(first_index, 100)]
    if not files:
        print("Nie znaleziono żadnych plików.")
        return

    # Przygotowanie figury
    plt.style.use("dark_background")
    fig, ax = plt.subplots(figsize=(10, 10))
    last_adj_list = load_neighbours_list(files[-1])
    last_G = build_graph(last_adj_list)
    pos = nx.spring_layout(last_G, seed=42)

    def update(frame):
        ax.clear()
        filename = files[frame]
        adj_list = load_neighbours_list(filename)
        G = build_graph(adj_list)
        nx.draw(
            G,
            pos,
            with_labels=True,
            node_color="#FF4C4C",
            edge_color="white",
            font_color="white",
            node_size=100,
            font_size=10,
            ax=ax,
        )
        ax.set_title(
            f"Graf: {graph_name_prefix} - {frame + first_index}", color="white"
        )

    anim = FuncAnimation(fig, update, frames=len(files), interval=250, repeat=True)
    anim.save(f"plots/{graph_name_prefix}_animation.gif", writer="pillow", fps=4)


plot_degree_distribution("graph1")
plot_degree_distribution("graph2")
plot_degree_distribution("graph3")

plot_graph("graph1")
plot_graph("graph3")

animate_graph("graph1", 5)
animate_graph("graph3", 1)
