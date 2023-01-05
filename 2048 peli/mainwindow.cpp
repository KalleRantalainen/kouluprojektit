/*
 COMP.CS.110 Ohjelmointi 2: Rakenteet / Programming 2: Structures
 Project4: 2048
 File: mainwindow.cpp

 Has every fuction that is needed to make the graphical interface.
 Most of the game mechanics are done in the gameboard and numbertile
 files but this file uses them and makes the game window.

 Programmer information:
 -Programmer: Kalle Rantalainen
 -Name: Kalle Rantalainen
 -Student number: 50329870
 -Username: cdkara
 -E-mail: kalle.rantalainen@tuni.fi
*/

#include "mainwindow.hh"
#include "ui_mainwindow.h"

#include "gameboard.hh"

#include <QGraphicsRectItem>
#include <vector>
#include <QString>
#include <QLabel>
#include <QColor>
#include <math.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer_ = new QTimer(this);
    connect(timer_, SIGNAL(timeout()), this, SLOT(interval_function()));

    game_disabled_ = new QGraphicsRectItem;

    // Initializing the gameboard and making it empty, so the user
    // can choose seed value and goal.
    init_gameboard();
    init_numbers();
    init_tiles();
    gameboard_.fill_empty();
    change_color();
    change_numbers();
    set_game_disabled_rect();

    // Setting all buttons except start and quit button disabled,
    // so that the player cannot mess anything up with the buttons
    // before choosing seed and goal.
    ui->upPushButton->setDisabled(true);
    ui->downPushButton->setDisabled(true);
    ui->leftPushButton->setDisabled(true);
    ui->rightPushButton->setDisabled(true);

    ui->resetPushButton->setDisabled(true);

    ui->pausePushButton->setDisabled(true);
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::interval_function()
{
    // Updating the variables minutes_ and seconds_ every second
    // and displaying the update on the game window so the player
    // can see the time.
    if (seconds_ < 59)
    {
        seconds_ = seconds_ + 1;
    }
    else
    {
        seconds_ = 0;
        minutes_ += 1;
    }
    display_timer();
}



void MainWindow::init_numbers()
{
    // Initializing labels so we can display numbers on the
    // gameboard tiles to tell the value of a tile.
    int tile_width = side_length_/SIZE;
    for (int y = 0; y < SIZE; y++)
    {
        std::vector<QLabel*> row  ={};

        for (int x = 0; x < SIZE; x++)
        {
            QLabel *number_label = new QLabel(this);
            number_label->setAlignment(Qt::AlignCenter | Qt::AlignCenter);
            number_label->setGeometry(10 + (x * tile_width),
                                      10 + (y * tile_width),
                                      tile_width + 2, tile_width + 2);

            QString value_as_qstring = "";

            number_label->setText(value_as_qstring);

            row.push_back(number_label);
            if (x == SIZE - 1)
            {
                numbers_.push_back(row);
            }
        }
    }
}

void MainWindow::set_game_disabled_rect()
{
    // Drawing translucent white rectangle on the gameboard, so
    // it looks like disabled.
    QColor translucent_white = QColor(255, 255, 255, 100);
    QBrush white_brush(translucent_white);
    QPen black_pen(Qt::black);

    game_disabled_ = scene_->addRect(0, 0, side_length_, side_length_,
                                     black_pen, white_brush);
}


void MainWindow::change_background_color(bool has_won)
{
    // Drawing translucent green or red rectangle on the
    // gameboard to describe winning or losing.
    QColor translucent_red = QColor(255, 150, 150, 100);
    QColor translucent_green = QColor(150, 255, 150, 100);

    if (has_won == true)
    {
        QPen black_pen(Qt::black);
        QBrush brush(translucent_green);
        game_has_ended_ = scene_->addRect(0, 0, side_length_,
                                          side_length_, black_pen, brush);
    }
    else
    {
        QPen black_pen(Qt::black);
        QBrush brush(translucent_red);
        game_has_ended_ = scene_->addRect(0, 0, side_length_,
                                          side_length_, black_pen, brush);
    }
}


