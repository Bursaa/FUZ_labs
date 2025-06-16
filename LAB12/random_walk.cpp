#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <map>

using namespace std;

double rnd()
{
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_real_distribution<double> dist(0.0, 1.0); // Poprawiony typ

    return dist(gen);
}
void zadanie1()
{
    cout << "ZADANIE 1 - Losowy spacer w 1D" << endl;
    ofstream trajFile("data/zad1_d_vs_N.txt");
    ofstream histFile("data/zad1_histogram.txt");

    const int Nmax = 200;
    const int trials = 1e6;

    // 5 trajektorii
    for (int t = 0; t < 5; ++t)
    {
        int d = 0;
        trajFile << "#Trajektoria " << t + 1 << "\n";
        for (int n = 0; n <= Nmax; ++n)
        {
            trajFile << n << " " << d << "\n";
            d += rnd() > 0.5 ? 1 : -1;
        }
        trajFile << "\n";
    }

    // Histogram dla N=20
    map<int, int> histogram;
    for (int t = 0; t < trials; ++t)
    {
        int d = 0;
        for (int n = 0; n < 20; ++n)
        {
            d += rnd() > 0.5 ? 1 : -1;
        }
        histogram[d]++;
    }

    for (int d = -20; d <= 20; d += 2)
    {
        int count = histogram[d];
        double prob = count / static_cast<double>(trials);
        histFile << d << " " << prob << "\n";
    }
    trajFile.close();
    histFile.close();
}

void zadanie2()
{
    cout << "ZADANIE 2 - Losowy spacer w 2D" << endl;
    const int M = 10;
    const int N = 1e6;
    vector<vector<int>> visits(M, vector<int>(M, 0));

    int x = M / 2, y = M / 2;
    for (int n = 0; n < N; ++n)
    {
        visits[y][x]++;
        int d = rnd() * 4;
        if (d == 0)
            y = (y + 1) % M;
        else if (d == 1)
            y = (y - 1 + M) % M;
        else if (d == 2)
            x = (x - 1 + M) % M;
        else
            x = (x + 1) % M;
    }

    ofstream out("data/zad2_heatmap.txt");
    double mean = 0, var = 0;

    for (int i = 0; i < M; ++i)
        for (int j = 0; j < M; ++j)
            mean += visits[i][j];
    mean /= (M * M);

    for (int i = 0; i < M; ++i)
        for (int j = 0; j < M; ++j)
            var += pow(visits[i][j] - mean, 2);
    var /= (M * M);
    double stddev = sqrt(var);

    for (int i = 0; i < M; ++i)
        for (int j = 0; j < M; ++j)
            out << i << " " << j << " " << visits[i][j] << "\n";

    out << "# Srednia = " << mean << ", Odchylenie std = " << stddev << "\n";
    out.close();
}

void zadanie3()
{
    cout << "ZADANIE 3 - Losowy spacer w 2D bez siatki z zapisem trajektorii" << endl;
    ofstream out("data/zad3_distances.txt");
    ofstream out_walking("data/zad3_walking_pattern.txt");
    int N_max = 1000;
    int trials = 10 * N_max;
    vector<double> sum_d(N_max + 1, 0.0);

    for (int t = 0; t < trials; ++t)
    {
        double x = 0, y = 0;
        for (int i = 0; i <= N_max; ++i)
        {
            if (t == 0)
            {
                out_walking << i << " " << x << " " << y << "\n";
            }
            double theta = rnd() * 2 * M_PI;
            x += cos(theta);
            y += sin(theta);
            sum_d[i] += sqrt(x * x + y * y) / trials;
        }
    }
    for (int i = 0; i <= N_max; ++i)
    {
        double avg_d = sum_d[i];
        out << i << " " << avg_d << " " << 0.5 * sqrt(M_PI) * sqrt(i) << "\n";
    }
    out.close();
    out_walking.close();
}

void zadanie4()
{
    cout << "ZADANIE 4 - Losowy spacer w 2D bez siatki z zapisem trajektorii i losowym krokiem" << endl;
    ofstream out("data/zad4_distances.txt");
    ofstream out_walking("data/zad4_walking_pattern.txt");
    int N_max = 1000;
    int trials = 10 * N_max;
    double step_size_sr = 0.0;
    vector<double> sum_d(N_max + 1, 0.0);

    for (int t = 0; t < trials; ++t)
    {
        double x = 0, y = 0;
        for (int i = 0; i <= N_max; ++i)
        {
            if (t == 0)
            {
                out_walking << i << " " << x << " " << y << "\n";
            }
            double theta = rnd() * 2 * M_PI;
            double step_size = rnd();
            x += step_size * cos(theta);
            y += step_size * sin(theta);
            sum_d[i] += sqrt(x * x + y * y) / trials;
            step_size_sr += step_size;
        }
    }
    step_size_sr /= (N_max * trials);
    for (int i = 0; i <= N_max; ++i)
    {
        double avg_d = sum_d[i];
        out << i << " " << avg_d << " " << step_size_sr * 1 / 2 * sqrt(M_PI) * sqrt(i) << "\n";
    }
    out.close();
    out_walking.close();
}

int main()
{
    zadanie1();
    zadanie2();
    zadanie3();
    zadanie4();
    cout << "Symulacje zakonczone, wyniki zapisane w katalogu 'data/'\n";
    return 0;
}
