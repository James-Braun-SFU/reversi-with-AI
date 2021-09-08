# Reversi with AI - README

First off, I want to acknowledge that the basis of this Reversi program was created by myself for Professor Donaldson's CMPT 135 course during the Spring 2017 semester. The major changes I made to this program were to the following files and functions:

###  Board.h
- set_positional_table()
- get_positional_square_score()
- get_positional_board_score()
- num_empty_squares()
- get_valid_moves()
- get_random_move()
- get_combined_move()

### Computer_player.h
- invert_pieces()
- get_playout_score()
- play_out()
- MCTS()

and all of `a5.cpp`. 

I received explicit verbal consent from Professor Donaldson to re-use the other parts of the code. He was also the author of the `cmpt_error.h` and `makefile` files.

I also re-used ideas about using Monte Carlo tree search from my own assignment #3 for CMPT 310.

In addition, I read the following websites for help with various programming portions:
- https://www.techiedelight.com/measure-elapsed-time-program-chrono-library/
	- for help with timing the playouts.
- https://www.geeksforgeeks.org/how-to-find-the-maximum-element-of-a-vector-using-stl-in-c/
	- for how to get the maximum element of a vector.
- http://www.cplusplus.com/reference/cstdlib/rand/
	- for how to get a random integer.

Finally, I learned how to make good Reversi moves from the following websites. I did not use any code from any of them:
- http://play-othello.appspot.com/files/Othello.pdf
- http://samsoft.org.uk/reversi/strategy.htm
	- this website is also where I got the evaluation table for the positional heuristic.


This program was written in C++ using only the standard C++ library.

There are three ways to run my program. If the first way doesn't work, please try the second, and if that doesn't work, please try the third.

### Way 1:
I have already compiled my program, so to run it you should be able to just go: 
`./a5`

However, on some Unix systems you may get the error message:
`bash: ./a5: Permission denied`

If that happens, try 
### Way 2:
The way I solved this (using this as guidence: https://stackoverflow.com/questions/18960689/ubuntu-says-bash-program-permission-denied) was to go:
`chmod u+x a5`
and then go:
`./a5`


If that doesn't work, try
### Way 3:
First, delete the "a5" object (BUT NOT a5.cpp!!!)

Then, re-compile by going:
`make a5`

Finally, try running it with:
`./a5`

Once the program starts, you will be asked a few questions, such as if you would like to play a game of Reversi yourself, how the computer opponent(s) should make moves, and how many games should be played. Please enter in an appropriate response, as if you enter in an invalid selection, while the program won't crash, it will immediately terminate and you will have to run it again.

Should you choose to play a game, feel free to try to make any moves, legal or not, as that portion of my program can properly handle nonsensical or invalid inputs.

If you choose to play, you will play an ordinary game of Reversi against the computer that will make moves using the method of your choice and at the end my program will tell you who won (or if it was a draw).

If you choose to see two computer opponents play, you will choose how they make moves and how many games they will play. To avoid an excessive amount of text on screen, only the final board and the winner of each game will be displayed. After all the games have been played, the program will say how many times the second computer player won, lost, or tied.

As a final note, I took many steps to make my program easier to use and understand. First, the program outputs instructions to help you as you use it. Second, I have commented large portions of my code to improve readability and so you can get a better idea on what each part does. Third, I go over why I chose my heuristics and how I used them in my report (see: `Report.pdf`).
