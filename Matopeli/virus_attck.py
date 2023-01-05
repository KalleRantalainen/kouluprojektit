"""
COMP.CS.100 Ohjelmointi 1: Graafinen käyttöliittymä projekti
Nimi/Name: Kalle Rantalainen
Opiskelijanumero: 50329870

Kehittynyt käyttöliittymä / Advanced gui

This program is version of classic snake game.
When the program is strated the user will be
in the main menu of the program. User has three
choises: user can play the game by pressing
play button, look at the previous scores in
highscores window by pressing highscores button or
look at the introduction/help window by
pressing help button. User can go back to main
menu by pressing button in the left corner of
the screen, or if user is playing the game
he/she will be sent back to main menu, if he/she
dies.
In the game player is antivirus program which is
represented as 'snake' or 'caterpillar'. The idea
is to eat as many viruses, which are represented
as spider icons on the screen, as the player can.
Snake starts as 3 block long, and every time player
manages to eat a virus the lenght of the snake is
increased by one block, and new virus spawns at a
random location on the screen. If player tries to go
outside of the screen, the snake dies and player loses.
Player also loses in the case if the snake bumps in
to it's tail. So the game works similarly to the classic
snake game, but instead of eating apples with a
snake, the player has to eat viruses with a 'antivirus worm'.

About controls: Snakes direction is controlled with
keys w, a, s, d. Snake is constantly moving, but its
direction can be changed with key press. Also mentioned in the
'help' window of the program.
"""

from tkinter import *
import random
import time

# Canvas width and height is defined
# as global variables since they are used
# throughout the code and their values
# don't change at any point. These two variables
# defines the size of the game, main_menu, help and
# highscores canvases.
CANVAS_WIDTH = 550
CANVAS_HEIGHT = 550

# Variable CELL_SIZE is defined as global
# variable, since it's used througout the code.
# The game window can be thought as a grid where
# each cell of the grid is 50 pixels high and
# 50 pixels wide. Game window is 550 wide and
# high so it's built from 11x11 cells. If for
# example new virus (food for the snake) location
# needs to be calculated, it's easy to take number
# for x-coordinate from 0 to 11 and for t-coordinate
# from 0 to 11 and multply those coordinates by
# CELL_SIZE. That way we can access to every cell in
# the game window.
CELL_SIZE = 50

# WINDOW is defined as global variable since
# it's the main window and it doesen't change
# at any point of the code. WINDOW variable is also used
# in the code when something is drawn on the window, so
# it's easier that WINDOW is global variable.
WINDOW = Tk()

# Window geometry is set to 570x570 pixels,
# window's ability to change size is blocked and
# title is set for the window.
WINDOW.geometry(f"570x570")
WINDOW.resizable(False, False)
WINDOW.title("Virus Attck version 3000")

# CANVAS is defined as global variable, because
# the main game uses this canvas. Since the game is built
# from three different classes, it's easier to define game
# canvas as a global variable, as all these classes
# uses this canvas.
CANVAS = Canvas(WINDOW, height=CANVAS_HEIGHT, width=CANVAS_WIDTH)

# Global variable LAST_PRESS has time when key
# was last time pressed. First it's value is
# zero because no key is jet pressed. After key
# is pressed LAST_PRESSes value will be updated
# to the time key was pressed.
LAST_PRESS = 0


