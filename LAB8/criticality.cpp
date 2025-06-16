#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include <string>
#include <iomanip>
#include <sstream>
#include <fstream>

using namespace std;
const int MAX_GRAINS = 10000;

double rnd()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<double> dist(0.0, 1.0); // Poprawiony typ

    return dist(gen);
}

// Generuj losową siatkę zajętości
vector<vector<int>> generate_grid(int L, int max_h)
{
    vector<vector<int>> grid(L, vector<int>(L));
    for (int x = 0; x < L; x++)
        for (int y = 0; y < L; y++)
            grid[x][y] = (int)(rnd() * max_h);

    return grid;
}
// Główna funkcja do zadania 1
tuple<vector<vector<vector<int>>>, vector<int>, vector<int>> simulate(int N, int max_h, bool till_first_big_avalanche = false)
{
    vector<int> avalanche_size(MAX_GRAINS + 1, 0);
    vector<vector<vector<int>>> grid_evo;
    vector<int> grain_index_for_frame;
    auto grid = generate_grid(N, max_h);
    int x = N / 2;
    int y = N / 2;
    int i = 1;
    grid_evo.push_back(grid);
    grain_index_for_frame.push_back(i);
    while ((till_first_big_avalanche && avalanche_size[i - 1] < 8) ||
           (!till_first_big_avalanche && i < MAX_GRAINS))
    {
        if (!till_first_big_avalanche)
        {
            x = (int)(rnd() * N);
            y = (int)(rnd() * N);
        };

        int h = grid[x][y];
        grid[x][y] = h + 1;
        grid_evo.push_back(grid);
        grain_index_for_frame.push_back(i);
        auto new_grid = grid;
        bool relaxed = false;
        while (!relaxed)
        {
            relaxed = true;
            for (int y = 0; y < N; ++y)
            {
                for (int x = 0; x < N; ++x)
                {
                    if (grid[x][y] >= max_h)
                    {
                        int distrib = (int)(max_h / 4);
                        new_grid[x][y] -= 4 * distrib;
                        if (x > 0)
                            new_grid[x - 1][y] += distrib;
                        if (x < N - 1)
                            new_grid[x + 1][y] += distrib;
                        if (y > 0)
                            new_grid[x][y - 1] += distrib;
                        if (y < N - 1)
                            new_grid[x][y + 1] += distrib;
                        relaxed = false;
                        avalanche_size[i] += 4 * distrib;
                    }
                }
            }
            if (!relaxed)
            {
                grid_evo.push_back(new_grid);
                grain_index_for_frame.push_back(i);
            }
            grid = new_grid;
        }

        i++;
    }

    return {grid_evo, avalanche_size, grain_index_for_frame};
}

void save_avalanche_sizes(const vector<int> &avalanche_size, const string &filename)
{
    ofstream file(filename);
    file << "index" << "," << "avalanche_size" << endl;
    for (size_t i = 0; i < avalanche_size.size(); ++i)
    {
        if (avalanche_size[i] > 0)
            file << i << "," << avalanche_size[i] << "\n";
    }
    file.close();
}

void save_grid_evolution_to_single_file(const vector<vector<vector<int>>> &grid_evo, const vector<int> &timeframe, const string &filename, bool show_all = true)
{
    int iterations = 1000;
    if (show_all)
    {
        iterations = grid_evo.size();
    }
    ofstream file(filename);
    if (!file.is_open())
        return;

    for (size_t t = 0; t < iterations; ++t)
    {
        file << "# t = " << timeframe[t] << "\n";
        for (const auto &row : grid_evo[t])
        {
            for (size_t i = 0; i < row.size(); ++i)
            {
                file << row[i];
                if (i < row.size() - 1)
                    file << ",";
            }
            file << "\n";
        }
        file << "\n"; // pusty wiersz między klatkami
    }
    file.close();
}

int main()
{
    int N1 = 11;
    int N2 = 20;
    int max_h = 4;
    auto [grid_evo1, avalanche_size1, timeframes1] = simulate(N1, max_h, true);
    save_grid_evolution_to_single_file(grid_evo1, timeframes1, "data/output_N10_grid_all.csv");
    save_avalanche_sizes(avalanche_size1, "data/output_N10_avalanche.csv");

    auto [grid_evo2, avalanche_size2, timeframes2] = simulate(N2, max_h, false);
    save_grid_evolution_to_single_file(grid_evo2, timeframes2, "data/output_N20_grid_all.csv", false);
    save_avalanche_sizes(avalanche_size2, "data/output_N20_avalanche.csv");
    return 0;
}
