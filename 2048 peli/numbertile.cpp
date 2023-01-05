/*
 COMP.CS.110 Ohjelmointi 2: Rakenteet / Programming 2: Structures
 Project4: 2048
 File: numbertile.cpp
 Implements the numbertile objects of the gameboard. Has all the
 mechanics that single numbertile needs.
 Notes: Most of this file was given for this project but I modified this file
 so that it no longer has ASCII print functions and did small changes.

 Programmer information:
 -Programmer: Kalle Rantalainen
 -Name: Kalle Rantalainen
 -Student number: 50329870
 -Username: cdkara
 -E-mail: kalle.rantalainen@tuni.fi
*/

#include "numbertile.hh"
#include "gameboard.hh"
#include <iomanip>

Coords operator+(Coords lhs, Coords rhs)
{
    return std::make_pair(lhs.first + rhs.first, lhs.second + rhs.second);
}

NumberTile::NumberTile(int value, Coords coords, GameBoard* board):
    value_(value), coords_(coords), board_(board), is_merged_(false)
{
}

NumberTile::~NumberTile()
{
}

bool NumberTile::move(Coords direction, const int goal_val)
{
    Coords curr_loc = coords_;
    Coords new_loc = coords_ + direction;
    while( is_on_board( new_loc ) )
    {
        NumberTile* curr = board_->get_item(curr_loc);
        NumberTile* dest = board_->get_item(new_loc);
        if( dest->value_ == 0 )
        {
            dest->value_ = curr->value_;
            dest->is_merged_ = curr->is_merged_;
            curr->value_ = 0;
            curr->is_merged_ = false;
        }
        else if( dest->value_ == curr->value_ and
                 not dest->is_merged_ and
                 not curr->is_merged_ )
        {
            dest->value_ = 2 * curr->value_;
            curr->value_ = 0;
            dest->is_merged_ = true;

            board_->update_score(dest->value_);

            return dest->value_ == goal_val;
        }
        curr_loc = new_loc;
        new_loc = dest->coords_ + direction;
    }
    return false;
}

bool NumberTile::new_value(int new_val)
{
    if( value_ == 0 )
    {
        value_ = new_val;
        return true;
    }
    return false;
}

bool NumberTile::is_empty()
{
    return value_ == 0;
}

void NumberTile::reset_turn()
{
    is_merged_ = false;
}

int NumberTile::get_value()
{
    return value_;
}

bool NumberTile::is_on_board(Coords coords)
{
    return coords.first >= 0 and coords.first < board_->get_size() and
           coords.second >= 0 and coords.second < board_->get_size();
}
