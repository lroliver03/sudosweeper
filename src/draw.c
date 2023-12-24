#include "draw.h"

void draw(game_t *game) {
  uint8_t rows = game->board->rows;
  uint8_t cols = game->board->cols;
  printf("\033[0;0H");
  printf("\n");

  for (int i = 0; i < game->board->rows; i++) {
    for (int j = 0; j < game->board->cols; j++) {
      uint8_t is_cursor = (i == getCursorY(game) && j == getCursorX(game));
      printf("\033[%d;%dH", 2 * i + 1, 2 * j + 1);

      if (is_cursor) 
        printf(ESC_REVERSE);

      switch (getCellState(game->board, j, i)) {
        case HIDDEN:
          printf("%c", CHAR_HIDDEN);
          break;
        case SHOWN:
          printf("%1d", game->board->cells[game->board->rows * i + j].neighbors);
          break;
        case FLAGGED:
          printf("%c", CHAR_FLAG);
          break;
      }

      if (is_cursor) 
        printf(ESC_UNREVERSE);
    }
  }
}
