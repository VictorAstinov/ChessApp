#include <stdio.h>
#include <stdlib.h>
#include "chessboard.h"
#include "chessgame.h"

int main(void) {
  struct chess_game *cg = create_game();
  print_pieces(cg->cb);
  cg->cb->board[2][0] = malloc(sizeof(struct chess_piece));
  cg->cb->board[2][0]->color = 'b';
  cg->cb->board[2][0]->piece = 'P';
  play_chess(cg);
}