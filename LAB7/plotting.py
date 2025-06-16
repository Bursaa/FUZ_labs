import matplotlib.pyplot as plt
import numpy as np
from matplotlib.colors import ListedColormap


plt.style.use("dark_background")
Ls = [16, 32, 64, 128, 256]
R = 1000
p_min = 0.4
p_max = 0.8
p_step = 0.01
p_values = np.arange(p_min, p_max + p_step, p_step)
custom_greys = ListedColormap(["white", "green"])

percolation_arr = np.loadtxt("data/percolation.txt")
for p_index, p in enumerate(p_values):
    p_str = f"{p:.2f}"

    grid_arr = []
    labels_arr = []
    for L in Ls:
        grid_arr.append(np.loadtxt(f"data/grid_p_{p_str}L_{L}.txt"))
        labels_arr.append(np.loadtxt(f"data/labels_p_{p_str}L_{L}.txt"))

    fig, axes = plt.subplots(len(Ls), 2, figsize=(10, len(Ls) * 5))
    fig.suptitle(f"p = {p_str}", fontsize=16)

    for i, L in enumerate(Ls):
        # Lewy wykres – grid
        grid = grid_arr[i]
        im0 = axes[i, 0].imshow(grid, cmap=custom_greys, origin="upper")
        axes[i, 0].set_title(
            f"Grid (Occupancy) L={L} percolation={percolation_arr[i, p_index]}"
        )
        axes[i, 0].axis("off")
        cbar0 = fig.colorbar(im0, ax=axes[i, 0], fraction=0.046, pad=0.04)
        cbar0.set_ticks([0, 1])
        cbar0.set_ticklabels(["0", "1"])
        cbar0.set_label("Occupancy")

        labels = labels_arr[i]
        # Prawy wykres – labels
        im1 = axes[i, 1].imshow(labels, cmap="tab20", origin="upper")
        axes[i, 1].set_title(f"Labels (Clusters) L={L}")
        axes[i, 1].axis("off")
        cbar1 = fig.colorbar(im1, ax=axes[i, 1], fraction=0.046, pad=0.04)
        cbar1.set_label("Cluster ID")

    # Zapisz do pliku
    output_path = f"plots/combined_p_{p_str}.pdf"
    fig.tight_layout()
    fig.savefig(output_path, bbox_inches="tight")
    plt.close()


W_arr = []
for L in Ls:
    W_arr.append(np.loadtxt(f"data/W_L_{L}.txt"))


fig, ax = plt.subplots(figsize=(10, 7))
for i, L in enumerate(Ls):
    W_L = W_arr[i]
    ax.plot(p_values, W_L, label=f"W(p) for L={L}")
ax.set_xlabel("p")
ax.set_ylabel("W")
ax.legend()
fig.suptitle("W(p,L)")
fig.tight_layout()
fig.savefig(f"plots/W(p,L).pdf", bbox_inches="tight")
plt.close()