class Snake():
    """
    Makes the snake character of the game. Snake
    can have direction and every block of the snake
    has position. Direction of the snake can be
    changed with pressing certain keys.
    """

    def __init__(self):
        """
        Constructor, creates character snake and
        gives snake coordinates for the Main_game
        calss. Adds new coordinates in the
        __coordinates list and so keeps track
        of the snake position.
        """

        self.__canvas = CANVAS
        self.__body_lenght = 3
        self.__coordinates = []
        self.__blocks = []

        # Adds starting coordinates to snake
        # coordinates list
        for i in range(0, 3):
            self.__coordinates.append([0, 0])

        self.__body_right_img = PhotoImage(file="snake_body_right.png")

        # draws the snakes starting position on the screen
        for x, y in self.__coordinates:
            block = self.__canvas.create_image(x + 28, y + 28,
                                               image=self.__body_right_img)
            self.__blocks.append(block)

    def get_head_coordinates(self):
        """
        Method, that returns coordinates of
        the snakes head
        :return: Int, snakes head position
        """
        return self.__coordinates[0]

    def insert_new_coordinates(self, x, y):
        """
        Inserts new head coordinates when
        snake moves.
        :param x: Int, head x coordinate
        :param y: Int, head y coordinate
        """
        self.__coordinates.insert(0, (x, y))

    def insert_new_block(self, block):
        """
        Inserts new head block to the __blocks list
        every time snake moves.
        :param block: Graphics of the snake block
        """
        self.__blocks.insert(0, block)

    def get_coordinates(self):
        """
        Returns list of every coordinate of
        every block.
        :return: Coordinate list
        """
        return self.__coordinates

    def delete_last_coordinates(self):
        """
        Deletes coordinates of the last block
        of the snake every time the snake moves,
        except when snake eats a virus. This makes the
        snake grow one block longer every time a virus is
        eaten.
        """
        self.__coordinates.pop(-1)

    def delete_last_block(self):
        """
        Deletes the last block of the snake from
        __blocks list every time the snake moves, except
        when the snake eats a virus. This makes the
        snake grow one block longer every time a virus is
        eaten.
        """
        self.__blocks.pop(-1)

    def get_last_block(self):
        """
        Returns the last blocks graphics from the __blocks
        list. This method is needed when last block of
        the snake graphics on the screen needs to be removed,
        so every time snake moves except, when snake eats a virus.
        :return: Last block of the snake
        """
        return self.__blocks[-1]

    def check_death(self):
        """
        This mehtod checks if the player have lost
        the game. Player loses when the snake runs to
        its tail or when the snake tries to exit the game
        window.
        :return: Returns True, if player has lost
        """
        head_x, head_y = self.__coordinates[0]
        if head_x < 0 or head_y < 0:
            return True
        elif head_x > 500 or head_y > 500:
            return True
        for body_x, body_y in self.__coordinates[1:]:
            if body_x == head_x and body_y == head_y:
                return True


class Virus():
    """
    Makes character virus. Virus is a food
    for snake and player has no control over the virus.
    Virus has a position and new position is randomly generated
    every time snake eats the virus.
    """

    def __init__(self):
        """
        Constructor of the virus.
        """
        self.__canvas = CANVAS

        self.__virus_image = PhotoImage(file="virus_image.png")

        self.__x_pos = random.randint(1, 10) * CELL_SIZE
        self.__y_pos = random.randint(0, 10) * CELL_SIZE

        self.__coordinates = [self.__x_pos, self.__y_pos]

        # When drawing the virus on the screen
        # 28 is added to self.__x_pos and self.__y_pos
        # to center the virus image on the cell.
        self.__virus = self.__canvas.create_image(self.__x_pos + 28,
                                                  self.__y_pos + 28,
                                                  image=self.__virus_image)

    def get_virus_coordinates(self):
        """
        Returns coordinates of the virus.
        :return: Virus coordinates
        """
        return self.__x_pos, self.__y_pos

    def make_new_virus(self, snake_coordinates):
        """
        Makes a new virus, when the old one
        is eaten. Genrates new random coordinates
        for the new virus and places it on the screen.
        If the new virus coordinates are same as any
        snake block coordinate, generates new coordinates.
        That way new virus should newer generate on top
        of the snake.
        :param snake_coordinates: List of every snake
        block coordinates.
        """

        self.__x_pos = random.randint(0, 10) * CELL_SIZE
        self.__y_pos = random.randint(0, 10) * CELL_SIZE

        for snake_x, snake_y in snake_coordinates:

            if snake_x == self.__x_pos and snake_y == self.__y_pos:
                # If new coordinates of the virus is on top of
                # any block of the snake, new coordinates are
                # generated.
                self.make_new_virus(snake_coordinates)

            elif self.__x_pos == 0 and self.__y_pos == 0:
                # If new coordinates of the virus is 0,0 the virus is
                # under the score label. If score is 2 digit
                # number, the virus will be very hard to see.
                # This elif statement will prevent this
                # from happening.
                self.make_new_virus(snake_coordinates)

            else:
                # If new coordinates passes previous if- statements
                # old virus image is deleted and new one is placed
                # on the generated coordinates.
                self.__canvas.delete(self.__virus)
                self.__virus = self.__canvas.create_image(self.__x_pos + 28,
                                                          self.__y_pos + 28,
                                                          image=self.__virus_image)


