// Board.h

// To make it easier to keep track of and display various different piece types.
enum class Square : char {
	empty = '.', human = 'o', computer = 'x'
};

// Keeps track of where two squares are in relation to one another.
struct Direction {
	int dirRow;
	int dirCol;
};

// Keeps track of where on the board a Square is.
struct Location {
	int row;
	int col;
};

// Board class
class Board {
private:
	// The gameboard itself.
	vector<vector<Square>> board;
	// Keeps track of directions that result in valid moves.
	vector<Direction> possibleDirections;

	// Holds the positional heuristic value of controlling each square on the board.
	vector<vector<int>> posTable;

public:
	// Board constructor
	Board()
	: board(8)
	{
		// Sets every square on the board to be empty.
		for(int i = 0; i < board.size(); i++) {
			board[i] = vector<Square>(8, Square::empty);
		}
		// Sets the four middle squares to standard Reversi starting positions.
		board[3][3] = Square::computer;
		board[3][4] = Square::human;
		board[4][4] = Square::computer;
		board[4][3] = Square::human;
	}

	void clear_board(){
		// Sets every square on the board to be empty.
		for(int i = 0; i < board.size(); i++) {
			board[i] = vector<Square>(8, Square::empty);
		}
		// Sets the four middle squares to standard Reversi starting positions.
		board[3][3] = Square::computer;
		board[3][4] = Square::human;
		board[4][4] = Square::computer;
		board[4][3] = Square::human;
	}

	// Returns the width of the gameboard.
	int width() const {
		return board[0].size();
	}

	// Returns the height of the gameboard.
	int height() const {
		return board.size();
	}

	// Returns the game piece at a Square.
	// Already assumes the Square is on the board.
	Square get(int r, int c) const {
		return board[r][c];
	}

	// Places a Square on the board of a certain type (i.e. Human or Computer).
	// Already assumes the board location is valid.
	void set(int r, int c, const Square& s) {
		board[r][c] = s;
	}

	// Sets the table containing the positional heuristic value of controlling each square on the board.
	void set_positional_table(const vector<vector<int>>& ptbl) {
		posTable = ptbl;
	}

	// Returns the positional worth of a given square.
	int get_postional_square_score(Location loc) {
		int r = loc.row;
		int c = loc.col;
		return posTable[r][c];
	}

	// Calculates the positional strength of the player's board.
	// Does this by adding up the positional worth of all the player's pieces 
	// and subtracting the positional worth of all the opponent's pieces.
	int get_positional_board_score(Square s, Square opp) {
		int score = 0;
		for (int r = 0; r < 8; r++) {
			for (int c = 0; c < 8; c++) {
				if (board[r][c] == s) {
					score += posTable[r][c];
				} else if (board[r][c] == opp) {
					score -= posTable[r][c];
				}
			}
		}
		return score;
	}

	// Searches all the squares of the board and returns the number of pieces on it for a given player.
	int num_pieces(Square s) const {
		int count = 0;
		for(int i = 0; i < height(); i++) {
			for (int j = 0; j < width(); j++) {
				if (board[i][j] == s) {
					count++;
				}
			}
		}
		return count;
	}

	// Checks if a Square is on the gameboard or not.
	bool on_board(int row, int col) const {
		return (0 <= row && row <= 7)
			&& (0 <= col && col <= 7);
	}

	// Checks all Squares of the board to see if a move is available.
	bool move_exists(Square s) const {
		for (int r = 0; r < 8; r++) {
			for (int c = 0; c < 8; c++) {
				if (valid_move(r, c, s)) {
					return true;
				}
			}

		}
		return false;
	}

	// Takes in a string from the user that represents a move.
	// Checks to see if the move is valid and prints error helpful error message if it is not.
	bool valid_move (string move, Square s) const {
		// It's invalid if the user ever enters a string that is not two characters long.
		if (move.size() != 2) {
			cout << "Invalid move, improper format, please re-enter.\n";
			return false;
		}
		int r = move.at(1)-49;
		int c = move.at(0)-97;
		// It's invalid if the user enters a Square that is not on the board.
		if (!on_board(r, c)) {
			cout << "Invalid move, square not on board, please re-enter.\n";
			return false;
		}
		// The Square must be empty for a move to be made on it.
		if (board[r][c] != Square::empty) {
			cout << "Invalid move, piece already in space, please re-enter.\n";
			return false;
		}
		// If any pieces are flipped by the making of the move then the move is valid.
		if (valid_move(r, c, s)){
			return true;
		} else {
			cout << "Invalid move, cannot flip any pieces, please re-enter.\n";
			return false;
		}
	}

