#include "chessboard.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

#define BOARD_DIMENSION 8
#define QUEEN 'Q'
#define KNIGHT 'N'
#define BISHOP 'B'
#define ROOK 'R'
#define KING 'K'
#define PAWN 'P'


struct chess_board *create_board(void) {
  struct chess_board *cb = malloc(sizeof(struct chess_board));
  cb->len = 32;
  cb->max_len = 32;
  cb->active = malloc(cb->len * sizeof(struct chess_piece*));
  int p_count = 0;
  // i is rank j is file
  for (int i = 0; i < BOARD_DIMENSION; ++i) {
    for (int j = 0; j < BOARD_DIMENSION; ++j) {
      if (i == 1 || i == BOARD_DIMENSION - 2) {
        cb->board[i][j] = malloc(sizeof(struct chess_piece));
        cb->board[i][j]->piece = PAWN;
        if (i == 1) {
          cb->board[i][j]->color = 'w';
        }
        else {
          cb->board[i][j]->color = 'b';
        }
        cb->board[i][j]->moved = false;
        cb->active[p_count] = cb->board[i][j];
        cb->board[i][j]->rank = i;
        cb->board[i][j]->file = j;
        ++p_count;
      }
      else if (i == 0 || i == BOARD_DIMENSION - 1) {
        cb->board[i][j] = malloc(sizeof(struct chess_piece));
        if (j == 0 || j == BOARD_DIMENSION - 1) {
          cb->board[i][j]->piece = ROOK;
        }
        else if (j == 1 || j == BOARD_DIMENSION - 2) {
          cb->board[i][j]->piece = KNIGHT;
        }
        else if (j == 2 || j == BOARD_DIMENSION - 3) {
          cb->board[i][j]->piece = BISHOP;
        }
        else if (j == 3) {
          cb->board[i][j]->piece = QUEEN;
        }
        else {
          cb->board[i][j]->piece = KING;
        }
        if (i == 0) {
          cb->board[i][j]->color = 'w';
        }
        else {
          cb->board[i][j]->color = 'b';
        }
        cb->board[i][j]->moved = false;
        cb->board[i][j]->rank = i;
        cb->board[i][j]->file = j;
        cb->active[p_count] = cb->board[i][j];
        ++p_count;
      }
      else {
        cb->board[i][j] = NULL;
      }
    }
  }
  return cb;
}

void print_pieces(const struct chess_board *cb) {
  assert(cb);
  for (int i = 0; i < cb->len; ++i) {
    if ((i + 1) % 8 == 0) {
      printf("%c%c\n", cb->active[i]->color, cb->active[i]->piece);
    }
    else {
      printf("%c%c, ", cb->active[i]->color, cb->active[i]->piece);
    }
  }
}

void destroy_board(struct chess_board *cb) {
  assert(cb);
  for (int i = 0; i < BOARD_DIMENSION; ++i) {
    for (int j = 0; j < BOARD_DIMENSION; ++j) {
      free(cb->board[i][j]);
    }
  }
  free(cb);
}

void print_board(const struct chess_board *cb) {
  assert(cb);
  for (int i = BOARD_DIMENSION; i >= 0; --i) {
    for (int j = 0; j < BOARD_DIMENSION; ++j) {
      printf("----");
    }
    printf("\n");
    if (i != 0) {
      for (int j = 0; j < BOARD_DIMENSION; ++j) {
        if (cb->board[i - 1][j]) {
          printf("|%c%c ", cb->board[i - 1][j]->color, cb->board[i - 1][j]->piece);
        }
        else {
          printf("|   ");
        }
      }
      printf("|\n");
    }
  }
}
