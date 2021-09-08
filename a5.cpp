// a5.cpp
// Please see the README for more information.

#include <iostream>
using namespace std;

#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <unistd.h>
#include <algorithm>
#include <chrono>

#include "cmpt_error.h"
#include "Board.h"
#include "Player.h"
#include "Human_player.h"
#include "Computer_player.h"

// Prints a welcome screen to the user and briefly explains this program.
void welcome() {
	cout << "\nWelcome to James Braun's Reversi game!\n\n";

	cout << "In my program, you can play against an AI opponent,"
		 << " or you can see the results of two AI opponents go head-to-head.\n";

	cout << "Would you like to play yourself, or sit back and watch?\n\n";

	cout << "Select 1 to play, or 2 to watch: ";
}

// This vector of vectors contains the positional evaluation table that determines
// how much controlling each square on the board is worth.
vector<vector<int>> get_positional_table() {
	vector<vector<int>> db;
	vector<int> r1 = {99,  -8,  8,  6,  6,  8,  -8, 99};
	vector<int> r2 = {-8, -24, -4, -3, -3, -4, -24, -8};
	vector<int> r3 = { 8,  -4,  7,  4,  4,  7,  -4,  8};
	vector<int> r4 = { 6,  -3,  4,  0,  0,  4,  -3,  6};
	vector<int> r5 = r4;
	vector<int> r6 = r3;
	vector<int> r7 = r2;
	vector<int> r8 = r1;

	db.push_back(r1);
	db.push_back(r2);
	db.push_back(r3);
	db.push_back(r4);
	db.push_back(r5);
	db.push_back(r6);
	db.push_back(r7);
	db.push_back(r8);

	return db;
}

// For when two CPU players play against each other.
void CPU_play(int p1type, int p2type, int max_games) {
	// Set up the board, positional value table, and computer players.
	Board gameBoard;
	gameBoard.set_positional_table(get_positional_table());

	Computer_player player1;
	player1.invert_pieces();
	Computer_player player2;

	Location player1Move;
	Location player2Move;

	bool gameStillGoing = true;
	int num_games = 0;
	int lostTurns = 0;
	double player2_score = 0;
	int wins = 0;
	int draws = 0;
	int losses = 0;

	// Play as many games as the user specifies.
	while (num_games < max_games) {
		cout << "Game Number: " << num_games + 1 << endl;
		while (gameStillGoing) {

			// If player 1 can make a move...
			if (gameBoard.move_exists(player1.ID())){
				// Selects the move based on the user's choice at the beginning.
				if (p1type == 3) {
					player1Move = player1.MCTS(gameBoard, "pure");
				} else if (p1type == 2) {
					player1Move = gameBoard.get_combined_move(player1.ID(), player2.ID());
				} else if (p1type == 4) {
					player1Move = player1.MCTS(gameBoard, "combined");
				} else {
					player1Move = gameBoard.get_random_move(player1.ID());
				}
				// Makes the move.
				gameBoard.make_move(player1Move, player1.ID());
				lostTurns = 0;
			} else { // If no move possible...
				lostTurns++;
			}
			
			if (lostTurns == 2) {
			// If neither player can make moves consecutively the game is over.
				gameStillGoing = false;
				break;
			}

			// If player 2 can make a move...
			if (gameBoard.move_exists(player2.ID())) {
				// Selects the move based on the user's choice at the beginning.
				if (p2type == 3) {
					player2Move = player2.MCTS(gameBoard, "pure");
				} else if (p2type == 2) {
					player2Move = gameBoard.get_combined_move(player2.ID(), player1.ID());
				} else if (p2type == 4) {
					player2Move = player2.MCTS(gameBoard, "combined");
				} else {
					player2Move = gameBoard.get_random_move(player2.ID());
				}
				// Update the game board.
				gameBoard.make_move(player2Move, player2.ID());
				lostTurns = 0;
			} else { // If player 2 cannot make a move...
				lostTurns++;
			}
			// If neither player can make moves consecutively the game is over.
			if (lostTurns == 2) {
				gameStillGoing = false;
			}
		}
		// Print the results of the game.
		gameBoard.println();
		gameBoard.print_results();
		cout << endl;
		num_games++;

		// Determine who won the game.
		player2_score = player2.get_playout_score(gameBoard);
		if (player2_score == 1) {
			wins++;
		} else if (player2_score == 0) {
			losses++;
		} else {
			draws++;
		}

		lostTurns = 0;
		gameBoard.clear_board();
		gameStillGoing = true;

	}

	// Print the results of all the games.
	cout << "Thank you for playing " << num_games << " games!" << endl << endl;

	cout << "Player 2 won " << wins << " times, tied " << draws << " times, and lost " << losses << " times during " << num_games << " games." << endl << endl;

	cout << "Player 1 played using ";
	if (p1type == 1) {
		cout << "random moves." << endl;
	} else if (p1type == 2) {
		cout << "greedy heuristic moves." << endl;
	} else if (p1type == 3) {
		cout << "pure Monte Carlo tree search." << endl;
	} else if (p1type == 4) {
		cout << "heuristic Monte Carlo tree search." << endl;
	}

	cout << "Player 2 played using ";
	if (p2type == 1) {
		cout << "random moves." << endl;
	} else if (p2type == 2) {
		cout << "greedy heuristic moves." << endl;
	} else if (p2type == 3) {
		cout << "pure Monte Carlo tree search." << endl;
	} else if (p2type == 4) {
		cout << "heuristic Monte Carlo tree search." << endl;
	}
}