void MainWindow::display_timer()
{
    // Displaying time on the game window. Because variables
    // minutes_ and seconds_ are type int, they need to be
    // converted to str -> char -> qstring so that they can
    // be displayed on the window.
    std::string time = "";
    std::string minutes_str = std::to_string(minutes_);
    std::string seconds_str = std::to_string(seconds_);

    // Making the label look like a digital clock, for example
    // if nine seconds has passed the label looks like 00:09.
    if (minutes_ < 10)
    {
        minutes_str = "0" + minutes_str;
    }
    if (seconds_ < 10)
    {
        seconds_str = "0" + seconds_str;
    }

    time = minutes_str + ":" + seconds_str;
    char const *time_as_char = time.c_str();
    QString time_as_qstring = "";
    time_as_qstring.append(time_as_char);

    ui->timerLabel->setText(time_as_qstring);
}


void MainWindow::check_win()
{
    // Cheking if any tile on the gameboard has the same value as
    // the goal_ value. It that is the case, then player has won.
    std::vector<std::vector<NumberTile*>> gameboard = gameboard_.get_board();
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            int tile_value = gameboard.at(i).at(j)->get_value();

            if (tile_value == goal_)
            {
                // Now the player has won, so letting the player
                // know this, timer is stopped, backgroung color
                // is changed and some buttons are disabled that
                // the player cannot move the tile after winning.
                // Player can still start the game again with reset button.
                timer_->stop();

                change_background_color(true);

                QFont win_font("Helvetica", 30);
                ui->winLabel->setText("You win");
                ui->winLabel->setFont(win_font);

                ui->upPushButton->setDisabled(true);
                ui->downPushButton->setDisabled(true);
                ui->leftPushButton->setDisabled(true);
                ui->rightPushButton->setDisabled(true);

                ui->pausePushButton->setDisabled(true);

                return;
            }
        }
    }
}


void MainWindow::check_lose()
{
    // Cheking if all the tiles next to each other have different
    // values and if the gameboard is full. If both are true, then
    // the player has lost.

    std::vector<std::vector<NumberTile*>> gameboard_tiles =
            gameboard_.get_board();

    if (gameboard_.is_full() == true)
    {
        // Gameboard is full of tiles, but we need to
        // check if the tiles can merge or not. If they can
        // merge, then the player has not lost yet, else player
        // loses.

        for (unsigned int i = 0; i < gameboard_tiles.size(); i++)
        {
            for (unsigned int j = 0; j < gameboard_tiles.size(); j++)
            {

                if (j == gameboard_tiles.size() - 1 and
                        i != gameboard_tiles.size() - 1)
                {
                    // Looking at the tile at the end of the row,
                    // so we only need to check if the tile below
                    // is equal to this tile
                    if (gameboard_tiles.at(i).at(j)->get_value() ==
                            gameboard_tiles.at(i + 1).at(j)->get_value())
                    {
                        return;
                    }
                }

                else if (i == gameboard_tiles.size() - 1 and j !=
                         gameboard_tiles.size() - 1)
                {
                    if(gameboard_tiles.at(i).at(j)->get_value() ==
                            gameboard_tiles.at(i).at(j + 1)->get_value())
                    {
                        return;
                    }
                }

                else if (i == gameboard_tiles.size() - 1 and
                         j == gameboard_tiles.size() - 1)
                {
                    // Now we are looking at the bottom right corner, so every
                    // tile and tile next to it has been looked at and no two
                    // equal values were found next to each other,
                    // so player has lost
                    timer_->stop();

                    change_background_color(false);

                    QFont win_font("Helvetica", 30);
                    ui->winLabel->setText("You lose");
                    ui->winLabel->setFont(win_font);

                    ui->upPushButton->setDisabled(true);
                    ui->downPushButton->setDisabled(true);
                    ui->leftPushButton->setDisabled(true);
                    ui->rightPushButton->setDisabled(true);

                    ui->pausePushButton->setDisabled(true);

                    return;

                }

                else if (gameboard_tiles.at(i).at(j)->get_value() ==
                         gameboard_tiles.at(i).at(j + 1)->get_value())
                {
                    // Tile and tile next to it on the same row are equal,
                    // so they can merge and player doesn't lose
                    return;
                }
                else if (gameboard_tiles.at(i).at(j)->get_value() ==
                         gameboard_tiles.at(i + 1).at(j)->get_value())
                {
                    // Tile and tile below it are equal, so they
                    // can merge and the player doesn't lose yet.
                    return;
                }
            }
        }
    }
}


