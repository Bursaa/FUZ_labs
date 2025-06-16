#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <map>

using namespace std;

const int N = 25; // 5x5 wzory

using Pattern = vector<int>;
using PatternMap = map<char, Pattern>;
using Matrix = vector<vector<int>>;

PatternMap load_pattern_map(const string &filename)
{
    ifstream file("data/" + filename);
    PatternMap patterns;
    string line;
    while (getline(file, line))
    {
        if (line.empty())
            continue;

        char label = line[0];
        Pattern pattern;

        for (int i = 0; i < 5; ++i)
        {
            getline(file, line);
            for (char ch : line)
            {
                if (ch == '1')
                    pattern.push_back(1);
                else if (ch == '0')
                    pattern.push_back(-1);
            }
        }

        if (pattern.size() == N)
        {
            patterns[label] = pattern;
        }
    }

    return patterns;
}

Matrix train(const vector<Pattern> &patterns)
{
    Matrix W(N, vector<int>(N, 0));
    for (const auto &p : patterns)
    {
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                if (i != j)
                    W[i][j] += p[i] * p[j];
            }
        }
    }
    return W;
}

Pattern run_network(const Matrix &W, Pattern s, Pattern train_pattern, const string &filename)
{
    ofstream file(filename); // dopisujemy dane

    for (int val : train_pattern)
    {
        file << val << " ";
    }
    file << "\n";

    Pattern prev;
    do
    {
        prev = s;
        for (int val : s)
        {
            file << val << " ";
        }
        file << "\n";

        for (int i = 0; i < N; ++i)
        {
            int sum = 0;
            for (int j = 0; j < N; ++j)
            {
                sum += W[i][j] * s[j];
            }
            s[i] = (sum >= 0) ? 1 : -1;
        }
    } while (s != prev);

    return s;
}

void print_pattern(const Pattern &p)
{
    for (int i = 0; i < N; ++i)
    {
        cout << (p[i] == 1 ? "1 " : "0 ");
        if ((i + 1) % 5 == 0)
            cout << endl;
    }
    cout << endl;
}

int main()
{
    // Wybierz litery do treningu i testów

    // Wczytaj dane
    PatternMap train_map = load_pattern_map("train.txt");
    PatternMap test_map = load_pattern_map("test.txt");

    vector<char> train_labels1 = {'T', 'H', 'A'};
    vector<char> train_labels2 = {'T', 'H', 'A', 'E'};
    vector<char> test_labels1 = {'T', 'H', 'A'};
    vector<char> test_labels2 = {'T', 'H', 'A', 'E'};
    // Zbuduj listy patternów
    vector<Pattern> training1;
    for (char c : train_labels1)
        training1.push_back(train_map[c]);

    vector<Pattern> training2;
    for (char c : train_labels2)
        training2.push_back(train_map[c]);

    vector<Pattern> test1;
    for (char c : test_labels1)
        test1.push_back(test_map[c]);

    vector<Pattern> test2;
    for (char c : test_labels2)
        test2.push_back(test_map[c]);

    // Trenuj sieć
    Matrix W1 = train(training1);
    Matrix W2 = train(training2);
    for (int i = 0; i < test1.size(); ++i)
    {
        Pattern result1 = run_network(
            W1,
            test1[i],
            training1[i],
            "data/iter_hopfield_test1_" + string(1, test_labels1[i]) + ".txt");
    }

    for (int i = 0; i < test2.size(); ++i)
    {
        Pattern result2 = run_network(
            W2,
            test2[i],
            training2[i],
            "data/iter_hopfield_test2_" + string(1, test_labels2[i]) + ".txt");
    }
    return 0;
}
