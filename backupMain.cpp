#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <thread>
#include <unistd.h>
#include "snake.h"

#define NORTH 0
#define SOUTH 1
#define EAST 2
#define WEST 3

//game speed (updates per second)
#define GAME_SPEED 15
//north south delay, because north south moves faster due to characters not being square
#define NS_DELAY 30

//snake piece char and apple char
#define SNAKE_PIECE 'O'
#define APPLE_PIECE 'X'

//dot structure
struct dot {
        int y;
        int x;
        char icon;
};

//FUNCTIONS DECLARATIONS-----------------------------------
//updates the direction of the dot based on kb input
void changeDirection(wchar_t c, int & direction);

//adjusts the dot's coordinates based on it's direction
void moveDot(int direction, dot& d);

//draw the dot on the screen
void drawDot(dot d);

//draw the whole snake when eating the apple, otherwise gives a weird bug
void drawSnake(std::queue<dot> snkQ);

//move the apple to a random spot
void moveApple(dot &a, int maxY, int maxX);

//return true if dot is in bounds, false otherwise
bool inBounds(dot d, int maxY, int maxX);
//END FUNCTION DECLARATIONS---------------------------------

int main(int argc, char** argv) {

    //initialize seed for rand function
    srand(time(NULL));

    //start ncurses
    initscr();

    //set some default ncurses settings
    nodelay(stdscr, true);
    curs_set(false);
    keypad(stdscr, true);
    noecho();
    cbreak();

    //default values for starting the game
    bool continueGame = true;
    char c = 'l';
    int direction = NORTH;

    //set the delay between loops based on game speed
    int interval = 1000/GAME_SPEED;
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

        //delay between loops
        //adjust for north-south being too fast
        std::this_thread::sleep_for(std::chrono::milliseconds(interval+delay[direction]));

        //change direction based on input, nodelay(), stops the blocking
        changeDirection(getch(), direction);

        //check for apple hit
        if(snakeQ.back().x == apple.x && snakeQ.back().y == apple.y){
            score++;
            //get new dot to be head
            dot newDot;
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
        }
        else {
            //move to the tail of the snake
            move(snakeQ.front().y, snakeQ.front().x);
            //clear the tail space
            addch(' ');
            //create the new head dot
            dot newDot;
            newDot.y = snakeQ.back().y;
            newDot.x = snakeQ.back().x;
            newDot.icon = SNAKE_PIECE;
            //move the head in the current direction
            moveDot(direction, newDot);
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

//alters the direction variable based on the keyboard input given
void changeDirection(wchar_t c, int& direction) {
    switch(c) {
        case KEY_LEFT:
            direction = WEST;
            break;
        case KEY_DOWN:
            direction = SOUTH;
            break;
        case KEY_RIGHT:
            direction = EAST;
            break;
        case KEY_UP:
            direction = NORTH;
            break;
        default:
            break;
    }
}

//updates the dot's coordinates based on it's direction
void moveDot(int direction, dot& d) {
    int arr[] = {-1, 1, 1, -1};

    //directions which alter y val
    if (direction == NORTH || direction == SOUTH) {
        d.y+=arr[direction];
    }
    else if (direction == WEST || direction == EAST) {
        d.x+=arr[direction];
    }
}

//clear and draw dot on the screen after moving it
void drawDot(dot d) {

    //move the snake to the new direction
    move(d.y, d.x);
    //draw the thing
    addch(d.icon);
}

//draw the entire snake current workaround for eating the apple
void drawSnake(std::queue<dot> snkQ) {
    std::queue<dot> temp = snkQ;

    while(!temp.empty()) {
        drawDot(temp.front());
        temp.pop();
    }

}

//return true if dot is inbounds, false otherwise
bool inBounds(dot d, int maxY, int maxX) {
    bool ret = true;

    if (d.y < 1 || d.y > maxY-2)
        ret = false;
    else if (d.x < 1 || d.x > maxX-2)
        ret = false;

    return ret;
}

//move the apple to a random location
void moveApple(dot &a, int maxY, int maxX) {
    a.x = rand()%(maxX-2)+1;
    a.y = rand()%(maxY-2)+1;
}