class Main_menu():
    """
    This class creates main menu, where player
    can choose what to do. When the program is
    started main menu opens up.
    """

    def __init__(self):
        """
        Constructor, creates the main menu
        and buttons on it.
        """

        # Creating menu canvas
        self.__menu_canvas = Canvas(WINDOW, height=CANVAS_HEIGHT,
                                    width=CANVAS_WIDTH)
        self.__menu_canvas.place(x=8, y=0)
        self.__menu_image = PhotoImage(file="paavalikkotausta.png")
        self.__menu_canvas.create_image(277, 277, image=self.__menu_image)

        # Creating play button
        self.__play_button_img = PhotoImage(file="play_button_image.png")
        self.__play_button = Button(self.__menu_canvas,
                                    image=self.__play_button_img, borderwidt=0,
                                    command=self.start_game)
        self.__play_button.place(x=275, y=300, anchor=CENTER)

        # Creating highscores button
        self.__highscores_button_img = PhotoImage(file="highscores_button_image.png")
        self.__highscores_button = Button(self.__menu_canvas,
                                          image=self.__highscores_button_img,
                                          borderwidth=0,
                                          command=self.highscores)
        self.__highscores_button.place(x=275, y=375, anchor=CENTER)

        # Creating help button
        self.__help_button_img = PhotoImage(file="help_button_img.png")
        self.__help_button = Button(self.__menu_canvas,
                                    image=self.__help_button_img,
                                    borderwidth=0,
                                    command=self.help)
        self.__help_button.place(x=275, y=450, anchor=CENTER)

    def start_game(self):
        """
        When player presses button 'play' this
        method is called. Destroys menu canvas and
        buttons on it, because the buttons are placed
        on menu canvas. After destroying menu canvas,
        calls for Main_game class and game starts.
        """
        self.__menu_canvas.destroy()
        Main_game()

    def help(self):
        """
        When player presses button 'help' this
        method is called. Destroys menu canvas and
        makes a new help canvas, and button on it
        to get back to the main menu.
        """
        self.__menu_canvas.destroy()

        self.__help_background_image = PhotoImage(file="help_background_oikea2.png")
        self.__help_canvas = Canvas(WINDOW, height=CANVAS_HEIGHT, width=CANVAS_WIDTH)
        self.__help_canvas.place(x=8, y=0)
        self.__help_canvas.create_image(278, 277, image=self.__help_background_image)

        self.__back_button_img = PhotoImage(file="takaisinnappi.png")
        self.__back_button = Button(self.__help_canvas,
                                    image=self.__back_button_img,
                                    borderwidth=0,
                                    command=self.destroy_help)
        self.__back_button.place(x=0, y=0)

    def highscores(self):
        """
        When player presses button 'highscores' this
        method is called. Destroys menu canvas and
        makes a new highscores canvas, and button on it
        to get back to the main menu. Also reads highscores
        from method read highscores and draws the highscores
        on the screen as labels.
        """
        self.__menu_canvas.destroy()

        self.__highscores_canvas = Canvas(WINDOW, bg="red", height=CANVAS_HEIGHT,
                                          width=CANVAS_WIDTH)
        self.__highscores_canvas.place(x=8, y=0)
        self.__highscore_img = PhotoImage(file="highscorestausta.png")
        self.__highscores_canvas.create_image(278, 277, image=self.__highscore_img)

        self.__back_button_img = PhotoImage(file="takaisinnappi.png")
        self.__back_button = Button(self.__highscores_canvas,
                                    image=self.__back_button_img,
                                    borderwidth=0,
                                    command=self.destroy_highscores)
        self.__back_button.place(x=0, y=0)

        highscores_list = self.read_highscores()
        self.__first_score = Label(self.__highscores_canvas,
                                   text=f"1. {highscores_list[0]}",
                                   height=1, width=4, font=("Helvetica", 25))
        self.__second_score = Label(self.__highscores_canvas,
                                    text=f"2. {highscores_list[1]}",
                                    height=1, width=4, font=("Helvetica", 25))
        self.__third_score = Label(self.__highscores_canvas,
                                   text=f"3. {highscores_list[2]}",
                                   height=1, width=4, font=("Helvetica", 25))
        self.__first_score.place(x=275, y=300, anchor=CENTER)
        self.__second_score.place(x=275, y=375, anchor=CENTER)
        self.__third_score.place(x=275, y=450, anchor=CENTER)

    def read_highscores(self):
        """
        Reads highscores from a file 'highscores.txt'.
        :return: list of highscores
        """
        file_name = "highscores.txt"
        highscores_list = []

        try:
            highscores_file = open(file=file_name, mode="r")

            for line in highscores_file:
                line = line.strip("\n")
                highscores_list.append(line)

            highscores_file.close()
        except OSError:
            print(f"Somebody deleted or renamed file {file_name}")
            print("and now you can't see the highscores.")

        return highscores_list

    def destroy_help(self):
        """
        This method is called when player is
        in help window and presses back button.
        destroys help canvas and calls for
        Main_menu class to get back to the main
        menu.
        """
        self.__help_canvas.destroy()
        Main_menu()

    def destroy_highscores(self):
        """
        This method is called when player is
        in highscores window and presses back button.
        destroys highscores canvas and calls for
        Main_menu class to get back to the main
        menu.
        """
        self.__highscores_canvas.destroy()
        Main_menu()


