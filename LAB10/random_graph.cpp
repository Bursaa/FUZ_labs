#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <fstream>
#include <unordered_map>
#include <algorithm>
#include <set>
#include <string>

using namespace std;

const int N1 = 100;
const int N2 = pow(10, 6);
const int N3 = 100;
const int M0 = 5;
const int M = 4;

double rnd()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<double> dist(0.0, 1.0); // Poprawiony typ

    return dist(gen);
}

void save_graph(const vector<vector<int>> &adj, const string &filename)
{
    ofstream fout(filename);
    for (int i = 0; i < adj.size(); ++i)
    {
        fout << i << ": ";
        for (int neighbor : adj[i])
        {
            fout << neighbor << " ";
        }
        fout << "\n";
    }
    fout.close();
}

vector<vector<int>> generate_ba_graph(int n, int m0, int m, string save_evolution = "_")
{
    if (n <= 0 || m0 < 1 || m < 1 || m0 > n || m > n - 1)
        throw invalid_argument("Nieprawidłowe parametry: wymagane n > 0, m0 ≥ 1, m ≥ 1, m0 ≤ n, m ≤ n-1");

    vector<vector<int>> adj(n);

    // Start with complete graph of size m0
    for (int i = 0; i < m0; ++i)
    {
        for (int j = i + 1; j < m0; ++j)
        {
            adj[i].push_back(j);
            adj[j].push_back(i);
        }
    }

    vector<int> nodes; // List of nodes with duplication by degree
    for (int i = 0; i < m0; ++i)
        for (int j = 0; j < (int)adj[i].size(); ++j)
            nodes.push_back(i);

    for (int i = m0; i < n; ++i)
    {
        if (save_evolution != "_")
        {

            save_graph(adj, "data/" + save_evolution + "_" + to_string(i) + ".txt");
        }
        set<int> targets;
        while ((int)targets.size() < m)
        {
            int candidate;

            if (nodes.empty())
            {
                // fallback – losuj z istniejących wierzchołków
                candidate = (int)(i * rnd());
            }
            else
            {
                int node_index = (int)(rnd() * nodes.size());
                candidate = nodes[node_index];
            }

            if (candidate != i)
                targets.insert(candidate);
        }

        for (int t : targets)
        {
            adj[i].push_back(t);
            adj[t].push_back(i);
            nodes.push_back(t);
            nodes.push_back(i);
        }
    }

    return adj;
}

void save_degree_distribution(const vector<vector<int>> &adj, const string &filename)
{
    unordered_map<int, int> degree_count;
    for (const auto &neighbors : adj)
    {
        int deg = neighbors.size();
        degree_count[deg]++;
    }

    ofstream fout(filename);
    for (const auto &[deg, count] : degree_count)
    {
        fout << deg << " " << count << "\n";
    }
    fout.close();
}

bool is_connected(const vector<vector<int>> &adj, int N)
{
    vector<bool> visited(N, false);
    vector<int> stack = {0};
    visited[0] = true;

    while (!stack.empty())
    {
        int u = stack.back();
        stack.pop_back();
        for (int v : adj[u])
        {
            if (!visited[v])
            {
                visited[v] = true;
                stack.push_back(v);
            }
        }
    }

    return all_of(visited.begin(), visited.end(), [](bool v)
                  { return v; });
}

void run_task_1()
{
    auto adj = generate_ba_graph(N1, 5, 4, "graph1");
    save_graph(adj, "data/graph1.txt");
    save_degree_distribution(adj, "data/graph1_degree_distribution.txt");
    cout << "Zapisano histogram stopni do pliku 'graph1_degree_distribution.txt'\n";
}

void run_task_2()
{
    auto adj = generate_ba_graph(N2, 5, 4);
    save_degree_distribution(adj, "data/graph2_degree_distribution.txt");
    cout << "Zapisano histogram stopni do pliku 'graph2_degree_distribution.txt'\n";
}

void run_task_3()
{
    auto adj = generate_ba_graph(N3, 1, 1, "graph3");
    save_graph(adj, "data/graph3.txt");
    save_degree_distribution(adj, "data/graph3_degree_distribution.txt");

    bool before = is_connected(adj, N3);
    // Remove a random edge
    vector<pair<int, int>> edges;
    for (int u = 0; u < N3; ++u)
        for (int v : adj[u])
            if (u < v)
                edges.emplace_back(u, v);

    if (!edges.empty())
    {
        int random_edge_index = (int)(rnd() * edges.size());
        auto [u, v] = edges[random_edge_index];

        // Remove edge u-v
        adj[u].erase(remove(adj[u].begin(), adj[u].end(), v), adj[u].end());
        adj[v].erase(remove(adj[v].begin(), adj[v].end(), u), adj[v].end());
    }
    bool after = is_connected(adj, N3);
    ofstream fout("data/graph_3_connectivity_check.txt");
    fout << "Czy graf był spójny przed usunięciem krawędzi? " << (before ? "TAK" : "NIE") << "\n";
    fout << "Czy graf jest spójny po usunięciu jednej losowej krawędzi? " << (after ? "TAK" : "NIE") << "\n";
    fout.close();

    cout << "Zapisano test spojnosci do pliku 'connectivity_check.txt'\n";
}

int main()
{
    run_task_1(); // generuje BA i zapisuje histogram i graph dla N=100
    run_task_2(); // generuje BA i zapisuje histogram dla N=10^6
    run_task_3(); // analizuje przypadek m0 = 1, m = 1
    return 0;
}
