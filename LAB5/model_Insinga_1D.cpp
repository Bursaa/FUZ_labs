#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iomanip>
#include <sstream>

using namespace std;

const int MC_number = pow(10, 5);
const int N = pow(10, 3);
const double kB = 1;
const bool m = 1;

// Stała wartość J (sprzężenie) i B (pole zewnętrzne)
double J(int node_i, int node_j) { return 1; }
double B(int i, int j) { return 0; }

// Obliczanie zmiany energii
double deltaE(int node1, int node2, int node3)
{
    double E_prim = -J(node1, -node2) * node1 * (-node2) - B(node1, -node2) * node1 -
                    J(-node2, node3) * (-node2) * node3 - B(-node2, node3) * (-node2);
    double E = -J(node1, node2) * node1 * node2 - B(node1, node2) * node1 -
               J(node2, node3) * node2 * node3 - B(node2, node3) * node2;
    return E_prim - E;
}

// Obliczanie średniej energii
double energy(const vector<int> &nodes)
{
    double sum = 0;
    for (int i = 0; i < N; i++)
    {
        sum -= J(nodes[i], nodes[(i + 1) % N]) * nodes[i] * nodes[(i + 1) % N] +
               B(nodes[i], nodes[(i + 1) % N]) * nodes[i];
    }
    return sum;
}

// Symulacja Monte Carlo
void MC_simulation(vector<int> &nodes, double T)
{
    for (int i = 0; i < N; i++)
    {
        int r = rand() % N;
        int node_r = nodes[r];
        double dE = deltaE(nodes[(r - 1 + N) % N], nodes[r], nodes[(r + 1) % N]);

        if (dE < 0.0)
        {
            nodes[r] = -nodes[r];
        }
        else
        {
            double p = min(1.0, exp(-dE * T / kB));
            double rand_num = (double)rand() / RAND_MAX;
            if (rand_num < p)
            {
                nodes[r] = -nodes[r];
            }
        }
    }
}

int main()
{

    vector<double> Temp;
    double T_min = 0.5, T_max = 4.0, step = 0.25;

    // Generowanie wartości temperatury
    for (double T = T_min; T <= T_max; T += step)
    {
        Temp.push_back(T);
    }
    // Pętla po temperaturach i Monte Carlo
    for (double T : Temp)
    {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2) << T;
        string T_str = ss.str();
        // Otwieranie pliku do zapisu
        ofstream Efile(("data/energies" + T_str + "_m=1.txt").c_str());

        srand(time(nullptr)); // Inicjalizacja generatora liczb pseudolosowych

        vector<int> nodes(N);

        // Inicjalizacja węzłów
        for (int i = 0; i < N; i++)
        {
            nodes[i] = (m == 0) ? (rand() % 2 ? -1 : 1) : 1;
        }
        for (int i = 0; i < MC_number; i++)
        {
            Efile << energy(nodes) << endl;
            MC_simulation(nodes, T);
        };
        // Zamknięcie pliku
        Efile.close();
    }

    return 0;
}
