#include <iostream>
#include <chrono>
#include <vector>
#include <random>

using namespace std;

class NQueensSolver {
public:
    explicit NQueensSolver(int size) : n(size), board(size), row_conflicts(size, 0),
        diagonal_conflicts(2 * size - 1, 0), anti_diagonal_conflicts(2 * size - 1, 0) {}

    vector<int> solve() {
        if (n == 2 || n == 3) {
            return vector<int>(1, -1); 
        }

        while (true) {
            initialize_board();

            for (int attempt = 0; attempt < n * 2; ++attempt) {
                int most_conflicted = select_most_conflicted_queen();
                if (most_conflicted == -1) {
                    return board;
                }
                
                int best_position = find_optimal_position(most_conflicted);
                update_queen_position(most_conflicted, best_position);
            }
        }
    }

private:
    int n;
    vector<int> board, row_conflicts, diagonal_conflicts, anti_diagonal_conflicts;

    void initialize_board() {
        fill(row_conflicts.begin(), row_conflicts.end(), 0);
        fill(diagonal_conflicts.begin(), diagonal_conflicts.end(), 0);
        fill(anti_diagonal_conflicts.begin(), anti_diagonal_conflicts.end(), 0);

        for (int x = 0; x < n; ++x) {
            int y = find_optimal_position(x);
            place_queen(x, y);
        }
    }

    int calculate_conflicts(int x, int y) const {
        return row_conflicts[y] + diagonal_conflicts[y - x + n - 1] + anti_diagonal_conflicts[y + x];
    }

    int select_most_conflicted_queen() {
        int max_conflicts = 0;
        vector<int> candidates;

        for (int x = 0; x < n; ++x) {
            int conflicts = calculate_conflicts(x, board[x]) - 3; // `- 3` is from the queen
            if (conflicts > max_conflicts) {
                max_conflicts = conflicts;
                candidates = {x};
            } else if (conflicts == max_conflicts) {
                candidates.push_back(x);
            }
        }
        return (max_conflicts == 0) ? -1 : candidates[random_index(candidates.size())];
    }

    int find_optimal_position(int x) {
        int min_conflicts = n;
        vector<int> best_positions;

        for (int y = 0; y < n; ++y) {
            int conflicts = calculate_conflicts(x, y);

            if (conflicts < min_conflicts) {
                min_conflicts = conflicts;
                best_positions.clear();
                best_positions.push_back(y);
            } else if (conflicts == min_conflicts) {
                best_positions.push_back(y);
            }
        }

        return best_positions[random_index(best_positions.size())];
    }

    void place_queen(int x, int y) {
        board[x] = y;

        ++row_conflicts[y];
        ++diagonal_conflicts[y - x + n - 1];
        ++anti_diagonal_conflicts[y + x];
    }

    void update_queen_position(int x, int new_y) {
        int old_y = board[x];
        if (new_y == old_y) {
            return;
        }

        --row_conflicts[old_y];
        --diagonal_conflicts[old_y - x + n - 1];
        --anti_diagonal_conflicts[old_y + x];

        place_queen(x, new_y);
    }

    int random_index(int limit) const {
        static mt19937 rng(random_device{}());
        uniform_int_distribution<int> dist(0, limit - 1);
        return dist(rng);
    }
};

int main() {
    int n;
    cin >> n;

    auto start = chrono::high_resolution_clock::now();

    NQueensSolver solver(n);
    vector<int> solution = solver.solve();
    
    auto end = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    if (solution.size() == 1 && solution[0] == -1) {
        cout << -1 << endl;
    } else if (n > 100) {
        cout << duration.count() / 1000 << "." << (duration.count() % 1000) / 10 << endl;
    } else {
        cout << "[" << solution[0];
        for (int i = 1; i < solution.size(); ++i) {
            cout << ", " << solution[i];
        }
        cout << "]" << endl;
    }

    return 0;
}
