import matplotlib.pyplot as plt
import numpy as np

plt.style.use("dark_background")
N = 10**3
tau = 10**4
Temp = np.arange(0.50, 4.25, 0.25)
Temp1 = np.array([0.5, 1.0, 1.5, 4.0])

e_ana = -np.tanh(Temp)
c_ana = Temp * Temp / (np.cosh(Temp) * np.cosh(Temp))

e = []
c = []
for T in Temp1:
    plt.figure()
    data1 = np.loadtxt(f"data/energies{T:.2f}_m=0.txt")
    data2 = np.loadtxt(f"data/energies{T:.2f}_m=1.txt")
    plt.plot(data1[:] / N, label=f"m=0, T={T:.2f}")
    plt.plot(data2[:] / N, label=f"m=1, T={T:.2f}")
    plt.legend()
    plt.title(f"Energy Landscape for m=0 and m=1, T={T:.2f}")
    plt.xlabel("t")
    plt.ylabel("E")
    plt.savefig(f"plots/energy_landscape_{T:.2f}.pdf")
    plt.close()

for T in Temp:
    data1 = np.loadtxt(f"data/energies{T:.2f}_m=0.txt")
    E_sr = np.mean(data1[-tau:])
    E_sr2 = np.mean(data1[-tau:] * data1[-tau:])
    e.append(E_sr / N)
    c.append((E_sr2 - E_sr**2) * T**2 / N)

e = np.array(e)
c = np.array(c)


plt.figure()
plt.plot(Temp, e_ana, label="Analytical energy dencity")
plt.scatter(Temp, e, label="Simulation energy dencity")
plt.legend()
plt.title("Energy Dencity")
plt.xlabel("1/T")
plt.ylabel("Energy density")
plt.savefig("plots/Energy_dencity.pdf")
plt.close()

plt.figure()
plt.plot(Temp, c_ana, label="Analitical heat capacity")
plt.scatter(Temp, c, label="Simulation heat capacity")
plt.legend()
plt.title("Heat Capacity")
plt.xlabel("1/T")
plt.ylabel("Heat Capacity")
plt.savefig("plots/Heat_capacity.pdf")
