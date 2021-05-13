#include <stdio.h>
#include "chessboard.h"
#include "chessgame.h"

int main(void) {
  struct chess_game *cg = create_game();
  play_chess(cg);
}