std::vector<std::vector<int>> MainWindow::values_of_the_tiles()
{
    // Getting all of the tile values on the gameboard to corresponding
    // place in vector and returning the vector.
    std::vector<std::vector<NumberTile*>> gameboard = gameboard_.get_board();
    std::vector<std::vector<int>> values;

    for (int i = 0; i < SIZE; i++)
    {
        std::vector<int> row_values = {};
        for (int j = 0; j < SIZE; j++)
        {
            int tile_value = gameboard.at(i).at(j)->get_value();
            row_values.push_back(tile_value);
        }
        values.push_back(row_values);
    }
    return values;
}


void MainWindow::change_score()
{
    // Changing or updating the score that is displayed on the game window.
    int new_score_as_int = gameboard_.get_score();

    std::string score_as_string = std::to_string(new_score_as_int);
    char const *score_char = score_as_string.c_str();

    QString score_as_qstring = "";
    score_as_qstring.append(score_char);

    ui->scoreLabel->setText(score_as_qstring);
}


void MainWindow::init_gameboard()
{
    // Initializing the gameboard rectangle.
    gameboard_.init_empty();
    scene_ = new QGraphicsScene(this);
    int left_margin = 10;
    int top_margin = 10;

    ui->graphicsView->setGeometry(left_margin, top_margin,
                                   side_length_ + 3, side_length_ + 3);
    ui->graphicsView->setScene(scene_);
    scene_->setSceneRect(0, 0, side_length_ - 1, side_length_ - 1);
}


void MainWindow::init_tiles()
{
    // Filling the gameboard with couple randomly placed tiles
    // with values two and the rest of tiles are placed on the
    // gameboard with values 0.
    gameboard_.fill(seed_);
    int tile_amount_on_a_row = gameboard_.get_size();
    int tile_size = side_length_/tile_amount_on_a_row;
    for (int y = 0; y < tile_amount_on_a_row; y++)
    {
        std::vector<QGraphicsRectItem*> row = {};
        for (int x = 0; x < tile_amount_on_a_row; x++)
        {
            QGraphicsRectItem* tile = new QGraphicsRectItem;
            row.push_back(tile);
            if (x == tile_amount_on_a_row - 1)
            {
                tiles_.push_back(row);
            }
        }
    }

    QBrush gray_brush(Qt::gray);
    QPen black_pen(Qt::black);

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            tiles_.at(i).at(j) = scene_->addRect(j*tile_size, i*tile_size,
                                                 tile_size, tile_size,
                                                 black_pen, gray_brush);
        }
    }
}


