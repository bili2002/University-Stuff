#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <queue>
#include <atomic>
#include <optional>
#include <utility>
#include <thread>

#define print(x) prettyPrint(x, width, height)

void prettyPrint(const std::string &board, const uint32_t width, const uint32_t height) {
	for (uint32_t i = 0; i < height; i++) {
		std::cout << board.substr(i * width, width) << std::endl;
	}
}

char negate(const char c) {
	if (c == 'X') {
		return 'O';
	} else {
		return 'X';
	}
};

std::string shiftDown(const std::string &board, const uint32_t width, const uint32_t height) {
	std::string ret = board;
	for (int64_t i = ret.size() - 1; i >= 0; i--) {
		if (ret[i] == '_') { continue; }
		int cpy = i;
		while (cpy + width < ret.size() && ret[cpy + width] == '_') {
			std::swap(ret[cpy], ret[cpy + width]);
			cpy += width;
		}
	}
	return ret;
}

bool hasWon(const std::string &board, const uint32_t width, const uint32_t height, const char moved, const uint32_t connectCountToWin) {
	bool ret = false;
	for (uint32_t i = 0; i < width; i++) {
		for (uint32_t j = 0; j < height; j++) {
      			const auto pos = i + j * width;
			if (i + connectCountToWin <= width) { // Row Win
				bool all = true;
				for (uint32_t k = 0; k < connectCountToWin; k++) {
					all &= (board[pos + k] == moved);
				}
				ret |= all;
			}

			if (j + connectCountToWin <= height) { // Column
				bool all = true;
				for (uint32_t k = 0; k < connectCountToWin; k++) {
					all &= (board[pos + k * width] == moved);
				}
				ret |= all;
			}

			if (i + connectCountToWin <= width && j + connectCountToWin <= height) { // Diagonal
				bool all = true;
				for (uint32_t k = 0; k < connectCountToWin; k++) {
					all &= (board[pos + k * (width + 1)] == moved);
				}
				ret |= all;
			}
		}
	}

	return ret;
}

std::pair<std::string, bool> resolveWin(const std::string &board, const uint32_t width, const uint32_t height, const char moved, const uint32_t connectCountToWin) {
	std::string ret = board;
	bool win = hasWon(board, width, height, moved, connectCountToWin);
	if (win) {
		for (auto &it : ret) {
			it = (it == moved) ? '_' : it;
		}
		ret = shiftDown(ret, width, height);
	}

	const auto opp = negate(moved);
	if (win && hasWon(ret, width, height, opp, connectCountToWin)) {
		for (auto &it : ret) {
			it = (it == opp) ? '_' : it;
		}
		ret = shiftDown(ret, width, height);
	}

	return { ret, win };
};

std::string resolveDraw(const std::string &board, const uint32_t width, const uint32_t height) {
	std::string ret = board;

	const auto clear = std::min(height - 1, (uint32_t)2);

	for (uint32_t i = 0; i < clear * width; i++) {
		ret[width * height - clear * width + i] = '_';
	}

	return shiftDown(ret, width, height);
};

std::vector<std::string> generateNext(const std::string &board, const uint32_t width, const uint32_t height, const char toMove) {
	std::vector<std::string> all = {};
	for (uint32_t i = 0; i < width; i++) {
		if (board[i] == '_') {
			auto ret = board;
			ret[i] = toMove;
			all.push_back(shiftDown(ret, width, height));
		}
	}

	return all;
}

// State for the thread pool
struct State {
	std::string board;
	char nextToMove;
	uint64_t calcDepth;
};

uint32_t total = 0;

struct ExecutionState {
	uint32_t width, height, connectCountToWin;
	uint64_t winX = 0, winO = 0, draw = 0;
	std::vector<std::string> boards;
};

struct PQueue {
	std::queue<State> q = {};
	std::mutex mut;

	void push(const State &it) {
		mut.lock();
		total++;
		q.push(it);
		mut.unlock();
	}

