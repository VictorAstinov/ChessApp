#ifndef __CHESSBOARD_H_INCLUDED__
#define __CHESSBOARD_H_INCLUDED__

#define BOARD_DIMENSION 8
#include <stdbool.h>

struct chess_piece {
  char piece;
  int rank;
  int file;
  char color;
  bool moved;
};
struct chess_board {
  struct chess_piece *board[BOARD_DIMENSION][BOARD_DIMENSION];
  struct chess_piece **active;
  int len;
  int max_len;
};

struct chess_board *create_board(void);
void destroy_board(struct chess_board *cb);
void print_board(const struct chess_board *cb);
void print_pieces(const struct chess_board *cb);

#endif