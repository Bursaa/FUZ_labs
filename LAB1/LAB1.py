import numpy as np
import networkx as nx
import matplotlib.pyplot as plt

N = 100


# Zadanie 1: Pełny graf o N wierzchołkach
def fully_connected_graph(N):
    adjacency_matrix = np.ones((N, N)) - np.eye(N)
    degrees = np.sum(adjacency_matrix, axis=1)

    plt.hist(
        degrees, bins=np.arange(min(degrees), max(degrees) + 2) - 0.5, edgecolor="black"
    )
    plt.xlabel("Stopień wierzchołka")
    plt.ylabel("Liczba wierzchołków")
    plt.title(f"Rozkład stopni wierzchołków - Pełny graf - N={N}")
    plt.savefig(f"output/full_graph_N={N}.png")
    plt.show()

    return adjacency_matrix


# Zadanie 2: Graf losowy z prawdopodobieństwem połączenia 1/3
def random_graph(N, p=1 / 3):
    G = nx.erdos_renyi_graph(N, p)
    adjacency_list = {node: list(G.neighbors(node)) for node in G.nodes()}
    degrees = [deg for _, deg in G.degree()]

    plt.hist(
        degrees, bins=np.arange(min(degrees), max(degrees) + 2) - 0.5, edgecolor="black"
    )
    plt.xlabel("Stopień wierzchołka")
    plt.ylabel("Liczba wierzchołków")
    plt.title(f"Rozkład stopni wierzchołków - Graf losowy - N={N}")
    plt.savefig(f"output/random_graph_N={N}.png")
    plt.show()

    return adjacency_list


# Zadanie 3: Tablica 2D z jedynkami w określonych miejscach
def create_2d_array(N):
    array = np.zeros((N, N), dtype=int)
    size = N // 2
    start = (N - size) // 2

    # Wewnętrzny kwadrat
    array[start : start + size, start : start + size] = 1
    print("Kwadrat:")
    print(array)

    # Przekątna od lewego górnego do prawego dolnego
    array = np.zeros((N, N), dtype=int)
    np.fill_diagonal(array, 1)
    print("Przekątna główna:")
    print(array)

    # Przekątna od prawego górnego do lewego dolnego
    array = np.zeros((N, N), dtype=int)
    np.fill_diagonal(np.fliplr(array), 1)
    print("Przekątna przeciwna:")
    print(array)

    # Obie przekątne
    array = np.zeros((N, N), dtype=int)
    np.fill_diagonal(array, 1)
    np.fill_diagonal(np.fliplr(array), 1)
    print("Obie przekątne:")
    print(array)

    # Szachownica
    array = np.zeros((N, N), dtype=int)
    array[::2, ::2] = 1
    array[1::2, 1::2] = 1
    print("Szachownica:")
    print(array)

    return array


array = fully_connected_graph(N)
array = random_graph(N)
array = create_2d_array(N)
