from matplotlib.colors import Normalize
import pandas as pd
import matplotlib.pyplot as plt
import networkx as nx
from matplotlib.animation import FuncAnimation

plt.style.use("dark_background")
titles = {
    "zad1": "Zadanie 1: Błądzenie losowe w grafie",
    "zad2": "Zadanie 2: Błądzenie losowe w grafie z teleportacją",
    "zad3": "Zadanie 3: PageRank w grafie",
    "zad4": "Zadanie 4: PageRank w grafie z teleportacją",
}


def update(frame):
    ax.cla()
    probs = history.iloc[frame]

    # Rysujemy ręcznie krawędzie, aby kończyły się na krawędzi węzła
    node_radius = 0.08  # dobierz eksperymentalnie
    for u, v in G.edges():
        x1, y1 = pos[u]
        x2, y2 = pos[v]
        dx = x2 - x1
        dy = y2 - y1
        dist = (dx**2 + dy**2) ** 0.5
        if dist == 0:
            continue
        start = (x1 + node_radius * dx / dist, y1 + node_radius * dy / dist)
        end = (x2 - node_radius * dx / dist, y2 - node_radius * dy / dist)

        ax.annotate(
            "",
            xy=end,
            xytext=start,
            arrowprops=dict(arrowstyle="->", color="white", lw=1.5),
        )

    # Węzły
    nx.draw_networkx_nodes(
        G,
        pos,
        node_color=probs.values,
        node_size=1000,
        cmap=plt.cm.get_cmap("viridis"),
        vmin=0,
        vmax=1,
        ax=ax,
    )

    # Etykiety
    for i, (node, (x, y)) in enumerate(pos.items()):
        prob = probs.iloc[i]
        ax.text(
            x,
            y,
            f"{node}\n{prob:.3f}",
            ha="center",
            va="center",
            fontsize=8,
            color="white" if prob < 0.5 else "black",
            weight="bold",
        )

    ax.set_title(f"Iteracja {frame}")
    ax.axis("off")
    return []


for zad, title in titles.items():
    # Wczytaj graf i dane
    edges = pd.read_csv(f"data/graph_edges_{zad}.csv", header=None)
    history = pd.read_csv(f"data/iter_{zad}.csv", header=None)
    G = nx.DiGraph()
    G.add_edges_from(edges.values)

    # Layout grafu
    pos = nx.circular_layout(G)

    # Przygotuj figurę i oś
    fig, ax = plt.subplots(figsize=(10, 8))
    fig.suptitle(title)
    sm = plt.cm.ScalarMappable(
        cmap=plt.cm.get_cmap("viridis"), norm=Normalize(vmin=0, vmax=1)
    )
    cbar = plt.colorbar(sm, ax=ax)
    cbar.set_label("Prawdopodobieństwo")

    ani = FuncAnimation(fig, update, frames=len(history), interval=500)
    ani.save(f"plots/anim_{zad}.mp4", writer="ffmpeg", fps=2)
    plt.tight_layout()
    plt.close(fig)