void MainWindow::change_color()
{
    // Changing the color of a tile with color that is given
    // on some tile value. For example if the tile value is
    // 0, then the tile is gray and if it is two, then the tile
    // is light skin color.
    int tile_amount_on_a_row = gameboard_.get_size();
    int tile_size = side_length_/tile_amount_on_a_row;

    QPen black_pen(Qt::black);

    std::vector<std::vector<NumberTile*>> gameboard = gameboard_.get_board();

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            QBrush color;
            int tile_value = gameboard.at(j).at(i)->get_value();
            // Chenking tile values all the way up to 2048 which is the
            // default goal for the game. If the tile value is higher than that
            // all the higher tiles are the same color.
            if (tile_value == 0)
            {
                color = pick_a_color(0);
                tiles_.at(i).at(j) = scene_->addRect(j*tile_size, i*tile_size,
                                                     tile_size, tile_size,
                                                     black_pen, color);
            }
            else if (tile_value == 2)
            {
                color = pick_a_color(1);
                tiles_.at(i).at(j) = scene_->addRect(j*tile_size, i*tile_size,
                                                     tile_size, tile_size,
                                                     black_pen, color);
            }

            else if (tile_value == 4)
            {
                color = pick_a_color(2);
                tiles_.at(i).at(j) = scene_->addRect(j*tile_size, i*tile_size,
                                                     tile_size, tile_size,
                                                     black_pen, color);
            }

            else if (tile_value == 8)
            {
                color = pick_a_color(3);
                tiles_.at(i).at(j) = scene_->addRect(j*tile_size, i*tile_size,
                                                     tile_size, tile_size,
                                                     black_pen, color);
            }
            else if (tile_value == 16)
            {
                color = pick_a_color(4);
                tiles_.at(i).at(j) = scene_->addRect(j*tile_size, i*tile_size,
                                                     tile_size, tile_size,
                                                     black_pen, color);
            }
            else if (tile_value == 32)
            {
                color = pick_a_color(5);
                tiles_.at(i).at(j) = scene_->addRect(j*tile_size, i*tile_size,
                                                     tile_size, tile_size,
                                                     black_pen, color);
            }
            else if (tile_value == 64)
            {
                color = pick_a_color(6);
                tiles_.at(i).at(j) = scene_->addRect(j*tile_size, i*tile_size,
                                                     tile_size, tile_size,
                                                     black_pen, color);
            }
            else if (tile_value == 128)
            {
                color = pick_a_color(7);
                tiles_.at(i).at(j) = scene_->addRect(j*tile_size, i*tile_size,
                                                     tile_size, tile_size,
                                                     black_pen, color);
            }
            else if (tile_value == 256)
            {
                color = pick_a_color(8);
                tiles_.at(i).at(j) = scene_->addRect(j*tile_size, i*tile_size,
                                                     tile_size, tile_size,
                                                     black_pen, color);
            }
            else if (tile_value == 512)
            {
                color = pick_a_color(9);
                tiles_.at(i).at(j) = scene_->addRect(j*tile_size, i*tile_size,
                                                     tile_size, tile_size,
                                                     black_pen, color);
            }
            else if (tile_value == 1028)
            {
                color = pick_a_color(10);
                tiles_.at(i).at(j) = scene_->addRect(j*tile_size, i*tile_size,
                                                     tile_size, tile_size,
                                                     black_pen, color);
            }
            else if (tile_value == 2048)
            {
                color = pick_a_color(11);
                tiles_.at(i).at(j) = scene_->addRect(j*tile_size, i*tile_size,
                                                     tile_size, tile_size,
                                                     black_pen, color);
            }
            else
            {
                color = pick_a_color(12);
                tiles_.at(i).at(j) = scene_->addRect(j*tile_size, i*tile_size,
                                                     tile_size, tile_size,
                                                     black_pen, color);
            }
        }
    }
}


void MainWindow::change_numbers()
{
    // Displaying the tile values as number on the tiles if the
    // tile value is greater than 0.
    std::vector<std::vector<NumberTile*>> gameboard = gameboard_.get_board();

    for (int y = 0; y < SIZE; y++)
    {
        for (int x = 0; x < SIZE; x++)
        {
            QLabel *number = numbers_.at(y).at(x);
            number->setText("");

            // Converting int to char so that it can be presented as QString
            int tile_value = gameboard.at(x).at(y)->get_value();
            std::string value_as_string = std::to_string(tile_value);
            char const *num_char = value_as_string.c_str();

            QString value_as_qstring = "";
            value_as_qstring.append(num_char);

            if (tile_value != 0)
            {
                number->setText(value_as_qstring);
            }
        }
    }
}

