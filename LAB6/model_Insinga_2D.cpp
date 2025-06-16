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
const int L = 32;
const double kB = 1;

int mod(int a, int b)
{
    return (a % b + b) % b;
}

// Obliczanie zmiany energii
double deltaE(int sum_sigmas, int node0)
{
    double E_prim = (-1) * (-node0) * sum_sigmas;
    double E = (-1) * node0 * sum_sigmas;
    return E_prim - E;
}

// Obliczanie magnetyzacji
double magnetization(vector<vector<int>> &nodes)
{
    double sum = 0;
    for (int i = 0; i < L; i++)
    {
        for (int j = 0; j < L; j++)
        {
            sum += nodes[i][j];
        }
    }
    return sum;
}

// Symulacja Monte Carlo
void MC_simulation(vector<vector<int>> &nodes, double T)
{
    for (int i = 0; i < L * L; i++)
    {
        int r1 = rand() % L;
        int r2 = rand() % L;
        int node_r = nodes[r1][r2];
        double dE = deltaE(
            nodes[mod(r1 - 1, L)][r2] +
                nodes[mod(r1 + 1, L)][r2] +
                nodes[r1][mod(r2 - 1, L)] +
                nodes[r1][mod(r2 + 1, L)],
            node_r);

        double p = min(1.0, exp(-dE / (T * kB)));
        double rand_num = (double)rand() / RAND_MAX;
        if (dE < 0)
        {
            nodes[r1][r2] = -nodes[r1][r2];
        }
        else if (rand_num < p)
        {
            nodes[r1][r2] = -nodes[r1][r2];
        }
    }
}

int main()
{
    vector<int> sigmas = {1, -1};
    vector<int> sigmas_sum = {+4, +2, 0, -2, -4};
    vector<double> Temp;
    double T_min = 0.25, T_max = 4.0, step = 0.25;

    // Generowanie wartości temperatury
    for (double T = T_min; T <= T_max; T += step)
    {
        Temp.push_back(T);
    }
    srand(time(nullptr)); // Inicjalizacja generatora liczb pseudolosowych

    vector<vector<int>> nodes(L, vector<int>(L));

    // Pętla po temperaturach i Monte Carlo
    for (double T : Temp)
    {
        cout << std::fixed << std::setprecision(2);
        cout << "T: " << T << endl;
        for (int sigma_i : sigmas)
        {
            for (int sum_sigma : sigmas_sum)
            {
                double value = exp(-deltaE(sum_sigma, sigma_i) / (T * kB));
                double out_val = (value >= 1.0 ? 1.0 : value);
                cout << std::scientific << std::setprecision(3) << std::setw(13) << out_val << " ";
            }
            cout << endl;
        }
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2) << T;
        string T_str = ss.str();
        // Otwieranie pliku do zapisu
        ofstream Efile(("data/magnetization_" + T_str + ".txt").c_str());

        // Inicjalizacja węzłów
        for (int i = 0; i < L; i++)
        {
            for (int j = 0; j < L; j++)
            {
                nodes[i][j] = 1;
            }
        }
        for (int i = 0; i < MC_number; i++)
        {
            Efile << magnetization(nodes) << endl;
            MC_simulation(nodes, T);
        };
        // Zamknięcie pliku
        Efile.close();

        // Zapis mapy wartości spinowych do pliku
        ofstream mapfile(("data/spin_map_" + T_str + ".txt").c_str());
        for (int i = 0; i < L; i++)
        {
            for (int j = 0; j < L; j++)
            {
                mapfile << nodes[i][j] << " ";
            }
            mapfile << endl;
        }
        mapfile.close();
    }

    return 0;
}
