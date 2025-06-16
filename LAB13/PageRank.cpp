#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
using namespace std;

const double EPS = 1e-6;
using Matrix = vector<vector<double>>;
using Vector = vector<double>;

void save_result(const Vector &v, const string &filename)
{
    ofstream file("data/" + filename);
    for (double val : v)
    {
        file << fixed << setprecision(6) << val << "\n";
    }
    file.close();
}

void save_iteration(const vector<Vector> &iterations, const string &filename)
{
    ofstream file("data/" + filename);
    for (const auto &step : iterations)
    {
        for (size_t i = 0; i < step.size(); ++i)
        {
            file << fixed << setprecision(6) << step[i];
            if (i + 1 < step.size())
                file << ",";
        }
        file << "\n";
    }
    file.close();
}

void save_graph_edges(const Matrix &adj, const string &filename)
{
    ofstream file("data/" + filename);
    for (size_t i = 0; i < adj.size(); ++i)
    {
        for (size_t j = 0; j < adj[i].size(); ++j)
        {
            if (adj[i][j] > 0)
                file << i << "," << j << "\n";
        }
    }
    file.close();
}

double diff_sum(const Vector &v1, const Vector &v2)
{
    double sum = 0.0;
    for (size_t i = 0; i < v1.size(); ++i)
        sum += abs(v1[i] - v2[i]);
    return sum;
}

Matrix normalize_adj_matrix(const Matrix &adj)
{
    int N = adj.size();
    Matrix A(N, Vector(N, 0.0));
    for (int i = 0; i < N; ++i)
    {
        double degree = 0;
        for (int j = 0; j < N; ++j)
            degree += adj[i][j];
        if (degree > 0)
        {
            for (int j = 0; j < N; ++j)
                A[i][j] = adj[i][j] / degree;
        }
    }
    return A;
}

void random_walk(const Matrix &adj)
{
    int N = adj.size();
    Vector prob(N, 1.0 / N), next(N, 0.0);
    Matrix A = normalize_adj_matrix(adj);
    vector<Vector> history;
    history.push_back(prob);

    while (true)
    {
        for (int i = 0; i < N; ++i)
        {
            next[i] = 0.0;
            for (int j = 0; j < N; ++j)
                next[i] += prob[j] * A[j][i];
        }
        if (diff_sum(prob, next) < EPS)
            break;
        prob = next;
        history.push_back(prob);
    }

    save_result(prob, "output_zad1.txt");
    save_iteration(history, "iter_zad1.csv");
    save_graph_edges(adj, "graph_edges_zad1.csv");
}

void random_walk_with_teleport(const Matrix &adj, double teleport_prob = 0.15)
{
    int N = adj.size();
    Vector prob(N, 1.0 / N), next(N, 0.0);
    Matrix A(N, Vector(N, 0.0));
    vector<Vector> history;
    history.push_back(prob);

    for (int i = 0; i < N; ++i)
    {
        double degree = 0;
        for (int j = 0; j < N; ++j)
            degree += adj[i][j];
        if (degree > 0)
        {
            for (int j = 0; j < N; ++j)
                A[i][j] = (1 - teleport_prob) * adj[i][j] / degree + teleport_prob / N;
        }
        else
        {
            for (int j = 0; j < N; ++j)
                A[i][j] = 1.0 / N;
        }
    }

    while (true)
    {
        for (int i = 0; i < N; ++i)
        {
            next[i] = 0.0;
            for (int j = 0; j < N; ++j)
                next[i] += prob[j] * A[j][i];
        }
        if (diff_sum(prob, next) < EPS)
            break;
        prob = next;
        history.push_back(prob);
    }

    save_result(prob, "output_zad2.txt");
    save_iteration(history, "iter_zad2.csv");
    save_graph_edges(adj, "graph_edges_zad2.csv");
}

void pagerank_no_teleport(const Matrix &adj)
{
    int N = adj.size();
    Vector v(N, 1.0 / N), next(N, 0.0);
    Matrix A = normalize_adj_matrix(adj);
    vector<Vector> history;
    history.push_back(v);

    while (true)
    {
        for (int i = 0; i < N; ++i)
        {
            next[i] = 0.0;
            for (int j = 0; j < N; ++j)
                next[i] += A[j][i] * v[j];
        }
        if (diff_sum(v, next) < N * EPS)
            break;
        v = next;
        history.push_back(v);
    }

    save_result(v, "output_zad3.txt");
    save_iteration(history, "iter_zad3.csv");
    save_graph_edges(adj, "graph_edges_zad3.csv");
}

void pagerank_with_teleport(const Matrix &adj, double p = 0.15)
{
    int N = adj.size();
    Vector v(N, 1.0 / N), next(N, 0.0);
    Matrix A = normalize_adj_matrix(adj);
    Matrix M(N, Vector(N, 0.0));
    vector<Vector> history;
    history.push_back(v);

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            M[i][j] = (1 - p) * A[i][j] + p / N;

    while (true)
    {
        for (int i = 0; i < N; ++i)
        {
            next[i] = 0.0;
            for (int j = 0; j < N; ++j)
                next[i] += M[j][i] * v[j];
        }
        if (diff_sum(v, next) < N * EPS)
            break;
        v = next;
        history.push_back(v);
    }

    save_result(v, "output_zad4.txt");
    save_iteration(history, "iter_zad4.csv");
    save_graph_edges(adj, "graph_edges_zad4.csv");
}

int main()
{
    // Graf testowy1
    Matrix G = {
        {0, 1, 1, 1},
        {1, 0, 0, 0},
        {1, 1, 0, 0},
        {0, 1, 1, 0}};

    random_walk(G);
    random_walk_with_teleport(G);
    pagerank_no_teleport(G);
    pagerank_with_teleport(G);

    std::cout << "Wyniki zapisane do plików output_zad*.txt" << std::endl;
    return 0;
}
