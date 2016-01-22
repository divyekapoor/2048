#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <set>
#include <math.h>

using namespace std;

typedef vector<vector<int>> Board;
typedef vector<int> Row;
typedef char Move;

void PrintRow(const Row& row) {
	for (int j = 0; j < row.size(); ++j) {
		printf("%4d ", row[j]);
	}
	printf("\n");
}

void PrintBoard(const Board& board) {
	for (int i = 0; i < board.size(); ++i) {
		PrintRow(board[i]);
	}
}

void GenerateInitialState(Board* board) {
	const int N = 5;
	int x_max = board->size();
	int y_max = board->at(0).size();
	for (int i = 0; i < N; ++i) {
		board->at(rand() % x_max).at(rand() % y_max) = 2;
	}
}

template<class T>
T& At(vector<vector<T>>* board, int i, int j) {
	return board->at(i).at(j);
}

// Returns the number of moves made on the board.
int ApplyMove(Board* board, Move move, int* score) {
	int x_size = board->size();
	int y_size = board->at(0).size();
	int move_count = 0;
	set<pair<int, int>> merged;
	switch (move) {
		case 'w':
		case 'W':
		for (int i = 0; i < x_size; ++i) {
			for (int j = 0; j < y_size; ++j) {
				if (At(board, i, j) == 0) {
					continue;
				}
				// Skip over zeros.
				int k = i - 1;
				while (k >= 0 && At(board, k, j) == 0) {
					swap(At(board, k, j), At(board, k + 1, j));
					k -= 1;
					++move_count;
				}
				// Apply tile collapse.
				if (k >= 0 && At(board, k, j) == At(board, k + 1, j) &&
					merged.find(make_pair(k, j)) == merged.end()) {
					At(board, k, j) += At(board, k, j);
					At(board, k + 1, j) = 0;
					*score += At(board, k, j);
					merged.insert(make_pair(k, j));
					++move_count;
				}
			}
		}
		break;

		case 's':
		case 'S':
		for (int i = x_size - 1; i >= 0; --i) {
			for (int j = 0; j < y_size; ++j) {
				if (At(board, i, j) == 0) {
					continue;
				}
				// Skip over zeros.
				int k = i + 1;
				while (k < x_size && At(board, k, j) == 0) {
					swap(At(board, k, j), At(board, k - 1, j));
					k += 1;
					++move_count;
				}
				// Apply tile collapse.
				if (k < x_size && At(board, k, j) == At(board, k - 1, j) &&
					merged.find(make_pair(k, j)) == merged.end()) {
					At(board, k, j) += At(board, k, j);
					At(board, k - 1, j) = 0;
					*score += At(board, k, j);
					merged.insert(make_pair(k, j));
					++move_count;
				}
			}
		}
		break;

		case 'a':
		case 'A':
		for (int j = 0; j < y_size; ++j) {
			for (int i = 0; i < x_size; ++i) {
				if (At(board, i, j) == 0) {
					continue;
				}
				// Skip over zeros.
				int k = j - 1;
				while (k >= 0 && At(board, i, k) == 0) {
					swap(At(board, i, k), At(board, i, k + 1));
					k -= 1;
					++move_count;
				}
				// Apply tile collapse.
				if (k >= 0 && At(board, i, k) == At(board, i, k + 1) &&
					merged.find(make_pair(i, k)) == merged.end()) {
					At(board, i, k) += At(board, i, k);
					At(board, i, k + 1) = 0;
					merged.insert(make_pair(i, k));
					*score += At(board, i, k);
					++move_count;
				}
			}
		}
		break;

		case 'd':
		case 'D':
		for (int j = y_size - 1; j >= 0; --j) {
			for (int i = 0; i < x_size; ++i) {
				if (At(board, i, j) == 0) {
					continue;
				}
				// Skip over zeros.
				int k = j + 1;
				while (k < y_size && At(board, i, k) == 0) {
					swap(At(board, i, k), At(board, i, k - 1));
					k += 1;
					++move_count;
				}
				// Apply tile collapse.
				if (k < x_size && At(board, i, k) == At(board, i, k - 1) &&
					merged.find(make_pair(i, k)) == merged.end()) {
					At(board, i, k) += At(board, i, k);
					At(board, i, k - 1) = 0;
					merged.insert(make_pair(i, k));
					*score += At(board, i, k);
					++move_count;
				}
			}
		}
		break;
	}
	return move_count;
}

void AddNewNumbers(Board* board, int N) {
	vector<pair<int, int>> zeros;
	int x_size = board->size();
	int y_size = board->at(0).size();
	for (int i = 0; i < x_size; ++i) {
		for (int j = 0; j < y_size; ++j) {
			if (At(board, i, j) == 0) {
				zeros.emplace_back(i, j);
			}
		}
	}
	int add_count = 0;
	while (add_count == 0) {
		for (int i = 0; i < zeros.size(); ++i) {
			if (rand() % zeros.size() < N) {
				At(board, zeros[i].first, zeros[i].second) = 2;
				++add_count;
        if (add_count == N) return;
			}
		}
	}
}