class Main_game():
    """
    This class is called when the
    player hits 'play' button. This
    class runs the game and 'gameloop'
    happens inside this class.
    """

    def __init__(self):
        """
        Constructor, defines all variables needed
        to start the game. Constructor runs once
        every time 'play' button is pressed and the game
        starts.
        """
        # Defines the starting direction to be right
        # so the snake moves right when the game starts.
        # This variable can be changed with pressing
        # keys w, a, s or d.
        self.__direction = "right"

        # Depending which direction the snake is going
        # different image will be drawn as a body block
        # snake can go 4 different directions, thus 4
        # different images needed.
        self.__body_down_img = PhotoImage(file="snake_body_down.png")
        self.__body_up_img = PhotoImage(file="snake_body_up.png")
        self.__body_right_img = PhotoImage(file="snake_body_right.png")
        self.__body_left_img = PhotoImage(file="snake_body_left.png")

        self.__canvas = CANVAS

        # Creates score label in the upper left corner
        # of the game window. Text of this label a.k.a score
        # will be updated every time virus is eaten.
        self.__score = 0
        self.__score_label = Label(WINDOW, text=f"{self.__score}",
                                   font=("Helvetica", 30))
        self.__score_label.place(x=8, y=0)

        self.__canvas.pack()
        self.__game_background_image = PhotoImage(file="game_background.png")
        self.__canvas.create_image(277, 277, image=self.__game_background_image)

        self.__snake = Snake()
        self.__virus = Virus()

        # After initializing the game window
        # method game_loop is called.
        self.game_loop(self.__snake, self.__virus)

    def game_loop(self, snake, virus):
        """
        The game runs inside this method.
        This method is gameloop and it runs every 150ms
        until the player dies. So the game window is
        updated every 150 milliseconds so the snake moves
        about 6,7 cells per second. This also
        results to fps of 6,7. Game loop speed could be adjusted
        by changing game_speed variable. This would result
        higher fps but also higher snake speed. Game_speed
        150 is good in my opinion, it doesn't make the game
        speed too fast but it isn't too easy either. Game speeds
        under 75 are unplayble because the snake gets too fast
        in my opinion.
        :param snake: snake object
        :param virus: virus object
        """

        game_speed = 150

        head_x, head_y = snake.get_head_coordinates()

        if self.__direction == "up":
            # If variable self.__direction is
            # up, cell size will be subtracted
            # from snakes head location on y axis and
            # new snake head block will be
            # drawn on these new coordinates
            # every time game loop runs. This
            # results new head block to be placed
            # one cell up from the last snake head
            # coordinates thus snake goes up.
            head_y -= CELL_SIZE
            block = self.__canvas.create_image(head_x + 28, head_y + 28,
                                               image=self.__body_up_img)

        elif self.__direction == "down":
            # If variable self.__direction is
            # down, cell size will be added
            # to snakes head location on y axis and
            # new snake head block will be
            # drawn on these new coordinates
            # every time game loop runs.
            head_y += CELL_SIZE
            block = self.__canvas.create_image(head_x + 28, head_y + 28,
                                               image=self.__body_down_img)

        elif self.__direction == "left":
            # If variable self.__direction is
            # left, cell size will be subtracted
            # from snakes head location on the x axis and
            # new snake head block will be
            # drawn on these new coordinates
            # every time game loop runs.
            head_x -= CELL_SIZE
            block = self.__canvas.create_image(head_x + 28, head_y + 28,
                                               image=self.__body_left_img)

        elif self.__direction == "right":
            # If variable self.__direction is
            # right, cell size will be added
            # to snakes head location on the x axis and
            # new snake head block will be
            # drawn on these new coordinates
            # every time game loop runs.
            head_x += CELL_SIZE
            block = self.__canvas.create_image(head_x + 28, head_y + 28,
                                               image=self.__body_right_img)

        # Inserting new head coordinates
        # to the __coordinates list in
        # the Snake class
        snake.insert_new_coordinates(head_x, head_y)

        # Inserting new head block to the
        # __blocks list in the Snake class
        snake.insert_new_block(block)

        # Calls method detect keys function
        # which results to keyboard key presses
        # being detected.
        self.detect_keys()

        virus_x, virus_y = virus.get_virus_coordinates()
        snake_block_coordinates = snake.get_coordinates()

        if head_x == virus_x and head_y == virus_y:
            # If snake head location is the same as
            # virus location, this results virus being
            # 'eated' or destroyed and to snake getting
            # longer by one block. Score label is also
            # updated when virus is eaten. If virus is
            # eaten, last body block will not be deleted
            # this time in the game loop. This results
            # snake getting bigger by one block.
            virus.make_new_virus(snake_block_coordinates)
            self.__score += 1
            self.__score_label.config(text="{}".format(self.__score))

        else:
            # If virus is not eaten this time
            # in the game loop, last block of the
            # snake will be deleted from the canvas.
            # This results snake moving and not
            # getting infinitely long
            snake.delete_last_coordinates()
            self.__canvas.delete(snake.get_last_block())
            snake.delete_last_block()

        # Variable game_over calls for
        # check_death method in the snake
        # class. Game_over has two values,
        # True nad False.
        game_over = snake.check_death()

        if game_over == True:
            # If snake runned into its own
            # tail or tried to get out of the screen,
            # game_over got value True. Time.sleep(1)
            # waits one second before it calls function
            # write_new_highscore and method game_over so that
            # player can watch how long his snake was before
            # the inevitable death.
            time.sleep(1)
            write_new_highscore(self.__score)
            self.game_over()
        else:
            # If snake is still alive game_over
            # has value False, and game_loop method
            # will be called again in time that is defined
            # in the game_speed variable, so in 150
            # milliseconds. This results game_loop looping
            # as long as snake is alive and well, and game
            # keeps running.
            WINDOW.after(game_speed, self.game_loop, snake, virus)

    def detect_keys(self):
        """
        This method binds the keyboard keys
        and detects when any key is pressed on the
        keyboard. If key is pressed, the information
        of the key press will be sent to key_pressed
        function.
        """
        WINDOW.bind("<Key>", self.key_pressed)

    def key_pressed(self, event):
        """
        This method changes variable __direction
        based on what key is pressed. Method also has
        delay between key presses. So variable key_press_delay
        determines the least amount of time that has
        to pass before user can change the snakes direction
        again. This should prevent for example situation
        where snake is going up and player rapidly presses
        a and right after that s. This would result the
        __direction parameter to change to left and
        right after that to down. Snake wouldn't have time
        to turn left but it would straigth away change
        direction to down. Since the snake was going up
        at the beginning this would result to snake colliding
        with itself and player losing without visibly hitting
        anything.
        :param event: Keyboard key
        :return: Updated __direction
        """
        global LAST_PRESS

        # Delay between keypresses is set to 0.11
        # I tried to find delay time the way that player
        # can still do U turns but can't make the snake
        # collide with itself.
        key_press_delay = 0.11

        # Time_elapsed variable keeps track of
        # the time elapsed. With the help of global
        # variable which has the time of the last press
        # and time_elapsed variable, we can detrmine
        # the time between last and ongoing keypress.
        time_elapsed = time.time()

        if event.char == "w" and time_elapsed - LAST_PRESS > key_press_delay:
            # If pressed key is 'w' snakes direction
            # sould be changed to up. However if the
            # snake is already going down we can't
            # change the direction to up, or otherwise
            # the snake would collide with itself.
            LAST_PRESS = time_elapsed
            if self.__direction == "down":
                self.__direction = "down"
            else:
                self.__direction = "up"

        elif event.char == "s" and time_elapsed - LAST_PRESS > key_press_delay:
            # If pressed key is 's' snakes direction
            # sould be changed to down. However if the
            # snake is already going up we can't
            # change the direction to down, or otherwise
            # the snake would collide with itself.
            LAST_PRESS = time_elapsed
            if self.__direction == "up":
                self.__direction = "up"
            else:
                self.__direction = "down"

        elif event.char == "a" and time_elapsed - LAST_PRESS > key_press_delay:
            # If pressed key is 'a' snakes direction
            # sould be changed to left. However if the
            # snake is already going right we can't
            # change the direction to left, or otherwise
            # the snake would collide with itself.
            LAST_PRESS = time_elapsed
            if self.__direction == "right":
                self.__direction = "right"
            else:
                self.__direction = "left"

        elif event.char == "d" and time_elapsed - LAST_PRESS > key_press_delay:
            # If pressed key is 'd' snakes direction
            # sould be changed to right. However if the
            # snake is already going left we can't
            # change the direction to right, or otherwise
            # the snake would collide with itself.
            LAST_PRESS = time_elapsed
            if self.__direction == "left":
                self.__direction = "left"
            else:
                self.__direction = "right"

    def game_over(self):
        """
        This method is called when game_over variable
        in the game_loop gets value True. This method
        destroy the score label and deletes everything from
        the game canvas. After that Main_menu class is called
        and player is sent back to main menu.
        :return:
        """
        self.__score_label.destroy()
        self.__canvas.delete("all")
        Main_menu()


