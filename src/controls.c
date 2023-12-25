#include "controls.h"

control_t getInput() {
  switch (getc(stdin)) {
    case KEY_FLAG:
    case KEY_FLAG_UPPERCASE:
      return ACTION_FLAG;
    case KEY_CLICK:
      return ACTION_CLICK;
    case KEY_QUIT:
      return ACTION_QUIT;
    case '\033':
      getc(stdin);
      switch (getc(stdin)) {
        case 'A':
          return MOVE_UP;
        case 'B':
          return MOVE_DOWN;
        case 'C':
          return MOVE_RIGHT;
        case 'D':
          return MOVE_LEFT;
      }
  }
  return ACTION_INVALID;
}

void handleInput(game_t *game, control_t input) {
  cell_state_t state = getCellState(game->board, game->cursor.x, game->cursor.y);
  uint8_t is_bomb = getCellIsBomb(game->board, game->cursor.x, game->cursor.y);
  switch (input) {
    case MOVE_UP:
      if (game->cursor.y > 0) --game->cursor.y;
      break;
    case MOVE_DOWN:
      if (game->cursor.y < game->board->rows - 1) ++game->cursor.y;
      break;
    case MOVE_LEFT:
      if (game->cursor.x > 0) --game->cursor.x;
      break;
    case MOVE_RIGHT:
      if (game->cursor.x < game->board->cols - 1) ++game->cursor.x;
      break;
    case ACTION_FLAG:
      if (state == HIDDEN)
        setCellState(game->board, game->cursor.x, game->cursor.y, FLAGGED);
      else if (state == FLAGGED)
        setCellState(game->board, game->cursor.x, game->cursor.y, HIDDEN);
      break;
    case ACTION_CLICK:
      handleCellClick(game, state, is_bomb); // Complex logic, warrants separate function.
      break;
    case ACTION_QUIT:
      setGameState(game, QUIT);
      break;
  }
}

void handleCellClick(game_t *game, cell_state_t state, uint8_t is_bomb) {
  if (state == HIDDEN) {
    // If cell is bomb, lose game.
    if (is_bomb)
      setBoardState(game->board, LOSE);

    // If cell has no neighbors, show adjacent cells with no neighbors.
    if (getCellNeighbors(game->board, getCursorX(game), getCursorY(game)) == 0) {
      __recursiveZeroCellClick(game, getCursorX(game), getCursorY(game));
    }

    // Show cell.
    setCellState(game->board, getCursorX(game), getCursorY(game), SHOWN);

  } else if (state == SHOWN) {
    // Not implemented.
  }
}

void __recursiveZeroCellClick(game_t *game, uint8_t x, uint8_t y) {
  // If cell is bomb or is already shown, return.
  if (getCellIsBomb(game->board, x, y) || (getCellState(game->board, x, y) == SHOWN)) return;

  setCellState(game->board, x, y, SHOWN);
  
  uint8_t x_lower = (x > 0)?(x - 1):(0);
  uint8_t x_upper = (x < game->board->cols - 1)?(x + 2):(game->board->cols);
  uint8_t y_lower = (y > 0)?(y - 1):(0);
  uint8_t y_upper = (y < game->board->rows - 1)?(y + 2):(game->board->rows);

  for (int i = y_lower; i < y_upper; ++i) {
    for (int j = x_lower; j < x_upper; ++j) {
      if (i == y && j == x) continue;
      __recursiveZeroCellClick(game, j, i);
    }
  }
}