	// Checks if a move is valid for any direction.
	// If any direction would result in pieces being flipped, then it is a valid move.
	// n and m represent the vertical and horizontal directions from the Square.
	// Assumes Square is on the board.
	bool valid_move(int r, int c, Square s) const {
		if (board[r][c] != Square::empty) {
			return false;
		}
		for (int n = -1; n < 2; n++) {
			for (int m = -1; m < 2; m++) {
				if (valid_move(r, c, n, m, s)) {
					return true;
				}
			}
		}
		return false;
	}

	// Checks if a move for a player in a certain direction is valid.
	// y and x represent the vertical and horizontal coordinates of the Square after moved in the m/n direction
	bool valid_move(int r, int c, int n, int m, Square s) const {
		int y = r+n;
		int x = c+m;
		// If moving in that direction takes you off the board, it is not valid.
		if (!on_board(y, x)){
			return false;
		}
		// Placing a piece adjacent to another of the same type is not a valid move
		// (at least in the direction between the two pieces)
		if (get(y, x) == s) {
			return false;
		}
		// Keep moving in the same direction until another anchor piece is found (which makes the move valid)
		// Or if you move off the board or hit an empty square then the move is not valid in that direction
		while (on_board(y, x)) {
			if (get(y, x) == s) {
				return true;
			} else if (get(y, x) == Square::empty) {
				return false;
			} else {
				y+=n;
				x+=m;
			}
		}
		return false;
	}

	// Returns the number of empty squares on the board.
	int num_empty_squares() {
		int sum = 0;
		for (int r = 0; r < 8; r++) {
			for (int c = 0; c < 8; c++) {
				if (board[r][c] == Square::empty) {
					sum++;
				}
			}
		}
		return sum;
	}

	// Returns a vector containing all the valid moves the player can make.
	vector<Location> get_valid_moves(Square s) const {
		vector<Location> valid_moves;
		for (int r = 0; r < 8; r++) {
			for (int c = 0; c < 8; c++) {
				if (valid_move(r, c, s)) {
					valid_moves.push_back(Location{r,c});
				}
			}
		}
	return valid_moves;
	}

	// Returns a random valid move the player can make.
	Location get_random_move(Square s) const {
		vector<Location> possibilities = get_valid_moves(s);
		int index = rand() % possibilities.size();
		return possibilities[index];
	}

	// Returns the best move the player can make using a variety of heuristics.
	Location get_combined_move(Square s, Square opp) {
		vector<Location> possibilities = get_valid_moves(s);
		vector<double> good_moves;

		double positional_score;
		double opp_moves;
		double flipped;
		double temp_score;

		// The weights of all the heuristics.
		// Determined these myself through trial and error.
		double w1 = 1;
		double w2 = 0.3;
		double w3 = 0.8;
		double w4 = 0;

		vector<double> all_scores;

		for (int i = 0; i < possibilities.size(); i++) {
			int r = possibilities[i].row;
			int c = possibilities[i].col;

			// Creates a temporary board to make each move on.
			Board tempBoard = *this;
			tempBoard.make_move(possibilities[i], s);

			// Returns the positional score of the player's pieces after the move has been made.
			positional_score = tempBoard.get_positional_board_score(s, opp);
			// Returns how many moves the opponent can make after the move has been made.
			opp_moves = tempBoard.get_valid_moves(opp).size();
			// Returns how many pieces will get flipped after the move has been made.
			flipped = calc_score(r, c, s);

			// The weights get adjusted during the late-game.
			// Again, I created these weights myself through trial and error.
			if (num_empty_squares() <= 10) {
				w1 = 0.1;
				w2 = 0.6;
				w3 = 0;
				w4 = 4;
			}

			// Weights the heuristics to generate a single score for a move.
			temp_score = (w1 * positional_score) + (w2 * (100 / (opp_moves + 1))) + (w3 * (100 / (flipped + 1))) + (w4 * 100 * flipped);
			all_scores.push_back(temp_score);
		}

		// Determines which moves result in the greatest score.
		// Breaks ties randomly.
		double max_score = *max_element(all_scores.begin(), all_scores.end());
		for (int i = 0; i < all_scores.size(); i++) {
			if (all_scores[i] == max_score) {
				good_moves.push_back(i);
			}
		}
		int good_index = rand() % good_moves.size();
		return possibilities[good_moves[good_index]];
	}


