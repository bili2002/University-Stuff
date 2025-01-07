#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

using namespace std;

class TicTacToe {
public:
    TicTacToe() : board(3, vector<int>(3, 0)) {
    }

    void play() {
        cout << "You are X, AI is O\n";
        printBoard();

        while (true) {
            playerMove();
            if (isGameOver(PLAYER)) {
                break;
            }

            aiMove();
            if (isGameOver(AI)) {
                break;
            }
        }
    }

private:
    const int INF = 1e6;
    const int PLAYER = 1000;
    const int AI = -1000; // ...
    vector<vector<int>> board;

    void printBoard() const {
        for (const auto& row : board) {
            for (const auto& cell : row) {
                if (cell == PLAYER) {
                    cout << "X ";
                } else if (cell == AI) {
                    cout << "O ";
                } else {
                    cout << ". ";
                }
            }
            cout << "\n";
        }
        cout << "\n";
    }

    bool isGameEnded() const {
        for (const auto& row : board) {
            for (const auto& cell : row) {
                if (cell == 0) {
                    return false;
                }
            }
        }
        return true;
    }

    int evaluate() const {
        for (int i = 0; i < 3; i++) {
            if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != 0) {
                return board[i][0];
            }
        }

        for (int i = 0; i < 3; i++) {
            if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != 0) {
                return board[0][i];
            }
        }

        if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != 0) {
            return board[0][0];
        }

        if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != 0) {
            return board[0][2];
        }

        return 0;
    }

    int minimax(int depth, bool isMaximizing, int alpha, int beta) {
        int score = evaluate();
        if (score == AI) {
            return score + depth;
        }
        if (score == PLAYER) {
            return score - depth;
        }
        if (isGameEnded()) {
            return 0;
        }

        if (isMaximizing) {
            int best = -INF;
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    if (board[i][j] != 0) {
                        continue;
                    }
                    
                    board[i][j] = PLAYER;
                    best = max(best, minimax(depth + 1, false, alpha, beta));
                    board[i][j] = 0;

                    alpha = max(alpha, best);
                    if (beta <= alpha) {
                        return best;
                    }
                }
            }
            return best;
        } else {
            int best = INF;
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    if (board[i][j] != 0) {
                        continue;
                    }

                    board[i][j] = AI;
                    best = min(best, minimax(depth + 1, true, alpha, beta));
                    board[i][j] = 0;

                    beta = min(beta, best);
                    if (beta <= alpha) {
                        return best;
                    }
                }
            }
            return best;
        }
    }

    void playerMove() {
        int x, y;
        cout << "Enter row and column\n";
        cin >> x >> y;
        --x; --y;

        if (x < 0 || x >= 3 || y < 0 || y >= 3 || board[x][y] != 0) {
            cout << "Move not valid!\n";
            playerMove();
        } else {
            board[x][y] = PLAYER;
            printBoard();
        }
    }

    void aiMove() {
        int bestVal = INF;
        pair<int, int> bestMove = {-1, -1};

        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board[i][j] != 0) {
                    continue;
                }
                
                board[i][j] = AI;
                int moveVal = minimax(0, true, -INF, INF);
                board[i][j] = 0;

                if (moveVal < bestVal) {
                    bestMove = {i, j};
                    bestVal = moveVal;
                }
            }
        }

        board[bestMove.first][bestMove.second] = AI;
        printBoard();
    }

    bool isGameOver(int player) {
        int winner = evaluate();
        if (winner == player) {
            if (player == PLAYER) {
                cout << "You win!\n";
            } else {
                cout << "AI wins!\n";
            }
            return true;
        }
        if (isGameEnded()) {
            cout << "Draw!\n";
            return true;
        }
        return false;
    }
};

int main() {
    TicTacToe game;
    game.play();
    return 0;
}
