//teoriq

#include <iostream>
#include <vector>
#include <stack>
#include <cmath>
#include <chrono>
#include <iomanip>

using namespace std;

bool print_timing = true; 

struct Puzzle {
    vector<vector<int>> board;
    int N;
    int zero_row, zero_col;
    int goal_zero_row, goal_zero_col;

    Puzzle(vector<vector<int>> b, int zr, int zc, int gzr, int gzc) 
        : board(b), zero_row(zr), zero_col(zc), goal_zero_row(gzr), goal_zero_col(gzc) {
        N = b.size();
    }

    bool is_goal() const {
        int num = 1;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (i == goal_zero_row && j == goal_zero_col) {
                    if (board[i][j] != 0) {
                        return false;
                    }
                } else if (board[i][j] != num++) {
                    return false;
                }
            }
        }
        return true;
    }

    int heuristic_cost() const {
        int dist = 0;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (board[i][j] != 0) {
                    int goal_row = (board[i][j] - 1) / N;
                    int goal_col = (board[i][j] - 1) % N;
                    dist += abs(i - goal_row) + abs(j - goal_col); // manhatan
                }
            }
        }
        return dist;
    }

    bool is_solvable() const {
        vector<int> flat_board;
        int inversions = 0;

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (board[i][j] != 0) {
                    flat_board.push_back(board[i][j]);
                }
            }
        }

        for (size_t i = 0; i < flat_board.size(); ++i) {
            for (size_t j = i + 1; j < flat_board.size(); ++j) {
                if (flat_board[i] > flat_board[j]) {
                    ++inversions;
                }
            }
        }

        if (N % 2 == 1) {
            return inversions % 2 == 0;
        }
        else {
            return inversions % 2 == abs(goal_zero_row - zero_row) % 2;
        }
    }
};

struct Node {
    Puzzle puzzle;
    int cost_from_start;
    int estimated_cost_to_goal;
    string move_sequence; // it would be better to preserve ints here, and convert it to string in the end

    Node(Puzzle p, int cfs, int ectg, string ms) 
        : puzzle(p), cost_from_start(cfs), estimated_cost_to_goal(ectg), move_sequence(ms) {}

    int total_estimated_cost() const { 
        return cost_from_start + estimated_cost_to_goal; 
    }
};


const int dx[] = {0, 0, -1, 1};
const int dy[] = {-1, 1, 0, 0};
const string moves[] = {"right", "left", "down", "up"};

bool IDA_star(Puzzle &start, int &solution_length, string &solution_path) {
    if (!start.is_solvable()) {
        return false;
    }

    int threshold = start.heuristic_cost();

    while (true) {
        int min_threshold = INT_MAX;
        stack<Node> st;
        st.push(Node(start, 0, start.heuristic_cost(), ""));

        while (!st.empty()) {
            Node node = st.top(); 
            st.pop();

            if (node.puzzle.is_goal()) {
                solution_length = node.cost_from_start;
                solution_path = node.move_sequence;

                return true;
            }

            for (int i = 0; i < 4; ++i) {
                int nx = node.puzzle.zero_row + dx[i];
                int ny = node.puzzle.zero_col + dy[i];

                if (nx >= 0 && nx < node.puzzle.N && ny >= 0 && ny < node.puzzle.N) {
                    Puzzle next_puzzle = node.puzzle;
                    swap(next_puzzle.board[node.puzzle.zero_row][node.puzzle.zero_col],
                         next_puzzle.board[nx][ny]);
                    next_puzzle.zero_row = nx;
                    next_puzzle.zero_col = ny;

                    int heuristic_cost = next_puzzle.heuristic_cost();
                    int cost = node.cost_from_start + 1 + heuristic_cost;

                    if (cost > threshold) {
                        min_threshold = min(min_threshold, cost);
                        continue;
                    }

                    st.push(Node(next_puzzle, node.cost_from_start + 1, heuristic_cost, node.move_sequence + moves[i] + "\n"));
                }
            }
        }
        if (min_threshold == INT_MAX) {
            return false;
        }

        threshold = min_threshold;
    }
}

int main() {
    int num_tiles, zero_index;
    cin >> num_tiles >> zero_index;

    int N = sqrt(num_tiles + 1);
    vector<vector<int>> board(N, vector<int>(N));
    int zero_row, zero_col;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cin >> board[i][j];
            if (board[i][j] == 0) {
                zero_row = i;
                zero_col = j;
            }
        }
    }

    int goal_zero_row, goal_zero_col;
    if (zero_index == -1) {
        goal_zero_row = N - 1;
        goal_zero_col = N - 1;
    } else {
        goal_zero_row = zero_index / N;
        goal_zero_col = zero_index % N;
    }

    Puzzle start(board, zero_row, zero_col, goal_zero_row, goal_zero_col);
    int solution_length;
    string solution_path;

    auto start_time = chrono::high_resolution_clock::now();

    bool found_solution = IDA_star(start, solution_length, solution_path);

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed_seconds = end_time - start_time;
    if (print_timing) {
        cerr << "Time taken: " << fixed << setprecision(2) << elapsed_seconds.count() << " seconds" << endl;
    }

    if (found_solution) {
        cout << solution_length << endl;
        cout << solution_path;
    } else {
        cout << "-1" << endl;
    }

    return 0;
}