	// Assumes a valid move is passed in
	void make_move(string userMove, Square s) {
		int r = userMove.at(1)-49;
		int c = userMove.at(0)-97;
		Location move {r, c};
		make_move(move, s);
	}

	// First determine which directions can flip pieces,
	// then for each direction that works, flip the pieces in that direction (all the ones that are flippable).
	void make_move(Location move, Square s) {
		int r = move.row;
		int c = move.col;
		for (int n = -1; n < 2; n++) {
			for (int m = -1; m < 2; m++) {
				if (valid_move(r, c, n, m, s)) {
					possibleDirections.push_back(Direction{n, m});
				}
			}
		}
		set(r, c, s);
		for (Direction &d : possibleDirections) {
			make_move(r, c, d.dirRow, d.dirCol, s);
		}
		// Clears the possible directions vector for the next move.
		possibleDirections.clear();
	}

	// Flips pieces on the gameboard in a certain direction.
	void make_move(int r, int c, int n, int m, Square s) {
		int x = c+m;
		int y = r+n;
		while (board[y][x] != s) {
			set(y, x, s);
			y+=n;
			x+=m;
		}
	}

	// Returns how many pieces would be flipped in a certain direction if a move was made.
	int test_move(int r, int c, int n, int m, Square s) const {
		int count = 0;
		int x = c+m;
		int y = r+n;
		while (board[y][x] != s) {
			count++;
			y+=n;
			x+=m;
		}
		return count;
	}

	// Calculates how many pieces a move would score if made.
	// Very similar to the make_move functions.
	int calc_score(int r, int c, Square s) {
		int count = 0;
		if (valid_move(r, c, s)) {
			for (int n = -1; n < 2; n++) {
				for (int m = -1; m < 2; m++) {
					if (valid_move(r, c, n, m, s)) {
						possibleDirections.push_back(Direction{n, m});
					}
				}	
			}
			for (Direction &d : possibleDirections) {
				count += test_move(r, c, d.dirRow, d.dirCol, s);
			}
			possibleDirections.clear();
			count++;
		}
		return count;

	}

	// Prints the gameboard.
	// Includes an A-H and 1-8 coordinate system to help the user.
	void print() const {
		cout << "  ";
		for (int i = 0; i < width(); i++) {
			char c = 'a' + i;
			cout << c << " ";
		}
		cout << endl;
		for(int i = 0; i < height(); i++) {
			cout << i + 1 << " ";
			for (int j = 0; j < width(); j++) {
				cout << char(board[i][j]) << " ";
			}
			cout << endl;
		}
		// Prints the number of pieces each player has.
		cout << " Player 1    (O): " << num_pieces(Square::human) << endl;
		cout << " Player 2    (X): " << num_pieces(Square::computer) << endl;
	}

	// Prints the gameboard with a new line feed.
	void println() const {
		print();
		cout << endl;
	}

	// Prints the results of the game after it completes.
	// Says who won and what the final score was.
	void print_results() const {
		cout << "The game is over as no one can make a move.\n";
		if (num_pieces(Square::computer) > num_pieces(Square::human)) {
			cout << "Player 2 wins by a score of "
				 << num_pieces(Square::computer) << " to " << num_pieces(Square::human) << ".\n";
		} else if (num_pieces(Square::computer) < num_pieces(Square::human)) {
			cout << "Player 1 wins by a score of "
				 << num_pieces(Square::human) << " to " << num_pieces(Square::computer) << ".\n";
		} else {
			cout << "It's a tie as both players scored: "
				 << num_pieces(Square::human) << " points.\n";
		}
	}

}; // Class Board