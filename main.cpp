/*
Welcome to Minesweeper, coded in C++ on Visual Studio 2019! Sorry, the code is a little
messy; hopefully I explained it enough.

For AP CS Principles grading, block comments and line comments on their own line are my explanations of
what the following lines of code do. Line comments on a line of code with "!!!!" at the beginning indicate
where I believe I fulfilled one of the submission requirements.

My program includes several external libraries developed by other people, most noteably SFML,
which is use for the GUI (all credit for the SMFL library goes to the devs at https://www.sfml-dev.org/).
All five of the SFML libraries and includes (audio, graphics, network, system, and window) are added to
the project. The program  utilizes other external classes and methods for RNG, the timer, input and
output, and strings (credit to the standard C++ libraries and anywhere else those classes/methods came
from). I also add a font file (coure.fon) so that I could display text on the SFML window. The font was
downloaded from https://fontsgeek.com.
*/
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <string>
#include <cstdio>
#include <ctime>

/* These declarations are for the constants used throughout the program and the multidimensional arrays
that hold the majority of the data for the program. The arrays of 400 RectangleShapes and 400 Texts had
to be put on the heap to avoid stack overflow. There is also a boolean variable that monitors whether the
game has started yet, as the first click is unique. */
const int DIMENSION = 20;
const int NUM_MINES = ((DIMENSION * DIMENSION) / 4) - 1;
sf::RectangleShape* board[DIMENSION][DIMENSION]; //!!!! List representing collection of data for managing program complexity
sf::Text* numbers[DIMENSION][DIMENSION];
int minePositions[NUM_MINES][2];
bool firstClick, stop;

/* The method is pretty self-explanatory. I wanted to use this idea in an if conditionals, and it would've
been very clunky without a method that returned a boolean value. */
bool isMine(int x, int y) {
    for (int i = 0; i < NUM_MINES; i++) {
        if (x == minePositions[i][0] && y == minePositions[i][1]) {
            return true;
        }
    }
    return false;
}

/* This is a void method that I just use to bundle related, reused code together. It incorporates all the
random number generation. It's purpose is to assign NUM_MINES mines to unique map positions. There are
NUM_MINES int arrays of length two that hold the x and y position of the mines. The first for loops
iterates for the number of mines, the while loop assigns random coordinates to the given mine, and the
second for loop checks to see if that position has been assigned a mine already and iterates the while
again if it has. */
void assignMines() {//!!!! Algorithm with sequencing, selection, and iteration (called on line 191 in the main method).
    std::random_device device;
    std::mt19937 rng(device());
    std::uniform_int_distribution<int> distribution(0, DIMENSION - 1);
    for (int i = 0; i < NUM_MINES; i++) {
        bool loop = true;
        while (loop) {
            loop = false;
            minePositions[i][0] = distribution(rng);
            minePositions[i][1] = distribution(rng);
            for (int j = i - 1; j >= 0; j--) {
                if (minePositions[i][0] == minePositions[j][0] && minePositions[i][1] == minePositions[j][1]) {
                    loop = true;
                }
            }
        }
    }
}

/* Another case of a void method making code easier to repeat. This method assigns the numbers to each
square by checking the number of mines around it and setting the corresponding Text object's string to
that number. The first two for loops are to iterate through the 2D arrays and the second two are to
iterate through the surrounding squares. This is place where isMine() is used. */
void assignNumbers() {
    for (int i = 0; i < DIMENSION; i++) {
        for (int j = 0; j < DIMENSION; j++) {
            int surroundingMines = 0;
            for (int a = i - 1; a <= i + 1; a++) {
                if (a < 0) a = 0;
                if (a >= DIMENSION) break;
                for (int b = j - 1; b <= j + 1; b++) {
                    if (b < 0) b = 0;
                    if (b >= DIMENSION) break;
                    if (isMine(a, b)) surroundingMines++;
                }
            }
            if ((*numbers[i][j]).getString() != "M") {
                (*numbers[i][j]).setString(std::to_string(surroundingMines));
            }
        }
    }
}

/* reveal() is called when a square is left-clicked. It is passed the indices of the square and then,
using some convuluted logic based on the fill colors of the RectangleShape objects and the strings of the
Text objects, it determines how to reveal what that square is. The loops are for the case of the squares
with no mines around them. They check all the squares around the empty one to reveal them automatically
(since there can't be any mines on them, it saves the user time). I used recursion so that if there were
multiple adjacent empty squares, this process could happen for all of them. */
void reveal(int i, int j) {//!!!! Procedure that contributes to intended purpose with defined name, return type, and parameters
    if ((*board[i][j]).getFillColor() == sf::Color(255, 255, 255)) {
        if ((*numbers[i][j]).getString() == "0") {
            (*board[i][j]).setFillColor(sf::Color(100, 100, 100));
            for (int a = i - 1; a <= i + 1; a++) {
                if (a >= DIMENSION) break;
                if (a < 0) a = 0;
                for (int b = j - 1; b <= j + 1; b++) {
                    if (b >= DIMENSION) break;
                    if (b < 0) b = 0;
                    reveal(a, b);
                }
            }
        }
        else {
            (*numbers[i][j]).setFillColor(sf::Color(0, 0, 0));
            (*board[i][j]).setFillColor(sf::Color(200, 200, 200));
        }
    }
    
}

