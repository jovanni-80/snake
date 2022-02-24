#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <thread>
#include <unistd.h>
#include "snake.h"

int main(int argc, char** argv) {

    //initialize seed for rand function
    srand(time(NULL));

	initGameScreen();

    //default values for starting the game
    bool continueGame = true;
    char c = 'l';
    int direction = NORTH;

	//delay in the north-south directions, not in east-west
    int delay[4] = {NS_DELAY, NS_DELAY, 0, 0};

    //get max x and y to find middle of screen
    int y,x;
    getmaxyx(stdscr, y,x);

    //set default dot values (x,y in middle)
    dot d;
    d.y = y/2-1;
    d.x = x/2-1;
    d.icon = SNAKE_PIECE;

    //setup snake (Queue)
    std::queue<dot> snakeQ;
    snakeQ.push(d);

    //set default for apple
    dot apple;
    apple.icon = APPLE_PIECE;
    moveApple(apple, y, x);
    int score = 0;

    //initially move the cursor
    move(d.y,d.x);

	//game loop
    do {
        //draw the apple
        drawDot(apple);

        //draw the border
        box(stdscr, '|', '-');

        //draw score
        move(0,2);
        printw("Score: %d", score);

        //refresh the screen
        refresh();

        //delay between game loops (how fast the snake moves)
        std::this_thread::sleep_for(std::chrono::milliseconds(INTERVAL+delay[direction]));

        //change direction based on input, nodelay(), stops the blocking
        changeDirection(getch(), direction);

        //check for apple hit
        if(snakeQ.back().x == apple.x && snakeQ.back().y == apple.y){

			//iterate score
            score++;

            //get new dot to be head
            dot newDot;
			//pupt new head dot at current head
            newDot.y = snakeQ.back().y;
            newDot.x = snakeQ.back().x;
            newDot.icon = SNAKE_PIECE;
            //move new head in the right direction
            moveDot(direction, newDot);
            //push the new head into the queue
            snakeQ.push(newDot);
            //draw the new snake piece
            drawSnake(snakeQ);
            //move the apple somewhere else
            moveApple(apple, y, x);
			//don't let the apple spawn in the snake
			while(mvinch(apple.y,apple.x) == SNAKE_PIECE)
				moveApple(apple,y,x);
        }
        else {
            //create the new head dot
            dot newDot;
			//put new head dot at current head
            newDot.y = snakeQ.back().y;
            newDot.x = snakeQ.back().x;
            newDot.icon = SNAKE_PIECE;
            //move the new head in the right direction
            moveDot(direction, newDot);
			//if snake head is moving to a place in which a snake piece already exists
			if (mvinch(newDot.y, newDot.x) == SNAKE_PIECE){
				continueGame = false;
				continue;
			}

			//move to the tail of the snake
            move(snakeQ.front().y, snakeQ.front().x);
            //clear the tail space
            addch(' ');

            //push the new head dot to the queue
            snakeQ.push(newDot);
            //pop the tail dot
            snakeQ.pop();
            //draw the new part of the snake
            drawDot(snakeQ.back());
        }

        //if out of bounds, end game
        continueGame = inBounds(snakeQ.back(),y,x);

    } while (continueGame);

    //clear the screen
    clear();

    //move to middle of the screen
    move(y/2-1, x/2-5);

    //print message
    printw("Game Over");

    move(y/2, x/2-5);
    printw("Score: %d", score);

    //reload screen with message
    refresh();

    //set no delay to false to wait for input
    nodelay(stdscr, false);

    //wait for user input
    getch();
	sleep(2);

    //deallocate window memory used by ncurses
    endwin();

    return 0;
}
