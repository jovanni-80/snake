#include "snake.h"

//get the ncurses options set, and initialize the screen
void initGameScreen() {

    //clear any previous screen
    clear();

    //start ncurses
    initscr();

    //set some ncurses settings
    nodelay(stdscr, true);
    curs_set(false);
    keypad(stdscr, true);
    noecho();
    cbreak();

}

void initGameVars(bool &continueRound, char &c, int &score,
        int &direction, int &selected, int &y, int &x, WINDOW *win) {
    //default values for blank game
    continueRound = true;
    c = 'l';
    direction = NORTH;
    selected = 0;
    getmaxyx(win, y, x);
    x = std::min(x, 100);
    y = std::min(y, 100);
    clear();
    resizeterm(y, x);
    score = 0;
}

//alters the direction variable based on the keyboard input given
void changeDirection(wchar_t c, int& direction) {
    //change direction based on c
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
void moveDot(int direction, dot& d, int x, int y) {
    int arr[] = {-1, 1, 1, -1};

    //directions which alter y val
    if (direction == NORTH || direction == SOUTH) {
        d.y += arr[direction];
        if (d.y < 1) {
            d.y = y-2;
        }
        else if (d.y > y-2) {
            d.y = 1;
        }
    }
    else if (direction == WEST || direction == EAST) {
        d.x += arr[direction];
        if (d.x < 1) {
            d.x = x-2;
        }
        else if (d.x > x-2) {
            d.x = 1;
        }
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

//move the apple to a random location
void moveApple(dot &a, int maxY, int maxX) {
    a.x = rand()%(maxX-2)+1;
    a.y = rand()%(maxY-2)+1;
}
