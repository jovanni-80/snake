// main.cpp
// runs the snake game cycle and checks some logic
//
// By: Jovanni Rodriguez

#include "snake.h"
#include <chrono>
#include <ncurses.h>
#include <stdlib.h>
#include <thread>
#include <time.h>
#include <unistd.h>

inline auto calculateMidpoint(const uint32_t coordinate) {
  return coordinate / 2;
}

inline auto doCollide(const dot &dot1, const dot &dot2) -> bool {
  return dot1.x == dot2.x && dot1.y == dot2.y;
}

auto main(int argc, char **argv) -> int {

  // initialize seed for random function
  srand(time(NULL));

  // delay values (slowr in north-south beacuse of character height)
  const uint8_t delay[4] = {NS_DELAY, NS_DELAY, 0, 0};

  // creating variables before the game loop
  bool continueRound; // player hasn't lost current round
  char c;             // input char?
  Orientation orientation = Orientation::NORTH;
  int playGame = 0, // play again
      score,        // score of the current round
      selected,     // selected option at end screen (yes/no)
      y,            // maximum y border of screen
      x;            // maximum x border of screen

  // game loop
  do {

    // set all of the ncurses stuff
    initGameScreen();

    // set window size
    // @TODO: Handle WINCHN events

    // initialize score, c, orientation, max y, and max x to default values
    initGameVars(continueRound, c, score, orientation, selected, y, x, stdscr);

    // set default snake coord values (x,y in middle)
    dot d;
    d.y = calculateMidpoint(y) - 1;
    d.x = calculateMidpoint(x) - 1;
    d.icon = SNAKE_PIECE;

    // setup snake (Queue)
    std::queue<dot> snakeQ;
    snakeQ.push(d);

    // create and print default apple
    dot apple;
    apple.icon = APPLE_PIECE;
    moveApple(apple, y, x);

    // initially move the cursor
    move(d.y, d.x);

    // game loop
    do {
      // draw the apple
      drawDot(apple);

      // draw the border
      box(stdscr, 0, 0);

      // draw score
      move(0, 2);
      printw("Score: %d", score);

      // refresh the screen
      refresh();

      // delay between game loops (how fast the snake moves)
      std::this_thread::sleep_for(
          std::chrono::milliseconds(INTERVAL + delay[orientation]));

      // change direction based on input, nodelay(), stops the blocking
      changeOrientation(getch(), orientation);

      const auto currentSnakeHead = snakeQ.back();

      // check for apple hit
      if (doCollide(apple, currentSnakeHead)) {

        // iterate score
        score++;

        // get new dot to be head
        dot newSnakeHead{currentSnakeHead.y, currentSnakeHead.x, SNAKE_PIECE};

        // move new head in the right direction
        moveDot(orientation, newSnakeHead, x, y);

        // push the new head into the queue
        snakeQ.push(newSnakeHead);

        // draw the new snake
        drawSnake(snakeQ);

        // move the apple somewhere else
        moveApple(apple, y, x);

        // don't let the apple spawn in the snake
        while (mvinch(apple.y, apple.x) == SNAKE_PIECE)
          moveApple(apple, y, x);

      } else {
        // create the new head dot
        dot newSnakeHead{currentSnakeHead.y, currentSnakeHead.x, SNAKE_PIECE};

        // move the new head in the right direction
        moveDot(orientation, newSnakeHead, x, y);

        // if snake head is moving to a place in which a snake piece already
        // exists
        if (doCollide(newSnakeHead, currentSnakeHead)) {
          continueRound = false;
          continue;
        } else if (mvinch(newSnakeHead.y, newSnakeHead.x) == SNAKE_PIECE) {
          continueRound = false;
          continue;
        }

        // move to the tail of the snake
        move(snakeQ.front().y, snakeQ.front().x);

        // clear the tail space
        addch(' ');

        // push the new head dot to the queue
        snakeQ.push(newSnakeHead);

        // pop the tail dot
        snakeQ.pop();

        // draw the new part of the snake
        drawDot(snakeQ.back());
      }

      // if out of bounds, end game

    } while (continueRound);

    // clear the screen
    clear();

    const auto xMidpoint = calculateMidpoint(x);
    const auto yMidpoint = calculateMidpoint(y);

    // print the end game screen
    // move to middle of the screen
    move(yMidpoint - 1, xMidpoint - 5);

    // print message
    printw("Game Over");

    // move right below game over message and print the score
    move(yMidpoint, xMidpoint - 5);
    printw("Score: %d", score);

    // move below score and ask user if they want to play again
    move(yMidpoint + 3, xMidpoint - 6);
    printw("Play again?");

    // game loop value
    bool game_over = true;

    // move and print yes entry under play again to left
    move(yMidpoint + 4, xMidpoint - 5);
    printw("Yes");

    // move and print no entry unter 'Play again?' to right
    move(yMidpoint + 4, xMidpoint + 1);
    printw("No");

    // game over menu loop
    do {
      nodelay(stdscr, false);

      // if first element is selected, draw it with background color
      // move to where to put the arrow
      move(yMidpoint + 4, xMidpoint - 7);
      playGame == 0 ? printw("->") : printw("  ");

      // if second element is selected, draw it with background color
      // move to where the arrow would be
      move(yMidpoint + 4, xMidpoint - 1);
      playGame == 1 ? printw("->") : printw("  ");

      // reload screen with message
      refresh();

      // get user input from game over screen
      selected = getch();

      // switch case on user input
      switch (selected) {
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
        game_over = false;
        break;
      }

      // wait for user input
    } while (game_over);

    // end window after game is over

  } while (playGame == 0);

  endwin();

  return 0;
}
