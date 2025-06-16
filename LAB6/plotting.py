import matplotlib.pyplot as plt
import numpy as np

plt.style.use("dark_background")
L = 32
tau = 10**4
Temp = np.arange(0.25, 4.01, 0.25)
T_C = 2 / np.arcsinh(1)
temp_teor = np.linspace(0.25, 4.0, 10000)

m_teor = np.zeros_like(temp_teor)
mask = temp_teor < T_C
m_teor[mask] = np.power(1 - np.power(np.sinh(2 / temp_teor[mask]), -4), 1 / 8)

m = []
chi = []
for T in Temp:
    plt.figure()
    data = np.loadtxt(f"data/spin_map_{T:.2f}.txt")
    plt.imshow(data, cmap="viridis", interpolation="none", vmin=-1, vmax=1)
    plt.colorbar(label="spin Value")
    plt.title(f"Mapa wartości spinów 2D dla T={T:.2f}")
    plt.savefig(f"plots/spin_map_{T:.2f}.pdf")
    plt.close()

for T in Temp:
    data = np.loadtxt(f"data/magnetization_{T:.2f}.txt")
    plt.figure()
    plt.plot(data, label=f"m(t) for T={T:.2f}")
    plt.title("Magnetization density(t)")
    plt.xlabel("Iteration")
    plt.ylabel("Magnetization density")
    plt.savefig(f"plots/m(t)_T={T:.2f}.pdf")
    plt.close()
    M_sr = np.mean(data[-tau:])
    M_sr2 = np.mean(data[-tau:] * data[-tau:])
    m.append(M_sr / L**2)
    chi.append((M_sr2 - M_sr**2) / (L**2 * T))

m = np.array(m)
chi = np.array(chi)

min_m = np.min(m)
max_m = np.max(m)
min_chi = np.min(chi)
max_chi = np.max(chi)

x = np.array([T_C for _ in range(10000)])
y_m = np.linspace(min_m, max_m, 10000)
y_chi = np.linspace(min_chi, max_chi, 10000)
plt.figure()
plt.scatter(Temp, m, label="Simulation m")
plt.plot(temp_teor, m_teor, label="Teoretical m")
plt.plot(x, y_m, label="$T_C$", color="yellow")
plt.legend()
plt.title("Magnetization density")
plt.xlabel(r"$T \left[\frac{J}{k_B} \right]$")
plt.ylabel("Magnetization density")
plt.savefig("plots/magnetization_density.pdf")
plt.close()

plt.figure()
plt.scatter(Temp, chi, label="Simulation $\chi$")
plt.plot(x, y_chi, label="$T_C$", color="yellow")
plt.legend()
plt.title("Magnetic susceptibility")
plt.xlabel(r"$T \left[\frac{J}{k_B} \right]$")
plt.ylabel("Magnetic susceptibility")
plt.savefig("plots/Magnetic_susceptibility.pdf")