	void push_all(const std::vector<std::string> &boards, const char nextMove, const uint64_t calcDepth) {
		mut.lock();
		total++;
		for (const auto &it : boards) {
			q.push({it, nextMove, calcDepth});
		}
		mut.unlock();
	}

	auto pop() {
		mut.lock();
		total++;
		std::optional<State> ret = std::nullopt;
		if (!q.empty()) {
			ret = q.front();
			q.pop();
		}
		mut.unlock();
		return ret;
	}
};

void compute(ExecutionState &exec, const std::string &board, const char nextToMove, const uint64_t calcDepth, const uint32_t threadCalcDepth = 1) {
	if (threadCalcDepth == 0) {
		exec.boards.push_back(board);
		return;
	}

	const auto prev = negate(nextToMove);
	const auto win = resolveWin(board, exec.width, exec.height, prev, exec.connectCountToWin);

	if (win.second) {
		if (prev == 'X') {
			exec.winX++;
		} else {
			exec.winO++;
		}
	}

	if (calcDepth == 0) {
		if (!win.second) {
			exec.draw++;
		}
		return;
	}

	auto cont = generateNext(win.first, exec.width, exec.height, nextToMove);

	if (cont.empty()) {
		cont = generateNext(resolveDraw(win.first, exec.width, exec.height), exec.width, exec.height, nextToMove);
	}

	for (const auto &it : cont) {
		compute(exec, it, prev, calcDepth - 1, threadCalcDepth - 1);
	}
}

void bruteFOURce(
	const char *board, 
	const int width, 
	const int height, 
	const int connectCountToWin, 
	const char nextToMove, 
	const int calcDepth,
	const uint32_t THREAD_COUNT = 1) { 

	const uint64_t THREAD_DEPTH = 6;

	std::atomic<uint64_t> active = 0;
	std::atomic<uint64_t> winX, winO, draw;
	std::vector<std::thread> workers = {};

	PQueue queue;
	queue.push({std::string(board), nextToMove, static_cast<uint64_t>(calcDepth)});

	for (uint32_t i = 0; i < THREAD_COUNT; i++) {
		workers.push_back(std::thread([&] () {
			// Sometimes we skip the whole mutex locking phase
			// by saving 1 of the last iteration
			bool found = false;
			State next;

			// These are the width, height and the currently queued
			ExecutionState exec{static_cast<uint32_t>(width), static_cast<uint32_t>(height), static_cast<uint32_t>(connectCountToWin)};

			while (true) {
				// Can be done with a condition variable, however it was slower as no waiting occurs in reality
				if (!found) {
					const auto curr = queue.pop();

					if (!curr) {
						if (active == 0) {
							break;
						} else {
							continue;
						}
					}

					next = curr.value();
				}

				active++;

				compute(exec, next.board, next.nextToMove, next.calcDepth, THREAD_DEPTH);

				if (!exec.boards.empty()) {
					const auto mv = (THREAD_DEPTH % 2 == 1 ? negate(next.nextToMove) : next.nextToMove);
					const auto depth = next.calcDepth - THREAD_DEPTH;

					found = true;

					next = State{ exec.boards.back(), mv, depth };
					exec.boards.pop_back();

					// Flush all into parallel queue
					queue.push_all(exec.boards, mv, depth);
					exec.boards.resize(0);
				} else {
					found = false;
				}

				active--;
			}

			winX += exec.winX;
			winO += exec.winO;
			draw += exec.draw;
		}));
	}

	for (auto &it : workers) {
		it.join();
	}

	std::cout << "X won " << winX << ", Y won " << winO << ", " << draw << " draws" << std::endl;
}

int main() {
	for (uint32_t i = 1; i <= 8; i++) {
		total = 0;
		const auto begin = std::chrono::high_resolution_clock::now();
		bruteFOURce("_____O___X_", 4, 3, 3, 'X', 14, i);
		const auto end = std::chrono::high_resolution_clock::now();
		std::cout << i << ": " << (end - begin).count() << " Mutex locks: " << total << std::endl;
	}
	// bruteFOURce("OOOOOOOOOOOOOOOO", 4, 4, 100, 'X', 2);
	return 0;
}