int main() {
    /* The SFML window's size is based on the size of the squares (41 pixels) and DIMENSION. The "+ 55" on
    the y is to allow space for the timer. */
    sf::RenderWindow window(sf::VideoMode((41 * DIMENSION - 1), ((41 * DIMENSION - 1) + 55)), "Minesweeper");

    //This is where I load the font in. I downloaded the font file from fontsgeek.com.
    sf::Font font;
    if (!font.loadFromFile("Courier Regular.ttf")) {
        std::cout << "No font!";
    }

    //This just sets the game to its initial state.
    firstClick = true;
    stop = false;

    /* These loops create the RectangleShape and Text objects and sets their colors and positions based
    on their indices. */
    for(int i = 0; i < DIMENSION; i++) {
        for (int j = 0; j < DIMENSION; j++) {
            board[i][j] = new sf::RectangleShape(sf::Vector2f(40.f, 40.f));
            (*board[i][j]).setFillColor(sf::Color(255, 255, 255));
            float x = 41 * i;
            float y = 41 * j + 55;
            (*board[i][j]).setPosition(x, y);

            x += 16.0;
            y += 8.0;
            numbers[i][j] = new sf::Text(" ", font, 20);
            (*numbers[i][j]).setPosition(x, y);
        }
    }

    /* These lines are for the stopwatch at the top of the game. The time variable will eventually hold
    the difference between the time of the first click and the current time. It will be constantly
    updated and reassigned to the Text object stopwatch's string in the "while(window.isOpen())" loop.
    stopwatch's color and position are also set. */
    std::clock_t start = std::clock();
    double time;
    sf::Text stopwatch = sf::Text(" ", font, 45);
    stopwatch.setFillColor(sf::Color(255, 255, 255));
    stopwatch.setPosition(0.f, 1.f);

    //This loop contains all the code that runs over and over while the program is running.
    while (window.isOpen()) {

        /* Event is an SFML class that could be anything that happens to the window. I use it for the
        window closing and the mouse clicking. */
        sf::Event event;

        //This loop checks to see if something has happened and takes actions based on which action it is.
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed) {//!!!! This is logic that depends on user input (mouse clicks).
                if (firstClick) {
                    start = std::clock();
                }

                if (!stop) {
                    /* These two variables hold the indices of the square clicked based on the coordinates of
                    the mouse when the click happened. */
                    int i = event.mouseButton.x / 41;
                    int j = (event.mouseButton.y - 55) / 41;

                    /* In Minesweeper, left clicking a square reveals whether if it's a mine or, if not,
                    how many mines surround it */
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        while (firstClick) {
                            /* These are the calls to the "bundles" of code in the void methods. They iterate
                            until the first square is empty. */
                            assignMines();
                            assignNumbers();

                            /* The game is very difficult if you don't start on an empty square, so this
                            conditional ensures that the while loop iterates until you do. */
                            if ((*numbers[i][j]).getString() == "0") {
                                firstClick = false;
                            }
                        }

                        //If a mine is clicked, the game is over and all mines are revealed.
                        if (isMine(i, j)) {
                            if ((*numbers[i][j]).getString() != "M") {
                                for (int k = 0; k < NUM_MINES; k++) {
                                    (*board[minePositions[k][0]][minePositions[k][1]]).setFillColor(sf::Color(0, 0, 0));
                                }
                                stop = true;
                            }
                        }

                        //Otherwise, the square is revealed.
                        else {
                            reveal(i, j);//!!!! This is a call to the student-developed procedure.
                        }
                    }
                    /* In Minesweeper, when a square is right clicked, it is marked as a mine, regardless of
                    whether it actually is. */
                    else if (event.mouseButton.button == sf::Mouse::Right) {
                        /* If the clicked square isn't already marked, its text is set to "M" and the text is
                        revealed. */
                        if ((*numbers[i][j]).getString() != "M") {
                            (*numbers[i][j]).setString("M");
                            (*numbers[i][j]).setFillColor(sf::Color(0, 0, 0));
                        }
                        //Otherwise, the square is unmarked and reassigned its number.
                        else {
                            (*numbers[i][j]).setString("");
                            assignNumbers();
                            if ((*board[i][j]).getFillColor() == sf::Color(200, 200, 200)) {
                                (*numbers[i][j]).setFillColor(sf::Color(0, 0, 0));
                            }
                            else if ((*board[i][j]).getFillColor() == sf::Color(255, 255, 255)) {
                                (*numbers[i][j]).setFillColor(sf::Color(255, 255, 255));
                            }
                        }
                    }
                }
            }
        }

        //This conditional runs once the game has started and is responsible for the timer.
        if (!firstClick) {
            /* Each time the window updates, the time from the first click is updated. CLOCKS_PER_SEC is
            a constant in ctime library that converts ticks, which is what the time variable measures,
            to seconds. */
            time = ((int)std::clock() - (int)start) / CLOCKS_PER_SEC; 
            if(!stop) stopwatch.setString(std::to_string((int)time));

            //Depending on the number of digits, the position of the timer adjusts slightly.
            if (time < 10) {
                stopwatch.setPosition((double)(((41 * DIMENSION - 1) / 2) - 45), 1.f);
            }
            else if (time < 100) {
                stopwatch.setPosition((double)(((41 * DIMENSION - 1) / 2) - 67), 1.f);
            }
            else if (time < 1000) {
                stopwatch.setPosition((double)(((41 * DIMENSION - 1) / 2) - 90), 1.f);
            }
            else {
                stopwatch.setPosition((double)(((41 * DIMENSION - 1) / 2) - 112), 1.f);
            }
        }
        
        //When using SFML windows, the window must be cleared each time you update it.
        window.clear();

        //This loop "draws" all the currently visible RectangleShapes or Texts on the window.
        for (int i = 0; i < DIMENSION; i++) {
            for (int j = 0; j < DIMENSION; j++) {
                window.draw(*board[i][j]);
                if ((*numbers[i][j]).getFillColor() == sf::Color(0, 0, 0)) {
                    window.draw(*numbers[i][j]);
                }
            }
        }
        window.draw(stopwatch);

        //window.display() updates the window with all the new drawings.
        window.display();//!!!! This line outputs the GUI.
    }

	return 0;
}