int main() {
	srand(time(NULL));
	int game_type;

	// Prints the welcome message.
	welcome();
	// Reads in the user's choice for if they want to play or watch.
	cin >> game_type;
	cout << endl;
	if (game_type != 1 && game_type != 2) {
		cout << "\nYou made an invalid selection. Ending program early.\n";
		return 0;
	}
	cout << "\n\n";

	if (game_type == 2) { // If the user wants to watch...
		cout << "Enter 1 for randomly,\n"
			 << "2 for greedily using a combination of good heuristics,\n"
			 << "3 for via pure Monte Carlo tree search,\n"
			 << "and 4 for via Monte Carlo tree search using the good heuristics.\n\n";

		cout << "How should the first CPU player make moves?\n" << endl;
		int p1type;
		cin >> p1type;
		cout << endl;
		if (p1type != 1 && p1type != 2 && p1type != 3 && p1type != 4) {
			cout << "\nYou made an invalid selection. Ending program early.\n";
			return 0;
		}

		int p2type;
		cout << "Enter 1 for randomly,\n"
			 << "2 for greedily using a combination of good heuristics,\n"
			 << "3 for via pure Monte Carlo tree search,\n"
			 << "and 4 for via Monte Carlo tree search using the good heuristics.\n\n";

		cout << "How should the second CPU player make moves?\n" << endl;
		cin >> p2type;
		cout << endl;
		if (p2type != 1 && p2type != 2 && p2type != 3 && p2type != 4) {
			cout << "\nYou made an invalid selection. Ending program early.\n";
			return 0;
		}

		int max_games;
		cout << "How many games should they play? Choose between 1 and 10,000 (no commas)." << endl;
		cin >> max_games;
		cout << endl;
		if (max_games < 1 || max_games > 10000) {
			cout << "\nYou made an invalid selection. Ending program early.\n";
			return 0;
		}

		// Play the specified number of games using the two CPU types.
		CPU_play(p1type, p2type, max_games);
	} else {
		cout << "So you've decided to try your luck playing against a computer opponent. "
			 << "I'm going to assume that you already know how to play regular Reversi. "
			 << "To play my game, when it's your turn, simply type in the coordinates of the square "
			 << "where you would like to play your piece. (i.e. \"g6\")\n\n"
			 << "You will be \"o\"'s, and you will be going first. Good luck!\n\n\n";

		// Creates the game board, Human player, and Computer player objects.
		Board gameBoard;
		gameBoard.set_positional_table(get_positional_table());

		Human_player User;
		Computer_player CPU;

		string userMove;
		Location computerMove;

		bool gameStillGoing = true;
		int lostTurns = 0;
		int p2type;
		cout << "Enter 1 for randomly,\n"
			 << "2 for greedily using a combination of good heuristics,\n"
			 << "3 for via pure Monte Carlo tree search,\n"
			 << "and 4 for via Monte Carlo tree search using the good heuristics.\n\n";

		cout << "But first, how should the CPU make moves?" << endl;
		cin >> p2type;
		cout << endl;
		if (p2type != 1 && p2type != 2 && p2type != 3 && p2type != 4) {
			cout << "\nYou made an invalid selection. Ending program early.\n";
			return 0;
		}


		cout << "Starting the game!" << endl;

		gameBoard.println();
		while (gameStillGoing) {
			// If the user can make a move...
			if (gameBoard.move_exists(User.ID())){
				User.start_turn();
				cin >> userMove;
				// Make sure the move is valid (if not, ask for another move)
				while (!gameBoard.valid_move(userMove, User.ID())){
					cin >> userMove;
				}
				// Makes the user's move.
				gameBoard.make_move(userMove, User.ID());
				lostTurns = 0;
			} else { // If the user cannot make a move...
				// Skip the user's turn and make note of it.
				User.skip_turn();
				lostTurns++;
			}
			if (lostTurns == 2) {
			// If the user and computer cannot make moves consecutively the game is over.
				gameStillGoing = false;
				break;
			}
			// Print the current game board to aid the user.
			gameBoard.println();

			// If the computer can make a move...
			if (gameBoard.move_exists(CPU.ID())) {
				CPU.start_turn();
				// Find the computer's move using the method the user specified.
				if (p2type == 3) {
					computerMove = CPU.MCTS(gameBoard, "pure");
				} else if (p2type == 2) {
					computerMove = gameBoard.get_combined_move(CPU.ID(), User.ID());
				} else if (p2type == 4) {
					computerMove = CPU.MCTS(gameBoard, "combined");
				} else {
					computerMove = gameBoard.get_random_move(CPU.ID());
				}
				lostTurns = 0;
				// Tell the user the move the computer made.
				CPU.print_move(computerMove);
				// Update the game board.
				gameBoard.make_move(computerMove, CPU.ID());
			} else { // If the computer cannot make a move...
				// Skip the computer's turn and make note of it.
				CPU.skip_turn();
				lostTurns++;
			}
			// Print the updated game board.
			gameBoard.println();
			// If the user and computer cannot make moves consecutively the game is over.
			if (lostTurns == 2) {
				gameStillGoing = false;
			}
		}
		// Prints the results of the game.
		cout << "Here is the final board." << endl;
		gameBoard.println();
		gameBoard.print_results();

		cout << "Player 2 played using ";
		if (p2type == 1) {
			cout << "random moves." << endl;
		} else if (p2type == 2) {
			cout << "greedy heuristic moves." << endl;
		} else if (p2type == 3) {
			cout << "pure Monte Carlo tree search." << endl;
		} else if (p2type == 4) {
			cout << "heuristic Monte Carlo tree search." << endl;
		}

	cout << "\nThank you for playing!" << endl;
	}

	return 0;
}