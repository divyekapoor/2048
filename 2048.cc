#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <set>

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
	srand(0);
	int x_max = board->size();
	int y_max = board->at(0).size();
	for (int i = 0; i < N; ++i) {
		board->at(rand() % x_max).at(rand() % y_max) = 2;
	}
}

int& At(Board* board, int i, int j) {
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

int main() {
	Board board(5, Row(5, 0));
	
	GenerateInitialState(&board);
	printf("Initial board: \n");
	PrintBoard(board);
	printf("\n");
	
	/*
	  For testing
	
	Board top_board(board), left_board(board), right_board(board), bottom_board(board);
	int top_board_result = ApplyMove(&top_board, 'w');
	int left_board_result = ApplyMove(&left_board, 'a');
	int bottom_board_result = ApplyMove(&bottom_board, 's');
	int right_board_result = ApplyMove(&right_board, 'd');
	
	printf("\nTop Board: %d\n", top_board_result);
	PrintBoard(top_board);
	printf("\nLeft Board: %d\n", left_board_result);
	PrintBoard(left_board);
	printf("\nRight Board: %d\n", right_board_result);
	PrintBoard(right_board);
	printf("\nBottom Board: %d\n", bottom_board_result);
	PrintBoard(bottom_board);
	
	*/
	
	int total_score = 0;
	char c;	
	printf("Enter move: ");
	while (scanf(" %c", &c)) {
		int score_produced = 0;
		int result = ApplyMove(&board, c, &score_produced);
		if (result == 0) {
			printf("Illegal move.\n");
			goto end;
		}
		total_score += score_produced;
		printf("Scored: %d Total: %d\n", score_produced, total_score);
		AddNewNumbers(&board, 1);
		if (IsGameOver(board)) {
			printf("Game over!\n");
			printf("Score: %d\n", total_score);
			break;
		}
		
	end:
		printf("\n");
		PrintBoard(board);
		printf("Enter move: ");
	}
	
	return 0;
}