def write_new_highscore(new_score):
    """
    This function reads file 'highscore.txt'.
    The file should contain 3 highscores from
    previous games. These highscores are saved
    into highscores_list and the file is closed.
    Next the funcion compares the new_score with
    the old highscores and places the new score
    in the highscores_list, if the new_score is big
    enough. The highscores_list can only have three
    scores. For example if the new_score is bigger that
    the best score in highscores_list the new score
    will be added to the top (index 0) on the list.
    The old best score will then be moved to the second
    place and the old second best score will now
    be the third score. The old third score will be deleted.
    :param new_score: Score that player got from the game,
    thus how many viruses player ate.
    """
    file_name = "highscores.txt"
    try:
        highscores_file = open(file=file_name, mode="r")
        highscores_list = []
        for line in highscores_file:
            line = line.strip()
            highscores_list.append(line)
        highscores_file.close()

    except OSError:
        # These errors should never happen because
        # game folder should contain highscores.txt
        # file
        print(f"Somebody deleted or renamed file {file_name}")
        print("and now you can't see the highscores.")

    i = 0
    for old_score in highscores_list:
        if int(old_score) < int(new_score):
            highscores_list.insert(i, new_score)
            highscores_list.pop(-1)
            break
        i += 1

    try:
        highscores_file = open(file=file_name, mode="w")
        for new_scores in highscores_list:
            print(new_scores, file=highscores_file)
        highscores_file.close()

    except OSError:
        print(f"Somebody deleted or renamed file {file_name}")
        print("and now you can't see the highscores.")


def main():
    Main_menu()
    WINDOW.mainloop()


if __name__ == '__main__':
    main()