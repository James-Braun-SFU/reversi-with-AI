// Human_player.h

// Human Player class
class Human_player : public Player {
private:
	// Square type to identify Human's piece
	Square s;
public:
	// Constructor
	Human_player() 
	: s(Square::human)
	{	}

	// Destructor
	~Human_player() { }

	// Asks the user to enter a move.
	void start_turn() const {
		cout << "Player please enter a move: \n";
	}

	// Informs the user they are unable to make a move, thereby skipping their turn.
	void skip_turn() const {
		cout << "Player, you are unable to make a move, skipping your turn.\n";
	}

	// Asks the user if they would like to play another game.
	void ask_another_game() const {
		cout << "Would you like to play again? Enter \"Yes\" for another game "
			 << "and anything else to quit.\n";
	}

	// Returns the ID of the player piece.
	Square ID() const {
		return s;
	}

}; // Human Player Class