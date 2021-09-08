// Player.h

// Player base class
class Player {
public:
	// Virtual destructor so derived classes can implement their own destructor if needed.
	virtual ~Player() { }

	// Prints the necessary information to start the turn.
	virtual void start_turn() const = 0;

	// Prints the necessary information if a turn needs to be skipped.
	virtual void skip_turn() const = 0;

	// Returns the ID of either the Computer or the Human (depending on which class it is).
	virtual Square ID() const = 0;

}; // Abstract Player Class