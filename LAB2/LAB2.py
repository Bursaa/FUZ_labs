import numpy as np
import matplotlib.pyplot as plt


def count_neighbors(grid):
    """
    Counts the number of neighbors
    Input: grid - array of boolean values
    Output: neighbors - array of integers representing the number of neighbors for each cell in the grid
    """
    neighbors = sum(
        np.roll(np.roll(grid, i, axis=0), j, axis=1)
        for i in [-1, 0, 1]
        for j in [-1, 0, 1]
        if (i, j) != (0, 0)
    )
    return neighbors


def apply_rule(grid):
    """
    Applys the regules of B4678/S35678 rule into the grid
    Input: grid - array of boolean values
    Output: new_grid - array of boolean values after applying the Game of Life rules

    """
    neighbors = count_neighbors(grid)
    birth = (neighbors == 4) | (neighbors == 6) | (neighbors == 7) | (neighbors == 8)
    survive = (
        (neighbors == 3)
        | (neighbors == 5)
        | (neighbors == 6)
        | (neighbors == 7)
        | (neighbors == 8)
    )
    new_grid = np.where(grid, survive, birth)
    return new_grid


def visualize_grid(grid, t, run):
    """
    Visualizes the grid of a given time series
    Input: grid - array of boolean values representing the grid at a given time step
           t - current time step
           run - current run number
    """
    plt.clf()
    plt.imshow(grid, cmap="gray_r", interpolation="nearest")
    plt.title(f"t = {t}, run = {run}")
    plt.axis("off")
    plt.pause(0.01)


def simulate(run):
    """
    Simulates the Game of Life with random grid filling
    Input: run - current run number
    """
    grid = np.random.choice([0, 1], size=(60, 60))
    timesteps = [0, 1, 2, 5, 10, 50, 100]
    plt.figure(figsize=(6, 6))
    for t in range(101):
        visualize_grid(grid, t, run)
        if t in timesteps:
            plt.savefig(f"output/t_{t}_run_{run}.png")
        grid = apply_rule(grid)
    plt.show()


if __name__ == "__main__":
    for run in range(1, 4):
        simulate(run)
