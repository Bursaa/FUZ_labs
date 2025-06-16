from turtle import color
import numpy as np
import matplotlib.pyplot as plt

plt.style.use("dark_background")


def evolve_population(x_n, r):
    return r * x_n * (1 - x_n)


def first_task():
    """
    Symuluje iteracje równania logistycznego dla różnych wartości początkowych x_0
    i wyświetla wykres pokazujący ewolucję populacji w czasie.
    """
    n_1 = 50  # liczba iteracji
    r = 2  # współczynnik wzrostu
    x_0_values = [i / 10 for i in range(1, 10)]  # różne wartości początkowe

    plt.figure(figsize=(8, 6))

    for x_0 in x_0_values:
        x_n = x_0
        trajectory = [x_n]

        for _ in range(n_1):
            x_n = evolve_population(x_n, r)
            trajectory.append(x_n)

        plt.plot(trajectory, label=f"x_0={x_0:.1f}")

    plt.xlabel("Iteracja")
    plt.xscale("log")
    plt.ylabel("x_n")
    plt.title("Ewolucja populacji dla różnych wartości początkowych")
    plt.legend()
    plt.savefig(f"output/ewolucja_pop_x0={x_0:.1f}.pdf")
    plt.show()


def second_task():
    """
    Symuluje iteracje równania logistycznego dla różnych wartości początkowych x_0
    i wyświetla wykres pokazujący ewolucję populacji w czasie.
    """
    n_2 = 100  # liczba iteracji
    r_values = [1, 2, 3, 3.5, 3.55, 3.6]  # współczynnik wzrostu
    x_0 = 0.5  # różne wartości początkowe

    for r in r_values:
        x_n = x_0
        trajectory = [x_n]

        for _ in range(n_2):
            x_n = evolve_population(x_n, r)
            trajectory.append(x_n)

        plt.figure(figsize=(8, 6))
        plt.plot(trajectory, label=f"r={r:.1f}")
        plt.xlabel("Iteracja")
        plt.ylabel("x_n")
        plt.title(f"Ewolucja populacji dla r={r:.1f}")
        plt.legend()
        plt.savefig(f"output/ewolucja_pop_r={r:.1f}.pdf")
        plt.show()


def third_task():
    """
    Symuluje iteracje równania logistycznego dla różnych wartości r
    i wyświetla wykres bifurkacyjny, optymalizując rysowanie.
    """
    n_3 = 10000  # liczba iteracji
    r_values = [i / 100 for i in range(100, 400)]  # współczynnik wzrostu
    x_0 = 0.5  # wartość początkowa populacji

    plt.figure(figsize=(10, 6))
    plt.xlabel("r")
    plt.ylabel("x_n")
    plt.title("Ewolucja x_n(r)")

    all_r, all_x = [], []  # Listy na punkty do rysowania

    for r in r_values:
        print(r)
        x_n = x_0
        for j in range(n_3):
            x_n = evolve_population(x_n, r)
            if j >= 9000:  # Zapisujemy tylko ostatnie 1000 iteracji
                all_r.append(r)
                all_x.append(x_n)

        plt.scatter(all_r, all_x, color="red", s=0.01)
        all_r, all_x = [], []  # Resetujemy listy
    plt.savefig("output/x_n(r).pdf")
    plt.show()


if __name__ == "__main__":
    first_task()
    second_task()
    third_task()