bool IsGameOver(const Board& board) {
	string options = "wasd";
	int score_produced = 0;
	for (int i = 0; i < options.size(); ++i) {
		Board b(board);
		if (ApplyMove(&b, options[i], &score_produced) != 0) {
			return false;
		}
	}
	return true;
}

char HumanGetMove(const Board& board) {
	char c;
	printf("\n");
	PrintBoard(board);
	printf("Enter move: ");
	scanf(" %c", &c);
	return c;
}

char RandomGetMove(const Board&) {
	string options = "wasd";
	return options[rand() % 4];
}

char WeightedRandomGetMove(const Board&) {
	int r = rand() % 16;
	// Tweak these numbers as you wish.
	if (r < 5) {
		return 'w';
	} else if (r < 10) {
		return 's';
	} else if (r < 14) {
		return 'a';
	} else {
		return 'd';
	}
}

struct Node {
	vector<float> weights;
	float value;
};

vector<vector<Node>> nodes;

void InitRL(int num_rows, int num_cols) {
	nodes.clear();
	const int num_layers = 4;
	nodes.resize(num_layers);
	nodes[0].resize(num_rows * num_cols);
	nodes[1].resize(128);
	nodes[2].resize(128);
	nodes[3].resize(4);

	// For each layer
	for (int i = 1; i < num_layers; ++i) {
		// For each node in layer.
		for (int j = 0; j < nodes[i].size(); ++j) {
			nodes[i][j].weights.resize(nodes[i-1].size());
			// For each weight in node.
			for (int k = 0; k < nodes[i][j].weights.size(); ++k) {
				// Initialize randomly in [-1, 1]
				nodes[i][j].weights[k] = rand() / (RAND_MAX / 2.) - 1;
			}
		}
	}
}

void EvalRL(const Board& board) {
	// Copy in board.
	int index = 0;
	const int max_value = 4096;
	for (int i = 0; i < board.size(); ++i) {
		for (int j = 0; j < board[i].size(); ++j) {
			nodes[0][index].value = board[i][j] / max_value;
		}
	}

	// Evaluate values.
	for (int i = 1; i < nodes.size(); ++i) {
		for (int j = 0; j < nodes[i].size(); ++j) {
			nodes[i][j].value = 0;
			for (int k = 0; k < nodes[i][j].weights.size(); ++k) {
				nodes[i][j].value += nodes[i][j].weights[k] * nodes[i-1][k].value;
			}
			nodes[i][j].value /= nodes[i][j].weights.size();
		}
	}

}

char RLGetMove(const Board& board) {
	int x_size = board.size();
	int y_size = board[0].size();
	float max_value = 4096;
  vector<float> input_weights(board.size() * board[0].size());
	for (int i = 0; i < x_size; ++i) {
		for (int j = 0; j < y_size; ++j) {
			input_weights[i * x_size + j] = board[i][j] / max_value;
		}
	}
  return 'w';
}

int Play(Board* board) {
	int total_score = 0;
	char c;
	while ((c = HumanGetMove(*board))) {
		int score_produced = 0;
		int result = ApplyMove(board, c, &score_produced);
		if (result == 0) {
			printf("Illegal move.\n");
			goto end;
		}
		total_score += score_produced;
		printf("Scored: %d Total: %d\n", score_produced, total_score);
		AddNewNumbers(board, 1);
		if (IsGameOver(*board)) {
			printf("Game over!\n");
			printf("Score: %d\n", total_score);
			break;
		}
	end:
		;
	}
	return total_score;
}

int main() {
	srand(time(0));
	const int num_rows = 5, num_cols = 5;
	InitRL(num_rows, num_cols);
	const int num_games = 100;
	vector<pair<int, int>> scores;
	for (int i = 0; i < num_games; ++i) {
		Board board(num_rows, Row(num_cols, 0));
		GenerateInitialState(&board);
		printf("Initial board: \n");
		PrintBoard(board);
		printf("\n");
		int score = Play(&board);
		int max_value = 0;
		for (int j = 0; j < board.size(); ++j) {
			for (int k = 0; k < board[0].size(); ++k) {
				max_value = max(board[j][k], max_value);
			}
		}
		scores.push_back(make_pair(score, max_value));
	}
	sort(scores.begin(), scores.end());

	double mean = 0;
	double std_dev = 0;
	printf("\n--- Scores:");
	for (int i = 0; i < scores.size(); ++i) {
		mean += scores[i].first;
		std_dev += (scores[i].first * scores[i].first);
		printf(" (%d, %d)", scores[i].first, scores[i].second);
	}
	printf("\n");
	mean /= scores.size();
	std_dev = sqrt((std_dev - mean*mean) / scores.size());
	printf("Mean: %g Std. Dev: %g\n", mean, std_dev);
	return 0;
}