void MainWindow::move_tiles(std::pair<int, int> dir)
{
    // Have to figure out if the values on the tiles are different before
    // moving them and after moving them. If they are different, then new
    // value is added to random empty tile, if not, then nothing happens.

    std::vector<std::vector<QString>> numbers_before;

    for (unsigned int i = 0; i < numbers_.size(); i++)
    {
        std::vector<QString> row = {};
        for (unsigned int j= 0; j < numbers_.size(); j++)
        {
            QString tile_value = numbers_.at(i).at(j)->text();
            row.push_back(tile_value);

            if (j == numbers_.size() - 1)
            {
                numbers_before.push_back(row);
            }
        }
    }

    gameboard_.move(dir, goal_);
    change_color();
    change_numbers();

    for (unsigned int i = 0; i < numbers_.size(); i++)
    {
        for (unsigned int j= 0; j < numbers_.size(); j++)
        {
            if (numbers_before.at(i).at(j) != numbers_.at(i).at(j)->text())
            {
                // Tile values are different and new tile is added
                gameboard_.new_value();
                change_color();
                change_numbers();
                return;
            }
        }
    }

}


QBrush MainWindow::pick_a_color(int tile_value)
{
    // Returns color as QBrush according to the tile value.
    QColor gray_color = QColor(103, 91, 91, 255);
    QBrush brush0(gray_color);

    QColor light_gray_color = QColor(195, 149, 130, 255);
    QBrush brush2(light_gray_color);

    QColor sand_color = QColor(255, 176, 112, 255);
    QBrush brush4(sand_color);

    QColor light_orange = QColor(250, 152, 82, 255);
    QBrush brush8(light_orange);

    QColor bright_orange = QColor(250, 112, 0, 255);
    QBrush brush16(bright_orange);

    QColor orange_red = QColor(224, 70, 27, 255);
    QBrush brush32(orange_red);

    QColor red = QColor(255, 25, 0, 255);
    QBrush brush64(red);

    QColor purple_red = QColor(173, 9, 55, 255);
    QBrush brush128(purple_red);

    QColor purple = QColor(255, 36, 200, 255);
    QBrush brush256(purple);

    QColor bright_purple = QColor(163, 2, 212, 255);
    QBrush brush512(bright_purple);

    QColor blue_pruple = QColor(93, 2, 250, 255);
    QBrush brush1024(blue_pruple);

    QColor bright_blue = QColor(2, 64, 250, 255);
    QBrush brush2048(bright_blue);

    QColor lime_green = QColor(2, 250, 97, 255);
    QBrush brush_over_2048(lime_green);


    std::vector<QBrush> colors = {brush0, brush2, brush4, brush8, brush16,
                                  brush32, brush64, brush128, brush256,
                                  brush512, brush1024, brush2048,
                                  brush_over_2048};
    return colors.at(tile_value);
}



void MainWindow::on_upPushButton_clicked()
{
    // When user presses up button, then the tiles are moved
    // up if possible and merged if possible. Also the score is
    // updated on the window and losing and winning are
    // checked. This happens every time that player presses any of the
    // buttons which move tiles only the direction of the movement varies.
    std::pair dir = {0, -1};
    move_tiles(dir);
    change_score();
    check_win();
    check_lose();
}


void MainWindow::on_downPushButton_clicked()
{
    std::pair dir = {0, 1};
    move_tiles(dir);
    change_score();
    check_win();
    check_lose();
}


void MainWindow::on_leftPushButton_clicked()
{
    std::pair dir = {-1, 0};
    move_tiles(dir);
    change_score();
    check_win();
    check_lose();
}


void MainWindow::on_rightPushButton_clicked()
{
    std::pair dir = {1, 0};
    move_tiles(dir);
    change_score();
    check_win();
    check_lose();
}


void MainWindow::on_seedSpinBox_valueChanged(int arg1)
{
    // Getting user given seed, when the user changes the spinbox
    // value.
    seed_ = arg1;
}


