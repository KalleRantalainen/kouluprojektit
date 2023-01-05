/*
 COMP.CS.110 Ohjelmointi 2: Rakenteet / Programming 2: Structures
 Project4: 2048
 File: gameboard.hh
 Header file for the gameboard.cpp that has the gameboard game mechanics.
 Notes: Most of this file was given for this project but i did minor
 changes to this file.

 Programmer information:
 -Programmer: Kalle Rantalainen
 -Name: Kalle Rantalainen
 -Student number: 50329870
 -Username: cdkara
 -E-mail: kalle.rantalainen@tuni.fi
*/

#ifndef GAMEBOARD_HH
#define GAMEBOARD_HH

#include "numbertile.hh"
#include <vector>
#include <random>

const int SIZE = 4;
const int NEW_VALUE = 2;
const int DEFAULT_GOAL = 2048;

class GameBoard
{
public:
    // Constructor
    GameBoard();

    // Destructor
    ~GameBoard();

    // Initializes the gameboard with nullptrs.
    void init_empty();

    // Initializes the random number generator and fills the gameboard
    // with random numbers.
    void fill(int seed);

    // Draws a new location (coordinates) from the random number generator and
    // puts the NEW_VALUE on that location, unless check_if_empty is true and
    // the gameboard is full.
    void new_value(bool check_if_empty = true);

    // Returns true, if all the tiles in the game board are occupied,
    // otherwise returns false.
    bool is_full() const;

    // Prints the game board.
    void print() const;

    // Moves the number tiles in the gameboard, if possible (by calling
    // move method for each number tile).
    // Finally, resets turn of all number tiles.
    bool move(Coords dir, int goal);

    // Returns the element (number tile) in the given coordinates.
    NumberTile* get_item(Coords coords);

    // Returns the gameboard with numbertile objects.
    std::vector<std::vector<NumberTile*>> get_board();

    // Returns the size of the gameboard. For example the default size
    // is 4x4 tiles, so the size is 4.
    int get_size();

    // Fills the gameboard with empty numbertiles
    void fill_empty();

    // Changes the player's score, if two tiles merge. Score is calculated
    // so that the value of the new merged tile is added to the total
    // score.
    void update_score(int new_value);

    // Returns the score.
    int get_score();

    // Sets the score to zero, when reseting the game.
    void set_score_zero();

private:
    // Internal structure of the game board
    std::vector<std::vector<NumberTile*>> board_;

    // Random number generator and distribution,
    // they work better, if they are attributes of a class.
    std::default_random_engine randomEng_;
    std::uniform_int_distribution<int> distribution_;

    int score_ = 0;

};

#endif // GAMEBOARD_HH
