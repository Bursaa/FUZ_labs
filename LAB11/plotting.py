import numpy as np
import matplotlib.pyplot as plt

plt.style.use("dark_background")

Ns = [10, 20, 50]
Ks = np.arange(0.25, 5.01, 0.25)

fig_r, ax_r = plt.subplots()
for N in Ns:
    r_finish = []
    for K in Ks:
        data = np.loadtxt(f"data/kuramoto_N={N}_K={K:.2f}.txt")
        time = data[:, 0]
        r = data[:, 1]
        r_finish.append(r[-1])
        fig, ax = plt.subplots()
        ax.plot(time, data[:, 2:] / np.pi)
        ax.set_xlabel("t")
        ax.set_ylabel(r"$\theta$ [$\pi$]")
        ax.set_ylim(0.0, 2.0)
        fig.suptitle(rf"$\theta(t)$ N={N} K={K:.2f}")
        fig.savefig(f"plots/theta(t)_N={N}_K={K:.2f}.pdf")
        plt.close()

    ax_r.plot(Ks, np.array(r_finish), label=f"N={N}")
    ax_r.set_xlabel("K")
    ax_r.set_ylabel(r"$\theta$")
    ax_r.legend()
    fig_r.suptitle(rf"$r_{{finish}}(K)$")
    fig_r.savefig(f"plots/r_finish(K).pdf")

N = 25
Ks = [0.01, 0.8, 2.0]
max_theta = {"2pi": r"$2 \cdot \pi$", "pi_12": r"$\frac{\pi}{12}$"}

fig3, ax3 = plt.subplots()
for K in Ks:
    for key, val in max_theta.items():
        data = np.loadtxt(f"data/kuramoto_N={N}_K={K:.2f}_max_theta={key}.txt")
        ax3.plot(
            data[:, 0], data[:, 1], label=rf"r(t) K={K:.2f} $\theta_{{max}}$={val}"
        )
ax3.set_xlabel("t")
ax3.set_ylabel("r")
ax3.legend()
fig3.suptitle(rf"r(t) diffrent K, dirffrent $\theta_{{max}}$")
fig3.savefig(r"plots/r(t)_diff_K_theta.pdf")
plt.close("all")
