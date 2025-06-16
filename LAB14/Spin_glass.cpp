#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <cmath>
#include <string>

using namespace std;

using Spins = vector<int>;
using Couplings = vector<double>;

double normal_rnd()
{
    static random_device rd;
    static mt19937 gen(rd());
    normal_distribution<double> dist(0.0, 1.0);
    return dist(gen);
}

Spins int_to_spins(int i, int N)
{
    Spins spins(N);
    for (int j = 0; j < N; ++j)
        spins[j] = (i & (1 << j)) ? 1 : -1;
    return spins;
}

double compute_energy(const Spins &spins, const Couplings &J)
{
    double E = 0.0;
    int N = spins.size();
    for (int i = 0; i < N; ++i)
        E += J[i] * spins[i] * spins[(i + 1) % N];
    return -E;
}

// -------------------- ZADANIE 1 --------------------
void zadanie1(int N, int J, const string &filename)
{
    ofstream file("data/" + filename);
    int total = 1 << N;
    for (int i = 0; i < total; ++i)
    {
        Spins spins = int_to_spins(i, N);
        int E = 0;
        for (int j = 0; j < N; ++j)
            E += spins[j] * spins[(j + 1) % N];
        E *= -J;

        for (int s : spins)
            file << s << " ";
        file << E << "\n";
    }
    file.close();
}

// -------------------- ZADANIE 2 --------------------
bool is_one_spin_stable(const Spins &spins, const Couplings &J)
{
    int N = spins.size();
    for (int i = 0; i < N; ++i)
    {
        double local_field = J[(i - 1 + N) % N] * spins[(i - 1 + N) % N] +
                             J[i] * spins[(i + 1) % N];
        if (spins[i] * local_field <= 0)
            return false;
    }
    return true;
}

int count_stable(int N, const Couplings &J, bool two_spin = false)
{
    int count = 0;
    int total = 1 << N;

    for (int i = 0; i < total; ++i)
    {
        Spins spins = int_to_spins(i, N);
        if (!is_one_spin_stable(spins, J))
            continue;

        if (two_spin)
        {
            double original_energy = compute_energy(spins, J);
            bool unstable = false;
            for (int a = 0; a < N && !unstable; ++a)
            {
                for (int b = a + 1; b < N; ++b)
                {
                    Spins flipped = spins;
                    flipped[a] *= -1;
                    flipped[b] *= -1;
                    if (compute_energy(flipped, J) < original_energy)
                    {
                        unstable = true;
                        break;
                    }
                }
            }
            if (unstable)
                continue;
        }

        ++count;
    }

    return count;
}

void uruchom_zadanie(int N, int repeats, const string &filename, bool two_spin)
{
    ofstream file("data/" + filename);
    double total = 0;

    for (int i = 0; i < repeats; ++i)
    {
        Couplings J(N);
        for (double &j : J)
            j = normal_rnd();

        int result = count_stable(N, J, two_spin);
        total += result;
        file << result << "\n";
    }

    double avg = total / repeats;
    double expected = pow(2, N / (two_spin ? 5.0 : 3.0));

    file << "Średnia: " << avg << "\n";
    file << "Oczekiwana: " << expected << "\n";
    file.close();

    cout << "Zadanie " << (two_spin ? 3 : 2) << " dla N=" << N << endl;
    cout << "Srednia liczba " << (two_spin ? "dwuspinowo" : "jednospinowo")
         << " stabilnych konfiguracji: " << avg << endl;
    cout << "Oczekiwana liczba: " << expected << "\n\n";
}

// -------------------- MAIN --------------------
int main()
{
    // Zadanie 1
    zadanie1(5, 1, "zad1_ferro_N5.txt");
    zadanie1(6, 1, "zad1_ferro_N6.txt");
    zadanie1(5, -1, "zad1_antiferro_N5.txt");
    zadanie1(6, -1, "zad1_antiferro_N6.txt");

    // Zadanie 2
    uruchom_zadanie(9, 200, "zad2_stabilne_N9.txt", false);
    uruchom_zadanie(12, 200, "zad2_stabilne_N12.txt", false);
    uruchom_zadanie(15, 200, "zad2_stabilne_N15.txt", false);

    // Zadanie 3
    uruchom_zadanie(9, 200, "zad3_dwu_stabilne_N9.txt", true);
    uruchom_zadanie(12, 200, "zad3_dwu_stabilne_N12.txt", true);
    uruchom_zadanie(15, 200, "zad3_dwu_stabilne_N15.txt", true);

    cout << "Obliczenia zakonczone. Sprawdz pliki tekstowe." << endl;
    return 0;
}
