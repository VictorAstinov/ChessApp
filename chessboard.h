#ifndef __CHESSBOARD_H_INCLUDED__
#define __CHESSBOARD_H_INCLUDED__

#define BOARD_DIMENSION 8
#include <stdbool.h>

struct chess_board;

struct square {
  char rank;
  char file;
};


struct chess_piece {
  char piece;
  char promote_to;
  struct square square;
  char color;
  bool moved;
  struct square *valid_moves;
  char valid_len;
  char valid_max_len;
};

struct chess_board {
  struct chess_piece *board[BOARD_DIMENSION][BOARD_DIMENSION];
  struct chess_piece **white_active;
  struct chess_piece **black_active;
  int w_len;
  int b_len;
};

struct chess_board *create_board(void);
void destroy_board(struct chess_board *cb);
void print_board(const struct chess_board *cb);
void print_pieces(const struct chess_board *cb);
void copy_piece(struct chess_board *cb, const struct square *old_s, const struct square *new_s);

#endif