void MainWindow::on_goalSpinBox_valueChanged(int arg1)
{
    // Getting user given goal as exponent of 2. Maximum goal
    // is 30 because 2³¹ results so big number that it cannot
    // be handeled so easily as 2³⁰, also it is very rare that
    // someone could ever reach higher tile values than 2³⁰, even
    // if the gameboard was bigger.
    goal_ = std::pow(2, arg1);
}


void MainWindow::on_startPushButton_clicked()
{
    // Player presses the start button.
    // Timer starts
    timer_->start(1000);

    // The goal and seed inputs are now set disabled so the player can't change
    // those when the game is on.
    ui->seedSpinBox->setDisabled(true);
    ui->goalSpinBox->setDisabled(true);

    // All the buttons which move the tiles are activated.
    ui->upPushButton->setDisabled(false);
    ui->downPushButton->setDisabled(false);
    ui->leftPushButton->setDisabled(false);
    ui->rightPushButton->setDisabled(false);

    // Start button is disabled, so the player has to first reset
    // the game before starting it again.
    ui->startPushButton->setDisabled(true);

    // Reset button is activated, so the player can reset the game
    // at any point during the game.
    ui->resetPushButton->setDisabled(false);

    // Pause is activated, od the player can pause the game at any point.
    ui->pausePushButton->setDisabled(false);

    // Initializing the tiles and updating colors and numbers of the tiles.
    init_tiles();
    change_color();
    change_numbers();
}


void MainWindow::on_resetPushButton_clicked()
{
    // Reset button is pressed.
    // Timer is stopped and the timer is displayed as 00:00
    // on the screen again as it was at the start of the game.
    timer_->stop();
    minutes_ = 0;
    seconds_ = 0;
    display_timer();

    // Spinboxes are activated again so the player can change seed and goal
    // for the new game.
    ui->seedSpinBox->setDisabled(false);
    ui->goalSpinBox->setDisabled(false);

    // Moving buttons are disabled, so the player can't move the tiles
    // before pressing start button.
    ui->upPushButton->setDisabled(true);
    ui->downPushButton->setDisabled(true);
    ui->leftPushButton->setDisabled(true);
    ui->rightPushButton->setDisabled(true);

    // Can start the game again
    ui->startPushButton->setDisabled(false);

    // Cannot reset two times in a row, because it
    // wouldn't do anything.
    ui->resetPushButton->setDisabled(true);

    // Palyer cannot pause the game when the game haven't started yet.
    ui->pausePushButton->setDisabled(true);

    // Labels are zeroed.
    ui->winLabel->setText("");
    ui->scoreLabel->setText("0");

    // Emptying the gameboard, so there is no colored tiles anymore.
    gameboard_.set_score_zero();
    gameboard_.fill_empty();
    change_numbers();
    change_color();

    // Gameboard looks disabled until the game is tarted again.
    set_game_disabled_rect();
}


void MainWindow::on_pausePushButton_clicked()
{
    // When game is paused, the player cannot move the tiles anymore,
    // the clock doesn't run and there is a big text that says "PAUSED on the
    // window. Also the game cannot be reseted when on pause. If the game was
    // not paused when the pause button was pressed, then the 'pause' text on
    // the button changes to 'resume' text and vice versa.
    if (is_paused_ == false)
    {
        timer_->stop();
        ui->pausePushButton->setText("Resume");

        ui->upPushButton->setDisabled(true);
        ui->downPushButton->setDisabled(true);
        ui->leftPushButton->setDisabled(true);
        ui->rightPushButton->setDisabled(true);

        ui->resetPushButton->setDisabled(true);

        QFont font("Helvetica", 30);
        ui->winLabel->setText("PAUSED");
        ui->winLabel->setFont(font);

        is_paused_ = true;
    }
    else
    {
        timer_->start();
        ui->pausePushButton->setText("Pause");

        ui->upPushButton->setDisabled(false);
        ui->downPushButton->setDisabled(false);
        ui->leftPushButton->setDisabled(false);
        ui->rightPushButton->setDisabled(false);

        ui->resetPushButton->setDisabled(false);

        ui->winLabel->setText("");

        is_paused_ = false;
    }
}


