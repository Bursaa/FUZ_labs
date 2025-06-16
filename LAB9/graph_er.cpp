#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <cmath>
#include <fstream>
#include <numeric>
#include <algorithm>
#include <limits>
#include <string>

using namespace std;

const int N = 100;
const int NUM_RUNS = 100;

struct GraphStats
{
    double avg_edges = 0;
    double avg_degree = 0;
    vector<double> avg_degree_distribution;
    double avg_distance = 0;
    double avg_triangles = 0;
};

double rnd()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<double> dist(0.0, 1.0); // Poprawiony typ

    return dist(gen);
}
// Funkcja pomocnicza do tworzenia grafu ER
vector<vector<bool>> generate_er_graph(double p)
{
    vector<vector<bool>> adj(N, vector<bool>(N, false));

    for (int i = 0; i < N; ++i)
    {
        for (int j = i + 1; j < N; ++j)
        {
            double u = rnd();
            if (u < p)
            {
                adj[i][j] = true;
                adj[j][i] = true;
            }
        }
    }
    return adj;
}

int count_edges(const vector<vector<bool>> &adj)
{
    int edges = 0;
    for (int i = 0; i < N; ++i)
        for (int j = i + 1; j < N; ++j)
            if (adj[i][j])
                edges++;
    return edges;
}

vector<int> degree_distribution(const vector<vector<bool>> &adj)
{
    vector<int> degs(N, 0);
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            if (adj[i][j])
                degs[i]++;
    return degs;
}

vector<vector<int>> floyd_warshall(const vector<vector<bool>> &adj)
{
    const int INF = numeric_limits<int>::max() / 2;
    vector<vector<int>> dist(N, vector<int>(N, INF));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            dist[i][j] = (i == j ? 0 : (adj[i][j] ? 1 : INF));

    for (int k = 0; k < N; ++k)
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
    return dist;
}

int count_triangles(const vector<vector<bool>> &adj)
{
    int triangles = 0;
    for (int i = 0; i < N; ++i)
        for (int j = i + 1; j < N; ++j)
            for (int k = j + 1; k < N; ++k)
                if (adj[i][j] && adj[j][k] && adj[k][i])
                    triangles++;
    return triangles;
}

GraphStats run_simulation(double alpha)
{
    double p = alpha / N;
    GraphStats stats;
    vector<double> degree_dist_accum(N, 0.0);

    for (int run = 0; run < NUM_RUNS; ++run)
    {
        auto adj = generate_er_graph(p);
        int L = count_edges(adj);
        stats.avg_edges += L;
        stats.avg_degree += 2.0 * L / N;

        auto degs = degree_distribution(adj);
        for (int i = 0; i < N; ++i)
            degree_dist_accum[degs[i]]++;

        auto dist = floyd_warshall(adj);
        double total_dist = 0;
        int count = 0;
        for (int i = 0; i < N; ++i)
            for (int j = i + 1; j < N; ++j)
                if (dist[i][j] < 1e6)
                {
                    total_dist += dist[i][j];
                    count++;
                }
        if (count > 0)
            stats.avg_distance += total_dist / count;

        stats.avg_triangles += count_triangles(adj);
    }

    stats.avg_edges /= NUM_RUNS;
    stats.avg_degree /= NUM_RUNS;
    stats.avg_distance /= NUM_RUNS;
    stats.avg_triangles /= NUM_RUNS;
    for (int k = 0; k < N; ++k)
        stats.avg_degree_distribution.push_back(degree_dist_accum[k]);

    return stats;
}

void run_all(double alpha)
{
    double p = alpha / N;
    double expected_L = p * N * (N - 1) / 2.0;
    double expected_k = p * (N - 1);
    double expected_d = log(N) / log(expected_k);
    double expected_triangles = (N * (N - 1) * (N - 2) / 6.0) * pow(p, 3);

    GraphStats stats = run_simulation(alpha);

    cout << "===== Wyniki dla α = " << alpha << " =====" << endl;
    cout << "Średnia liczba wiązań (L): " << stats.avg_edges << " (oczekiwana: " << expected_L << ")\n";
    cout << "Średni stopień wierzchołka (k): " << stats.avg_degree << " (oczekiwany: " << expected_k << ")\n";
    cout << "Średnia odległość między wierzchołkami (d): " << stats.avg_distance << " (oczekiwana: " << expected_d << ")\n";
    cout << "Średnia liczba trójkątów: " << stats.avg_triangles << " (oczekiwana: " << expected_triangles << ")\n";

    string filename = "data/degree_dist_alpha_" + to_string((int)alpha) + ".txt";
    ofstream fout(filename);
    for (int k = 0; k < N; ++k)
    {
        double poisson = exp(-expected_k) * pow(expected_k, k) / tgamma(k + 1);
        fout << k << " " << stats.avg_degree_distribution[k] << " " << (N * NUM_RUNS) * poisson << "\n";
    }
    fout.close();
}

int main()
{
    run_all(4);
    run_all(10);
    return 0;
}
