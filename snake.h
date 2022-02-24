#include <queue>

#define NORTH 0
#define SOUTH 1
#define EAST 2
#define WEST 3

#define MAX_BOARD_SIZE 50

//game speed (updates per second)
#define GAME_SPEED 15
//north south delay, because north south moves faster due to characters not being square
#define NS_DELAY 30
//interval of time between each game loop
#define INTERVAL 1000/GAME_SPEED

//snake piece char and apple char
#define SNAKE_PIECE 'O'
#define APPLE_PIECE 'X'

//dot structure
struct dot {
        int y;
        int x;
        char icon;
};

void initGameScreen() {

    //start ncurses
    initscr();

    //set some default ncurses settings
    nodelay(stdscr, true);
    curs_set(false);
    keypad(stdscr, true);
    noecho();
    cbreak();

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
