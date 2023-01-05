/*
 COMP.CS.110 Ohjelmointi 2: Rakenteet / Programming 2: Structures
 Project4: 2048
 File: mainwindow.hh
 Header file for the mainwindow.cpp that implements the gui.

 Programmer information:
 -Programmer: Kalle Rantalainen
 -Name: Kalle Rantalainen
 -Student number: 50329870
 -Username: cdkara
 -E-mail: kalle.rantalainen@tuni.fi
*/

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <gameboard.hh>

#include <QMainWindow>
#include <QGraphicsScene>
#include <vector>
#include <QLabel>
#include <QTimer>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // All of the 'clicked' ending functions are for the buttons
    // that the player can push. Use of the buttons are better
    // explained in the functions.
    void on_upPushButton_clicked();

    void on_downPushButton_clicked();

    void on_leftPushButton_clicked();

    void on_rightPushButton_clicked();

    void on_startPushButton_clicked();

    void on_resetPushButton_clicked();

    void on_pausePushButton_clicked();

    // Getting user input for the seed and goal values when the
    // value of the spinbox changes.
    void on_seedSpinBox_valueChanged(int arg1);

    void on_goalSpinBox_valueChanged(int arg1);

    // Is called every second when the program runs, so that we
    // can measure time.
    void interval_function();

private:
    Ui::MainWindow *ui;

    // Scene, on which the gameboard tiles are drawn on.
    QGraphicsScene* scene_;

    // Class gameboard which includes the gameboard mechanics with
    // the numbertile class.
    GameBoard gameboard_;

    // Vector that include every rect on the gameboard.
    std::vector<std::vector<QGraphicsRectItem*>> tiles_;

    // Vector that has corresponding label pointer for every tile
    // on the gameboard.
    std::vector<std::vector<QLabel*>> numbers_;

    // Pointer to rect that displays the gamboard white if the
    // game is in non playable state, exept for pause.
    QGraphicsRectItem* game_disabled_;

    // Pointer to rect that displays the gameboard red if the player loses
    // and green if the player wins
    QGraphicsRectItem* game_has_ended_;

    // The lenth of the gameboard side. If the SIZE variable changes,
    // side_length_ stays the same. This results tiles getting bigger
    // or smaller
    const int side_length_ = 400;

    // Seed for the random engine.
    int seed_ = 1;

    // The value that one numbertile has to reach to win the game.
    int goal_ = 2048;

    // Is used to measure time spent on single game
    QTimer* timer_;

    // Keeping track of the time with variables seconds_ and minutes_
    int seconds_ = 0;
    int minutes_ = 0;

    // Variable that tells if the game is paused or not.
    bool is_paused_ = false;

    // Initializes the board on which all the tiles are
    // drawn later
    void init_gameboard();

    // Initializes the graphical rectangles on gameboard.
    void init_tiles();

    // Draws number on the tiles, depending on the tile value. If the value
    // is zero, then nothing is drawn.
    void draw_numbers();

    // Changes the tile color depending on the tile value.
    void change_color();

    // Changes the numbers drawn on the tiles depending on the tile value.
    void change_numbers();

    // Moves the tiles to the direction where user them wants. Does
    // nothing if no tile can move or merge.
    void move_tiles(std::pair<int, int> dir);

    // Returns color as QBrush according to tile value.
    QBrush pick_a_color(int tile_value);

    // Initializes the labels on which the tile value numbers are drawn.
    void init_numbers();

    // If the game cannot be played, so if user haven't picked a seed or
    // goal yet, draws opaque rectangle on the gameboard.
    void set_game_disabled_rect();

    // Checks if the player has won and ends the game if so.
    void check_win();

    // Checks if the player has lost and ends the game if so.
    void check_lose();

    // Returns the tile values
    std::vector<std::vector<int>> values_of_the_tiles();

    // Updates the score on the gamewindow
    void change_score();

    // Changes the gameboard color to red if player lost,
    // green, if the player won
    void change_background_color(bool has_won);

    // Displays the timer on the gamewindow.
    void display_timer();
};
#endif // MAINWINDOW_HH
