#include <algorithm>
#include <fstream>
#include <ncurses.h>
#include <queue>
#include <string>

#define NORTH 0
#define SOUTH 1
#define EAST 2
#define WEST 3

#define MAX_BOARD_SIZE 50

// game speed (updates per second)
#define GAME_SPEED 20
// north south delay, because north south moves faster due to characters not
// being square
#define NS_DELAY 32
// interval of time between each game loop
#define INTERVAL 1000 / GAME_SPEED

// snake piece char and apple char
#define SNAKE_PIECE 'O'
#define APPLE_PIECE 'X'

// dot structure
struct dot {
  int y;
  int x;
  char icon;
};

struct HighScore {
  std::string playerName;
  int score;
};

void initGameScreen();

void loadHighScores(std::vector<HighScore> &highScores);

void addCurrentScore(HighScore currentScore,
                     std::vector<HighScore> &highScores);

int saveHighScores(std::vector<HighScore> &highScores);

void initGameVars(bool &continueRound, char &c, int &score, int &direction,
                  int &selected, int &y, int &x, WINDOW *win);

void changeDirection(wchar_t c, int &direction);

void moveDot(int direction, dot &d, int x, int y);

void drawDot(dot d);

void moveApple(dot &a, int maxY, int maxX);
