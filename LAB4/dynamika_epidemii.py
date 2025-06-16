import numpy as np
import matplotlib.pyplot as plt

plt.style.use("dark_background")

# Parametry symulacji
N = 60
T_MAX = 100
BETA = 0.3
ALPHAS = np.arange(0.1, 1.0, 0.1)
SNAPSHOTS = [0, 1, 2, 5, 10, 50, 100]


# Funkcja symulacji układu na siatce
def simulate(alpha, beta, t_max, snapshots):
    grid = np.zeros((N, N))
    grid[N // 2, N // 2] = 1  # Początkowa infekcja w centrum

    results = []
    counts = {"S": [], "I": [], "R": []}

    for t in range(t_max + 1):

        new_grid = np.zeros((N, N))
        if t in snapshots:
            results.append(grid.copy())
        for i in range(N):
            for j in range(N):
                if grid[i, j] == 1:  # Zainfekowany
                    if np.random.rand() < beta:
                        new_grid[i, j] = 2  # Ozdrowiał
                    else:
                        new_grid[i, j] = 1
                elif grid[i, j] == 0:  # Zdrowy
                    neighbours = grid[
                        (i - 1) % N : i + 2, (j - 1) % N : j + 2
                    ].flatten()
                    count = np.sum(neighbours == 1)
                    if count > 0 and np.random.rand() < alpha * count:
                        new_grid[i, j] = 1
                elif grid[i, j] == 2:  # Ozdrowiały
                    new_grid[i, j] = 2  # Pozostaje ozdrowiały
        grid = new_grid.copy()

        counts["S"].append(np.sum(grid == 0))
        counts["I"].append(np.sum(grid == 1))
        counts["R"].append(np.sum(grid == 2))

    return results, counts


# Zadanie 1: Generowanie zdjęć stanów dla 3 wartości alpha
fig, axes = plt.subplots(len(ALPHAS[:3]), len(SNAPSHOTS), figsize=(15, 10))
fig.suptitle(f"Epidemy Evolution for B={BETA}")
for idx, alpha in enumerate(ALPHAS[:3]):
    snapshots, _ = simulate(alpha, BETA, T_MAX, SNAPSHOTS)
    for jdx, snap in enumerate(snapshots):
        axes[idx, jdx].imshow(snap, cmap="viridis", vmin=0, vmax=2)
        axes[idx, jdx].set_title(f"α={alpha:.1f}, t={SNAPSHOTS[jdx]}")
        axes[idx, jdx].axis("off")
plt.tight_layout()
plt.savefig(f"output/Epidemy_evolution_B={BETA}.pdf")
plt.show()

# Zadanie 2: Ewolucja liczby agentów I dla różnych α
fig, axes = plt.subplots(3, 1, figsize=(10, 15))
fig.suptitle(f"Evolution of agents number (S,I,R) for B={BETA}")
for alpha in ALPHAS:
    _, counts = simulate(alpha, BETA, T_MAX, [])
    for i, type in enumerate(["S", "I", "R"]):
        axes[i].plot(counts[type], label=f"α={alpha:.1f}")
        axes[i].set_xlabel("Czas")
        axes[i].set_ylabel(f"Liczba agentów w stanie {type}")
        axes[i].legend()
plt.savefig(f"output/Evolution_of_agents_number_B={BETA}.pdf")
plt.show()

# Krytyczna wartość λc dla β = 0.1
lambda_c = ALPHAS / (8 * BETA)
print(f"Przybliżone wartości λc dla B={BETA:.1f}:", lambda_c)
