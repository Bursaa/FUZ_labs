#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <random>
#include <complex>
#include <string>
#include <iomanip>
#include <sstream>

using namespace std;

constexpr double PI = 3.14159265358979323846;

// Parametry symulacji
double dt = 0.05;
int steps = 2000;

double rnd_uniform()
{
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_real_distribution<double> dist(0.0, 1.0); // Poprawiony typ

    return dist(gen);
}
double rnd_normal(double mean, double stddev)
{
    static random_device rd;
    static mt19937 gen(rd());
    static normal_distribution<double> dist(mean, stddev); // Poprawiony typ

    return dist(gen);
}
vector<double> generate_normal_distribution(int N, double mean, double stddev)
{
    std::vector<double> result(N);
    for (int i = 0; i < N; ++i)
        result[i] = rnd_normal(mean, stddev);
    return result;
}

vector<double> generate_uniform_distribution(int N, double max_theta)
{
    vector<double> result(N);
    for (int i = 0; i < N; ++i)
        result[i] = rnd_uniform() * max_theta;
    return result;
}
double wrap_angle(double theta)
{
    return fmod(theta + 2 * PI, 2 * PI);
}

complex<double> order_parameter(const vector<double> &theta)
{
    complex<double> sum(0.0, 0.0);
    for (double th : theta)
    {
        sum += polar(1.0, th);
    }
    return sum / static_cast<double>(theta.size());
}

void simulate_kuramoto(int N, double K, const string &filename, double max_theta)
{
    const int repetitions = 10;
    vector<double> r_rep(steps, 0.0);
    ofstream file(filename);
    for (int rep = 0; rep < repetitions; ++rep)
    {
        vector<double> omega = generate_normal_distribution(N, 0.0, 1.0);
        vector<double> theta = generate_uniform_distribution(N, max_theta);
        for (int step = 0; step < steps; ++step)
        {
            double t = step * dt;
            vector<double> dtheta(N, 0.0);
            for (int i = 0; i < N; ++i)
            {
                double coupling_sum = 0.0;
                for (int j = 0; j < N; ++j)
                {
                    coupling_sum += sin(theta[j] - theta[i]);
                }
                dtheta[i] = omega[i] + (K / N) * coupling_sum;
            }

            for (int i = 0; i < N; ++i)
            {
                theta[i] += dt * dtheta[i];
                theta[i] = wrap_angle(theta[i]);
            }

            complex<double> r_complex = order_parameter(theta);
            r_rep[step] += abs(r_complex);

            if (rep = repetitions - 1)
            {
                file << t << " " << r_rep[step] / repetitions;
                for (int i = 0; i < N; ++i)
                    file << " " << theta[i];
                file << "\n";
            }
        }
        file.close();
        cout << "Simulation finished: " << filename << endl;
    }
}

void run_zad1_2()
{
    vector<int> Ns = {10, 20, 50};
    vector<double> Ks;
    for (double k = 0.25; k <= 5.0; k += 0.25)
        Ks.push_back(k);

    for (int N : Ns)
    {
        for (double K : Ks)
        {
            ostringstream oss;
            oss << fixed << setprecision(2) << K;
            string filename = "data/kuramoto_N=" + to_string(N) + "_K=" + oss.str() + ".txt";
            simulate_kuramoto(N, K, filename, 2 * PI);
        }
    }
}

void run_zad3()
{
    vector<double> Ks = {0.01, 0.8, 2.0};
    int N = 25;
    for (double K : Ks)
    {
        ostringstream oss;
        oss << fixed << setprecision(2) << K;
        string filename = "data/kuramoto_N=" + to_string(N) + "_K=" + oss.str() + "_max_theta=2pi.txt";
        simulate_kuramoto(N, K, filename, 2 * PI);

        string filename2 = "data/kuramoto_N=" + to_string(N) + "_K=" + oss.str() + "_max_theta=pi_12.txt";
        simulate_kuramoto(N, K, filename2, PI / 12);
    }
}

int main()
{
    run_zad1_2();
    run_zad3();

    return 0;
}