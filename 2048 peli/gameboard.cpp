/*
 COMP.CS.110 Ohjelmointi 2: Rakenteet / Programming 2: Structures
 Project4: 2048
 File: gameboard.cpp

 Implements the gameboard which can handle the numbertile objects. Includes
 most of the game mechanics together with the numbertile file.

 Notes: Most of this file was given for this project but I modified it
 so that this file doesn't have the ASCII print functions and has some new
 functions and small changes.

 Programmer information:
 -Programmer: Kalle Rantalainen
 -Name: Kalle Rantalainen
 -Student number: 50329870
 -Username: cdkara
 -E-mail: kalle.rantalainen@tuni.fi
*/

#include "gameboard.hh"

GameBoard::GameBoard()
{
}

GameBoard::~GameBoard()
{
    for(unsigned int y = 0; y < board_.size(); ++y)
    {
        for(unsigned int x = 0; x < board_.at(y).size(); ++x)
        {
            delete board_.at(x).at(y);
            board_.at(x).at(y) = nullptr;
        }
    }
}

void GameBoard::init_empty()
{
    std::vector<NumberTile*> row;
    for( int i = 0; i < SIZE; ++i)
    {
        row.push_back(nullptr);
    }
    for( int i = 0; i < SIZE; ++i)
    {
        board_.push_back(row);
    }
}

void GameBoard::fill(int seed)
{
    randomEng_.seed(seed);
    distribution_ = std::uniform_int_distribution<int>(0, SIZE - 1);

    // Wiping out the first random number (which is almost almost 0)
    distribution_(randomEng_);

    for( auto y = 0; y < SIZE; ++y )
    {
        for( auto x = 0; x < SIZE; ++x )
        {
            board_.at(y).at(x) = new NumberTile(0, std::make_pair(y, x), this);
        }
    }

    for( int i = 0 ; i < SIZE; ++i )
    {
        new_value();
    }
}

void GameBoard::new_value(bool check_if_empty)
{
    if( check_if_empty and is_full() ){
        // So that we will not be stuck in a forever loop
        return;
    }
    int random_x = 0;
    int random_y = 0;
    do
    {
        random_x = distribution_(randomEng_);
        random_y = distribution_(randomEng_);
    } while( not board_.at(random_y).at(random_x)->new_value(NEW_VALUE) );
}


bool GameBoard::move(Coords dir, int goal)
{
    bool has_won = false;
    for( unsigned y = 0; y < board_.size(); ++y )
    {
        for( unsigned x = 0; x < board_.front().size(); ++x )
        {
            int directed_y = dir.first > 0 ? board_.size() - y - 1 : y;
            int directed_x = dir.second > 0 ? board_.back().size() - x - 1 : x;
            if( board_.at(directed_y).at(directed_x)->move(dir, goal) )
            {
                has_won = true;
            }
        }
    }
    for( auto &row : board_ )
    {
        for( auto &tile : row )
        {
            tile->reset_turn();
        }
    }
    return has_won;
}

NumberTile* GameBoard::get_item(Coords coords)
{
    return board_.at(coords.first).at(coords.second);
}

int GameBoard::get_size()
{
    return SIZE;
}


void GameBoard::fill_empty()
{
    for( auto y = 0; y < SIZE; ++y )
    {
        for( auto x = 0; x < SIZE; ++x )
        {
            board_.at(y).at(x) = new NumberTile(0, std::make_pair(y, x), this);
        }
    }
}

void GameBoard::update_score(int new_value)
{
    score_ += new_value;
}

int GameBoard::get_score()
{
    return score_;
}

void GameBoard::set_score_zero()
{
    score_ = 0;
}


std::vector<std::vector<NumberTile *> > GameBoard::get_board()
{
    return board_;
}

bool GameBoard::is_full() const
{
    for( auto y : board_ )
    {
        for( auto x : y )
        {
            if( x->is_empty() )
            {
                return false;
            }
        }
    }
    return true;
}
