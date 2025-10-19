#include <ncurses.h>
#include <queue>
#include <stdlib.h>

constexpr uint8_t MAX_BOARD_SIZE = 50;

// game speed (updates per second)
constexpr uint8_t GAME_SPEED = 20;

// north south delay, because north south moves faster due to characters not
// being square
constexpr uint8_t NS_DELAY = 32;

// interval of time between each game loop
constexpr uint16_t INTERVAL = 1000 / GAME_SPEED;

// snake piece char and apple char
constexpr char SNAKE_PIECE = 'O';
constexpr char APPLE_PIECE = 'X';

enum Orientation { NORTH, SOUTH, EAST, WEST };

// dot structure
struct dot {
  int y;
  int x;
  char icon;
};

auto initGameScreen() -> void;

auto initGameVars(bool &continueRound, char &c, int &score,
                  Orientation &orientation, int &selected, int &y, int &x,
                  WINDOW *win) -> void;

auto changeOrientation(wchar_t c, Orientation &orientation) -> void;

auto moveDot(const Orientation, dot &d, const int x, const int y) -> void;

auto drawDot(dot d) -> void;

auto drawSnake(std::queue<dot> snkQ) -> void;

auto moveApple(dot &a, int maxY, int maxX) -> void;
