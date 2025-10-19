#include "snake.h"

// get the ncurses options set, and initialize the screen
auto initGameScreen() -> void {

  // clear any previous screen
  clear();

  // start ncurses
  initscr();

  // set some ncurses settings
  nodelay(stdscr, true);
  curs_set(false);
  keypad(stdscr, true);
  noecho();
  cbreak();
}

auto initGameVars(bool &continueRound, char &c, int &score,
                  Orientation &orientation, int &selected, int &y, int &x,
                  WINDOW *win) -> void {
  // default values for blank game
  continueRound = true;
  c = 'l';
  orientation = Orientation::NORTH;
  selected = 0;
  getmaxyx(win, y, x);
  x = std::min(x, 100);
  y = std::min(y, 100);
  clear();
  resizeterm(y, x);
  score = 0;
}

// alters the direction variable based on the keyboard input given
auto changeOrientation(wchar_t c, Orientation &orientation) -> void {
  // change direction based on c
  switch (c) {
  case KEY_LEFT:
    orientation = Orientation::WEST;
    break;
  case KEY_DOWN:
    orientation = Orientation::SOUTH;
    break;
  case KEY_RIGHT:
    orientation = Orientation::EAST;
    break;
  case KEY_UP:
    orientation = Orientation::NORTH;
    break;
  default:
    break;
  }
}

// updates the dot's coordinates based on it's direction
auto moveDot(const Orientation orientation, dot &d, const int x, const int y)
    -> void {
  int arr[4] = {-1, 1, 1, -1};

  // directions which alter y val
  if (orientation == Orientation::NORTH || orientation == Orientation::SOUTH) {
    d.y += arr[orientation];
    if (d.y < 1) {
      d.y = y - 2;
    } else if (d.y > y - 2) {
      d.y = 1;
    }
  } else if (orientation == Orientation::WEST ||
             orientation == Orientation::EAST) {
    d.x += arr[orientation];
    if (d.x < 1) {
      d.x = x - 2;
    } else if (d.x > x - 2) {
      d.x = 1;
    }
  }
}

// clear and draw dot on the screen after moving it
auto drawDot(dot d) -> void {

  // move the snake to the new direction
  move(d.y, d.x);
  // draw the thing
  addch(d.icon);
}

// draw the entire snake current workaround for eating the apple
auto drawSnake(std::queue<dot> snakeQueue) -> void {
  while (!snakeQueue.empty()) {
    drawDot(snakeQueue.front());
    snakeQueue.pop();
  }
}

// move the apple to a random location
auto moveApple(dot &a, int maxY, int maxX) -> void {
  a.x = rand() % (maxX - 2) + 1;
  a.y = rand() % (maxY - 2) + 1;
}
