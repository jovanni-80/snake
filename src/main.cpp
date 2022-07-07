// main.cpp
// runs the snake game cycle and checks some logic
//
// By: Jovanni Rodriguez

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <thread>
#include <unistd.h>
#include "snake.h"


int main(int argc, char** argv) {

    // initialize seed for random function
    srand(time(NULL));

    // delay values (slowr in north-south beacuse of character height)
    int delay[4] = {NS_DELAY, NS_DELAY, 0, 0};

    // creating variables before the game loop
    bool continueRound;         // player hasn't lost current round
    char c;                     // input char?
    int playGame = 0,           // play again
        direction,              // direction the snake is headed in
        score,                  // score of the current round
        selected,               // selected option at end screen (yes/no)
        y,                      // maximum y border of screen
        x;                      // maximum x border of screen

    // game loop
    do {

        // set all of the ncurses stuff
        initGameScreen();

        // set window size
        WINDOW *w;

        // initialize score, c, direction, max y, and max x to default values
        initGameVars(continueRound, c, score, direction, selected, y, x, stdscr);

        // set default snake coord values (x,y in middle)
        dot d;
        d.y = y/2-1;
        d.x = x/2-1;
        d.icon = SNAKE_PIECE;

        // setup snake (Queue)
        std::queue<dot> snakeQ;
        snakeQ.push(d);

        // create and print default apple
        dot apple;
        apple.icon = APPLE_PIECE;
        moveApple(apple, y, x);

        // initially move the cursor
        move(d.y,d.x);

        // game loop
        do {
            // draw the apple
            drawDot(apple);

            // draw the border
            box(stdscr,0,0);

            // draw score
            move(0,2);
            printw("Score: %d", score);

            // refresh the screen
            refresh();

            // delay between game loops (how fast the snake moves)
            std::this_thread::sleep_for(std::chrono::milliseconds(INTERVAL+delay[direction]));

            // change direction based on input, nodelay(), stops the blocking
            changeDirection(getch(), direction);

            // check for apple hit
            if(snakeQ.back().x == apple.x && snakeQ.back().y == apple.y){

                // iterate score
                score++;

                // get new dot to be head
                dot newDot;

                // pupt new head dot at current head
                newDot.y = snakeQ.back().y;
                newDot.x = snakeQ.back().x;
                newDot.icon = SNAKE_PIECE;

                // move new head in the right direction
                moveDot(direction, newDot);

                // push the new head into the queue
                snakeQ.push(newDot);

                // draw the new snake piece
                drawSnake(snakeQ);

                // move the apple somewhere else
                moveApple(apple, y, x);

                // don't let the apple spawn in the snake
                while(mvinch(apple.y,apple.x) == SNAKE_PIECE)
                    moveApple(apple,y,x);

            }
            else {
                // create the new head dot
                dot newDot;

                // put new head dot at current head
                newDot.y = snakeQ.back().y;
                newDot.x = snakeQ.back().x;
                newDot.icon = SNAKE_PIECE;

                // move the new head in the right direction
                moveDot(direction, newDot);

                // if snake head is moving to a place in which a snake piece already exists
                if (mvinch(newDot.y, newDot.x) == SNAKE_PIECE){
                    continueRound = false;
                    continue;
                }

                // move to the tail of the snake
                move(snakeQ.front().y, snakeQ.front().x);

                // clear the tail space
                addch(' ');

                // push the new head dot to the queue
                snakeQ.push(newDot);

                // pop the tail dot
                snakeQ.pop();

                // draw the new part of the snake
                drawDot(snakeQ.back());

            }

            // if out of bounds, end game
            continueRound = inBounds(snakeQ.back(),y,x);

        } while (continueRound);

        // clear the screen
        clear();

        // print the end game screen
        // move to middle of the screen
        move(y/2-1, x/2-5);

        // print message
        printw("Game Over");

        // move right below game over message and print the score
        move(y/2, x/2-5);
        printw("Score: %d", score);

        // move below score and ask user if they want to play again
        move(y/2+3, x/2-6);
        printw("Play again?");

        // game loop value
        bool gameOverLoop = true;

        // game over menu loop
        do {
            nodelay(stdscr, false);

            // if first element is selected, draw it with background color
            // move to where to put the arrow
            move(y/2+4, x/2-7);
            ( playGame == 0) ? printw("->"):printw("  ");

            // move and print yes entry under play again to left
            move (y/2+4, x/2-5);
            printw("Yes");

            // if second element is selected, draw it with background color
            // move to where the arrow would be
            move(y/2+4, x/2-1);
            (playGame == 1) ? printw("->"): printw("  ");

            // move and print no entry unter 'Play again?' to right
            move (y/2+4, x/2+1);
            printw("No");

            // reload screen with message
            refresh();

            // get user input from game over screen
            selected = getch();

            // switch case on user input
            switch(selected) {
                // upon left press key select yes
                case KEY_LEFT:
                    playGame = 0;
                    break;
                // upon right key press select no
                case KEY_RIGHT:
                    playGame = 1;
                    break;
                // upon pressing enter confirm choice by exiting the loop
                case 10:
                    gameOverLoop = false;
                    break;
            }

            // wait for user input
        } while (gameOverLoop);

        // end window after game is over
        endwin();

    } while(playGame == 0);


    return 0;
}
