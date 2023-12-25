#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include "game.h"
#include "draw.h"
#include "controls.h"

#define ROWS 10
#define COLS 10
#define MINES 10

int main() {

  // Hide key inputs.
  static struct termios oldt, newt;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  srand(time(NULL));

  game_t *game = createGame(ROWS, COLS, MINES);

  printf("\033[2J");
  while (getGameState(game) == ONGOING) {
    draw(game);
    handleInput(game, getInput());
  }

  deleteGame(game);

  printf("\n\n");
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  
  return 0;
}