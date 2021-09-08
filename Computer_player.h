// Computer_player.h

// Computer Player class
class Computer_player : public Player {
private:
	// Square type to identify Computer's piece.
	Square s;
	Square opp;
public:
	// Constructor
	Computer_player() 
	: s(Square::computer), opp(Square::human)
	{	}

	// Destructor
	~Computer_player() { }

	// Normally the CPU is player two, but it can also be player one.
	void invert_pieces() {
		s = Square::human;
		opp = Square::computer;
	}

	// Let's the player know the computer is going.
	void start_turn() const {
		cout << "Computer calculating optimal move... \n";
	}

	// Prints what move the computer is making.
	void print_move(Location loc) const {
		cout << char(s) << " is making a move on square: ";
		char row = loc.row + 49;
		char col = loc.col + 97;
		cout << col << row << endl;
	}

	// Determines if the CPU won, tied, or lost.
	// Returns 1 if the CPU won, 0.5 if they tied, and 0 if they lost.
	double get_playout_score(Board b) {
		if(b.num_pieces(s) > b.num_pieces(opp)){
			return 1;
		} else if (b.num_pieces(s) < b.num_pieces(opp)) {
			return 0;
		} else {
			return 0.5;
		}
	}

	// Plays out the remainder of the game using either random moves or heuristic moves and returns the playout score.
	double play_out(Board b, string heuristic) {
		int lost_turns = 0;
		Location human_move;
		Location computer_move;
		while (true) {
			if (b.move_exists(opp)) {
				if (heuristic == "pure") {
					human_move = b.get_random_move(opp);
				} else if (heuristic == "combined") {
					human_move = b.get_random_move(opp);
				}
				b.make_move(human_move, opp);
				lost_turns = 0;
			} else {
				lost_turns++;
			}
			if (lost_turns == 2) {
				return get_playout_score(b);
			}
			
			if (b.move_exists(s)) {
				if (heuristic == "pure") {
					computer_move = b.get_random_move(s);
				} else if (heuristic == "combined") {
					computer_move = b.get_combined_move(s, opp);
				}
				b.make_move(computer_move, s);
				lost_turns = 0;
			}
			if (lost_turns == 2) {
				return get_playout_score(b);
			}
		}
	}

	// Performs Monte Carlo tree search.
	// Either a pure or heuristic version.
	// Uses a number of playouts to make better moves.
	// Caps the time allowed to make a move at 5 seconds.
	// It does this by dividing 5 by the number of valid moves to perform playouts on.
	// It spends at most that amount of time on each move, or it stops sooner if 250 playouts have been done.
	// If less than 250 playouts can be performed for a given move, the score is adjusted to as if 250 playouts were done.
	// This prevents the program from simply picking the move that had the most random playouts done on it.
	Location MCTS(Board b, string heursitic) {
		int num_playouts = 250;
		double max_seconds = 5;
		double max_milliseconds = max_seconds * 1000;
		vector<Location> val_moves = b.get_valid_moves(s);
		vector<double> scores (val_moves.size());
		auto time_per = max_milliseconds / val_moves.size(); // This determines how long to spend on each move.

		for (int i = 0; i < val_moves.size(); i++) { // For every valid move...
			auto start = chrono::steady_clock::now();
			for (int j = 0; j < num_playouts; j++) { // Perform a set amount of playouts...
				auto end = chrono::steady_clock::now();
				auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start).count();
				// Stop performing playouts if more than the allotted time has passed.
				if (elapsed > time_per) {
					// This adjusts the score of making the move to as if all 250 playouts were done.
					scores[i] = scores[i] * (250.0 / j);
					break;
				}
				Board temp = b; // Creates a temporary copy of the board to perform the random playouts on.
				temp.make_move(val_moves[i], s);
				scores[i] += play_out(temp, heursitic);
			}
		}
		// This vector stores the indices of all the moves that result in the highest score.
		vector<double> max_moves;
		double max_score = *max_element(scores.begin(), scores.end());
		for (int i = 0; i < scores.size(); i++) {
			if (scores[i] == max_score) {
				max_moves.push_back(i);
			}
		}
		// If more than one move results in the same max score, this randomly choose one.
		int max_index = rand() % max_moves.size();

		// Returns the move that corresponds to the highest playout score.
		return val_moves[max_moves[max_index]];
	}

	// Informs the player that the computer is unable to make a move and it will be their turn again.
	void skip_turn() const {
		cout << char(s) << " cannot make a move, skipping turn.\n";
	}

	// Returns the ID of either the Computer's or the Human's piece (depending on which class it is).
	Square ID() const {
		return s;
	}

}; // Computer Player Class