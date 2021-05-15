#include <stdio.h>
#include <stdlib.h>
#include "chessboard.h"
#include "chessgame.h"

void print_valid_moves(struct chess_board *cb, struct square s) {
  for (int i = 0; i < cb->board[s.rank][s.file]->valid_len; ++i) {
    printf("%c%d ", cb->board[s.rank][s.file]->valid_moves[i].file + 'a', cb->board[s.rank][s.file]->valid_moves[i].rank + 1);
  }
  printf("\n");
}
int main(void) {
  struct chess_game *cg = create_game();
  print_pieces(cg->cb);
  //cg->cb->board[2][0] = malloc(sizeof(struct chess_piece));
  //cg->cb->board[2][0]->color = 'b';
  //cg->cb->board[2][0]->piece = 'P';
  struct square s = {7,1};
  print_valid_moves(cg->cb,s);
  s.file = 6;
  print_valid_moves(cg->cb,s);
  play_chess(cg);
}