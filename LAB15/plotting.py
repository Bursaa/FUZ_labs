import numpy as np
import matplotlib.pyplot as plt
import os

plt.style.use("dark_background")


def plot_pattern(ax, pattern, title, size=(5, 5)):
    """Rysuje pojedynczy wzorzec na podanej osi."""
    pattern = np.array(pattern, dtype=float)
    pattern = (pattern + 1) / 2  # -1 → 0, 1 → 1
    ax.imshow(pattern.reshape(size), cmap="plasma", vmin=0, vmax=1)
    ax.set_title(title)
    ax.axis("off")


def visualize_custom_3(filename, size=(5, 5), title=None):
    data = np.loadtxt(filename)
    titles = ["Wzorzec oczekiwany", "Wzorzec testowy", "Wynik"]

    fig = plt.figure(figsize=(7, 6))

    # Grid layout: 2 rows, 2 cols → dolny subplot zajmuje obie kolumny
    ax1 = plt.subplot2grid((2, 2), (0, 0))  # górny lewy
    ax2 = plt.subplot2grid((2, 2), (0, 1))  # górny prawy
    ax3 = plt.subplot2grid((2, 2), (1, 0), colspan=2)  # dolny, na środku

    plot_pattern(ax1, data[0], titles[0], size)
    plot_pattern(ax2, data[1], titles[1], size)
    plot_pattern(ax3, data[2], titles[2], size)
    if title:
        fig.suptitle(title)
    fig.tight_layout()
    base_filename = os.path.basename(filename)
    name_without_ext = os.path.splitext(base_filename)[0]
    fig.savefig(f"plots/{name_without_ext}.pdf")


# Przykład użycia:
title1 = "Wyniki dla szkolenia na 3 patternach (A, H, T)"
title2 = "Wyniki dla szkolenia na 4 patternach (A, H, T, E)"
visualize_custom_3("data/iter_hopfield_test1_T.txt", size=(5, 5), title=title1)
visualize_custom_3("data/iter_hopfield_test1_A.txt", size=(5, 5), title=title1)
visualize_custom_3("data/iter_hopfield_test1_H.txt", size=(5, 5), title=title1)

visualize_custom_3("data/iter_hopfield_test2_T.txt", size=(5, 5), title=title2)
visualize_custom_3("data/iter_hopfield_test2_A.txt", size=(5, 5), title=title2)
visualize_custom_3("data/iter_hopfield_test2_H.txt", size=(5, 5), title=title2)
visualize_custom_3("data/iter_hopfield_test2_E.txt", size=(5, 5), title=title2)
