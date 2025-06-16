from turtle import color
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from sympy import N

plt.style.use("dark_background")

# ------------------------- Zadanie 1: Trajektorie -------------------------
fig1, ax1 = plt.subplots()
with open("data/zad1_d_vs_N.txt") as f:
    traj = []
    for line in f:
        if line.strip() == "":
            if traj:
                data = np.array(traj)
                ax1.plot(data[:, 0], data[:, 1])
                traj = []
        elif line.startswith("#"):
            continue
        else:
            traj.append([int(x) for x in line.strip().split()])
    if traj:
        data = np.array(traj)
        ax1.plot(data[:, 0], data[:, 1])
ax1.set_xlabel("N")
ax1.set_ylabel("d")
fig1.suptitle("Zadanie 1: Błądzenie losowe 1D – 5 trajektorii")
fig1.savefig("plots/zad1_trajektorie.pdf")

# ------------------------- Zadanie 1: Histogram -------------------------
hist_data = np.loadtxt("data/zad1_histogram.txt")
fig2, ax2 = plt.subplots()
ax2.bar(hist_data[:, 0], hist_data[:, 1], width=1.8)
ax2.set_xlabel("d po N=20 krokach")
ax2.set_ylabel("P(d)")
fig2.suptitle("Zadanie 1: Histogram odległości po N=20 (10⁶ prób)")
fig2.savefig("plots/zad1_histogram.pdf")

# ------------------------- Zadanie 2: Heatmapa -------------------------
heatmap_data = np.loadtxt("data/zad2_heatmap.txt")
M = int(np.sqrt(len(heatmap_data)))
visits = np.zeros((M, M))
for row in heatmap_data:
    x, y, v = map(int, row)
    visits[y, x] = v

fig3, ax3 = plt.subplots()
c = ax3.imshow(visits, cmap="plasma", origin="lower")
fig3.colorbar(c, ax=ax3)
ax3.set_title("Zadanie 2: Częstość odwiedzin (M=10, N=10⁶)")
fig3.savefig("plots/zad2_heatmap.pdf")

# ------------------------- Zadanie 3: Odległości -------------------------
dist_data = np.loadtxt("data/zad3_distances.txt")
fig4, ax4 = plt.subplots()
ax4.plot(
    dist_data[:, 0],
    dist_data[:, 1],
    label="średnia z symulacji",
    marker="o",
    markersize=3,
)
ax4.plot(
    dist_data[:, 0],
    dist_data[:, 2],
    label=r"$\Gamma(\frac{3}{2}) \cdot \sqrt{N}$",
    color="#ff5555",
    linestyle="--",
    linewidth=1.5,
)
ax4.set_xlabel("N")
ax4.set_ylabel("d")
ax4.legend()
fig4.suptitle("Zadanie 3: Średnia odległość w 2D (ciągłe)")
fig4.savefig("plots/zad3_distance_vs_N.pdf")


# data3 = np.loadtxt("data/zad3_walking_pattern.txt")
# N_vals = data3[:, 0]
# x_vals = data3[:, 1]
# y_vals = data3[:, 2]

# # === INICJALIZACJA WYKRESU ===
# fig5, ax5 = plt.subplots()
# scat = ax5.scatter([x_vals[0]], [y_vals[0]], s=30, label="Punkt", zorder=2)
# (line_obj,) = ax5.plot(
#     x_vals[0:1], y_vals[0:1], color="#ff5555", label="Trasa", zorder=1
# )
# ax5.set_ylim(y_vals.min(), y_vals.max())
# ax5.set_xlim(x_vals.min(), x_vals.max())
# ax5.set_xlabel("x")
# ax5.set_ylabel("y")
# title = ax5.set_title(f"N={int(N_vals[0])}")
# ax5.legend(loc="upper right")


# # === AKTUALIZACJA KLATKI ===
# def update3(frame):
#     scat.set_offsets([[x_vals[frame], y_vals[frame]]])
#     line_obj.set_data(x_vals[: frame + 1], y_vals[: frame + 1])
#     title.set_text(f"N={int(N_vals[frame])}")
#     return scat, line_obj, title


# # === ANIMACJA ===
# ani = animation.FuncAnimation(fig5, update3, frames=len(N_vals), interval=40, blit=True)
# plt.tight_layout()

# # === ZAPIS DO PLIKÓW ===
# mp4_filename = "plots/zad3_walking_pattern.mp4"
# print(f"Zapisuję animację do {mp4_filename}...")
# ani.save(mp4_filename, writer="ffmpeg", fps=25)
# print("Gotowe.")


# ------------------------- Zadanie 4: Odległości random step_size -------------------------
dist_data = np.loadtxt("data/zad4_distances.txt")
fig6, ax6 = plt.subplots()
ax6.plot(
    dist_data[:, 0],
    dist_data[:, 1],
    label="średnia z symulacji",
    marker="o",
    markersize=3,
)
ax6.plot(
    dist_data[:, 0],
    dist_data[:, 2],
    label=r"$\hat{l} \cdot \Gamma(\frac{3}{2}) \cdot \sqrt{N}$",
    color="#ff5555",
    linestyle="--",
    linewidth=1.5,
)
ax6.set_xlabel("N")
ax6.set_ylabel("d")
ax6.legend()
fig6.suptitle("Zadanie 4: Średnia odległość w 2D (ciągłe) - random step_size")
fig6.savefig("plots/zad4_distance_vs_N.pdf")


# data4 = np.loadtxt("data/zad4_walking_pattern.txt")
# N_vals = data4[:, 0]
# x_vals = data4[:, 1]
# y_vals = data4[:, 2]

# # === INICJALIZACJA WYKRESU ===
# fig7, ax7 = plt.subplots()
# scat = ax7.scatter([x_vals[0]], [y_vals[0]], s=30, label="Punkt", zorder=2)
# (line_obj,) = ax7.plot(
#     x_vals[0:1], y_vals[0:1], color="#ff5555", label="Trasa", zorder=1
# )
# ax7.set_ylim(y_vals.min(), y_vals.max())
# ax7.set_xlim(x_vals.min(), x_vals.max())
# ax7.set_xlabel("x")
# ax7.set_ylabel("y")
# title = ax7.set_title(f"Random STEPSIZE - N={int(N_vals[0])}")
# ax7.legend(loc="upper right")


# # === AKTUALIZACJA KLATKI ===
# def update4(frame):
#     scat.set_offsets([[x_vals[frame], y_vals[frame]]])
#     line_obj.set_data(x_vals[: frame + 1], y_vals[: frame + 1])
#     title.set_text(f"Random STEPSIZE -  N={int(N_vals[frame])}")
#     return scat, line_obj, title


# # === ANIMACJA ===
# ani = animation.FuncAnimation(fig7, update4, frames=len(N_vals), interval=40, blit=True)
# plt.tight_layout()

# # === ZAPIS DO PLIKÓW ===
# mp4_filename = "plots/zad4_walking_pattern.mp4"
# print(f"Zapisuję animację do {mp4_filename}...")
# ani.save(mp4_filename, writer="ffmpeg", fps=25)
# print("Gotowe.")
plt.close("all")
print("Wszystkie wykresy zapisane w katalogu 'plots/'")
