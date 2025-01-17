#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;

string initial_state, target_state;

pair<bool, int> is_valid_move(const string& state, int index, char direction) {
    if (direction == '>') {
        if (index + 1 < state.size() && state[index + 1] == '_') {
            return {true, index + 1};
        }
        if (index + 2 < state.size() && state[index + 2] == '_' && state[index + 1] == '<') {
            return {true, index + 2};
        }
    } else if (direction == '<') {
        if (index - 1 >= 0 && state[index - 1] == '_') {
            return {true, index - 1};
        }
        if (index - 2 >= 0 && state[index - 2] == '_' && state[index - 1] == '>') {
            return {true, index - 2};
        }
    }
    return {false, -1};
}

bool dfs(string& state, vector<string>& path, unordered_set<string>& visited) {
    if (visited.find(state) != visited.end()) {
        return false;
    }

    visited.insert(state);
    path.push_back(state);

    if (state == target_state) {
        return true;
    }

    for (size_t i = 0; i < state.size(); ++i) {
        if (state[i] == '>' || state[i] == '<') {
            auto [valid, new_pos] = is_valid_move(state, i, state[i]);
            if (valid) {
                swap(state[i], state[new_pos]);
                if (dfs(state, path, visited)) {
                    return true;
                }
                swap(state[i], state[new_pos]); 
            }
        }
    }

    path.pop_back();
    return false;
}

void solve_frog_puzzle(int n) {
    initial_state.insert(initial_state.end(), n, '>');
    initial_state.push_back('_');
    initial_state.insert(initial_state.end(), n, '<');

    target_state.insert(target_state.end(), n, '<');
    target_state.push_back('_');
    target_state.insert(target_state.end(), n, '>');

    vector<string> path;
    unordered_set<string> visited;
    dfs(initial_state, path, visited);

    for (const auto& step : path) {
        cout << step << '\n';
    }
}

int main() {
    int N;
    cin >> N;

    solve_frog_puzzle(N);
    return 0;
}
