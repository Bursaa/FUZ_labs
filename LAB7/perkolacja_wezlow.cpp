#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <iomanip>
#include <sstream>
#include <fstream>

using namespace std;

const vector<int> L_arr = {16, 32, 64, 128, 256};
const int R = 1000;
const double p_min = 0.4;
const double p_max = 0.8;
const double p_step = 0.01;

// Struktura Union-Find do zarządzania etykietami
class UnionFind
{
public:
    vector<int> parent;

    UnionFind(int size)
    {
        parent.resize(size);
        for (int i = 0; i < size; ++i)
            parent[i] = i;
    }

    int find(int x)
    {
        if (x != parent[x])
            parent[x] = find(parent[x]);
        return parent[x];
    }

    void unite(int a, int b)
    {
        int root_a = find(a);
        int root_b = find(b);
        if (root_a != root_b)
            parent[root_b] = root_a;
    }
};

// Generuj losową siatkę zajętości
vector<vector<int>> generate_grid(double p, int L)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);

    vector<vector<int>> grid(L, vector<int>(L));
    for (int y = 0; y < L; ++y)
        for (int x = 0; x < L; ++x)
            grid[y][x] = dis(gen) < p ? 1 : 0;

    return grid;
}

// Algorytm Hoshena-Kopelmana
vector<vector<int>> hoshen_kopelman(const vector<vector<int>> &grid, int L)
{
    vector<vector<int>> labels(L, vector<int>(L, 0));
    UnionFind uf(L * L);
    int next_label = 1;

    for (int y = 0; y < L; ++y)
    {
        for (int x = 0; x < L; ++x)
        {
            if (grid[y][x] == 0)
                continue;

            int left = (x > 0) ? labels[y][x - 1] : 0;
            int up = (y > 0) ? labels[y - 1][x] : 0;

            if (left == 0 && up == 0)
            {
                labels[y][x] = next_label;
                next_label++;
            }
            else if (left != 0 && up == 0)
            {
                labels[y][x] = left;
            }
            else if (left == 0 && up != 0)
            {
                labels[y][x] = up;
            }
            else
            {
                labels[y][x] = min(left, up);
                uf.unite(left, up);
            }
        }
    }

    // Druga iteracja – przepisywanie etykiet przez Union-Find
    for (int y = 0; y < L; ++y)
        for (int x = 0; x < L; ++x)
            if (labels[y][x] != 0)
                labels[y][x] = uf.find(labels[y][x]);

    return labels;
}

// Sprawdź perkolację góra-dół
bool check_percolation(const vector<vector<int>> &labels, int L)
{
    unordered_set<int> top_labels, bottom_labels;

    for (int x = 0; x < L; ++x)
    {
        if (labels[0][x] != 0)
            top_labels.insert(labels[0][x]);
        if (labels[L - 1][x] != 0)
            bottom_labels.insert(labels[L - 1][x]);
    }

    for (int label : top_labels)
        if (bottom_labels.count(label))
            return true;

    return false;
}

// Wydruk siatki binarnej
void print_grid(const vector<vector<int>> &grid)
{
    for (const auto &row : grid)
    {
        for (int cell : row)
            cout << cell << " ";
        cout << "\n";
    }
}

// Wydruk siatki etykiet
void print_labels(const vector<vector<int>> &labels)
{
    for (const auto &row : labels)
    {
        for (int label : row)
            cout << setw(3) << label << " ";
        cout << "\n";
    }
}

// Główna funkcja do zadania 1
tuple<vector<vector<int>>, vector<vector<int>>, bool> zadanie1(double p, int L)
{
    auto grid = generate_grid(p, L);
    auto labels = hoshen_kopelman(grid, L);
    bool perkoluje = check_percolation(labels, L);

    return {grid, labels, perkoluje};
}

int main()
{
    vector<double> p_arr;
    for (double p = p_min; p < p_max + p_step; p += p_step)
    {
        p_arr.push_back(p);
    }

    ofstream percolation_file(("data/percolation.txt"));
    for (auto L : L_arr)
    {
        ofstream W_file(("data/W_L_" + to_string(L) + ".txt").c_str());
        for (auto p : p_arr)
        {
            int N_p = 0;
            for (int i = 0; i < R; i++)
            {
                auto [grid, labels, perlokuje] = zadanie1(p, L);
                N_p += (int)perlokuje;
                if (i == 0)
                {
                    percolation_file << perlokuje << " ";
                    std::ostringstream ss;
                    ss << std::fixed << std::setprecision(2) << p;
                    string p_str = ss.str();
                    // Otwieranie pliku do zapisu
                    ofstream grid_file(("data/grid_p_" + p_str + "L_" + to_string(L) + ".txt").c_str());
                    ofstream labels_file(("data/labels_p_" + p_str + "L_" + to_string(L) + ".txt").c_str());

                    for (int y = 0; y < L; ++y)
                    {
                        for (int x = 0; x < L; ++x)
                        {
                            labels_file << labels[y][x] << " ";
                            grid_file << grid[y][x] << " ";
                        }
                        labels_file << endl;
                        grid_file << endl;
                    }
                    grid_file.close();
                    labels_file.close();
                }
            }
            double W = ((double)N_p) / ((double)R);
            W_file << std::fixed << std::setprecision(4) << W << " ";
        }
        W_file.close();
        percolation_file << endl;
    }
    percolation_file.close();
    return 0;
}
