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

void copy_piece(struct chess_board *cb, const struct square *old_s, const struct square *new_s) {
  assert(cb);
  assert(old_s);
  assert(new_s);
  cb->board[new_s->rank][new_s->file]->color = cb->board[old_s->rank][old_s->file]->color;
  cb->board[new_s->rank][new_s->file]->piece = cb->board[old_s->rank][old_s->file]->piece;
  cb->board[new_s->rank][new_s->file]->valid_move = cb->board[old_s->rank][old_s->file]->valid_move;
  cb->board[new_s->rank][new_s->file]->promote_to = cb->board[old_s->rank][old_s->file]->promote_to;
  cb->board[new_s->rank][new_s->file]->moved = cb->board[old_s->rank][old_s->file]->moved;
  cb->board[new_s->rank][new_s->file]->square.file = new_s->file;
  cb->board[new_s->rank][new_s->file]->square.rank = new_s->rank;
}

// ONLY WORKS FOR WHITE SIDE NEED TO WORK FOR BLACK ASWELL
// MUST ALSO UPDATE THE PIECE VECTOR FOR WHITE/BLACK
static bool pawn_move(struct chess_board *cb, struct square old_s, struct square new_s) {
  assert(cb);
  if (new_s.rank < 0 || new_s.rank > BOARD_DIMENSION - 1 || new_s.file < 0 || 
      new_s.file > BOARD_DIMENSION - 1) {
    return false;
  }
  // check for taking diagonally
  else if (old_s.rank + 1 == new_s.rank && (old_s.file + 1 == new_s.file || old_s.file - 1 == new_s.file)) {
    // check for piece
    if (!cb->board[new_s.rank][new_s.file]) {
      return false;
    }
    else {
      // same color
      if (cb->board[new_s.rank][new_s.file]->color == cb->board[old_s.rank][old_s.file]->color) {
        return false;
      }
      // different color, can take
      else {
        cb->board[old_s.rank][old_s.file]->moved = true;
        // last rank, promote pawn to piece
        if (new_s.rank == BOARD_DIMENSION - 1) {
          cb->board[old_s.rank][old_s.file]->piece = cb->board[old_s.rank][old_s.file]->promote_to;
          // MUST CHANGE VALID MOVE FUNCTION POINTER AFTER PROMOTION
        }
        copy_piece(cb, &old_s, &new_s);
        free(cb->board[old_s.rank][old_s.file]);
        cb->board[old_s.rank][old_s.file] = NULL;
        return true;
      }
    }
  }
  // check for moves forward 2 squares
  else if (old_s.rank + 2 == new_s.rank && old_s.file == new_s.file) {
    // check if pawn has moved before
    if (!cb->board[old_s.rank][old_s.file]->moved) {
      // check if both square are unoccupied
      if (!cb->board[old_s.rank + 1][old_s.file] && !cb->board[old_s.rank + 2][old_s.file]) {
        cb->board[old_s.rank][old_s.file]->moved = true;
        cb->board[old_s.rank + 2][old_s.file] = malloc(sizeof(struct chess_piece));
        copy_piece(cb, &old_s, &new_s);
        free(cb->board[old_s.rank][old_s.file]);
        cb->board[old_s.rank][old_s.file] = NULL;
        return true;
      }
      else {
        return false;
      }
    }
    else {
      return false;
    }
  }
  // check for moving one square
  else if (old_s.rank + 1 == new_s.rank && old_s.file == new_s.file) {
    // check if square in front is unoccupied
    if (!cb->board[old_s.rank + 1][old_s.file]) {
      cb->board[old_s.rank][old_s.file]->moved = true;
      cb->board[old_s.rank + 1][old_s.file] = malloc(sizeof(struct chess_piece));
      // last rank, promote pawn to piece
      if (new_s.rank == BOARD_DIMENSION - 1) {
        cb->board[old_s.rank][old_s.file]->piece = cb->board[old_s.rank][old_s.file]->promote_to;
        // MUST CHANGE VALID MOVE FUNCTION POINTER AFTER PROMOTION
      }
      copy_piece(cb, &old_s, &new_s);
      free(cb->board[old_s.rank][old_s.file]);
      cb->board[old_s.rank][old_s.file] = NULL;
      return true;
    }
    else {
      return false;
    }
  }
  else {
    return false;
  }
}


struct chess_board *create_board(void) {
  struct chess_board *cb = malloc(sizeof(struct chess_board));
  cb->w_len = cb->b_len = 16;
  cb->w_max_len = cb->b_max_len = 16;
  cb->white_active = malloc(cb->w_len * sizeof(struct chess_piece*));
  cb->black_active = malloc(cb->b_len * sizeof(struct chess_piece*));
  int w_count = 0;
  int b_count = 0;
  // i is rank j is file
  for (int i = 0; i < BOARD_DIMENSION; ++i) {
    for (int j = 0; j < BOARD_DIMENSION; ++j) {
      if (i == 1 || i == BOARD_DIMENSION - 2) {
        cb->board[i][j] = malloc(sizeof(struct chess_piece));
        cb->board[i][j]->piece = PAWN;
        if (i == 1) {
          cb->board[i][j]->color = 'w';
          cb->white_active[w_count] = cb->board[i][j];
          ++w_count;
        }
        else {
          cb->board[i][j]->color = 'b';
          cb->black_active[b_count] = cb->board[i][j];
          ++b_count;
        }
        cb->board[i][j]->moved = false;
        cb->board[i][j]->valid_move = pawn_move;
        cb->board[i][j]->square.rank = i;
        cb->board[i][j]->square.file  = j;
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
          cb->white_active[w_count] = cb->board[i][j];
          ++w_count;
        }
        else {
          cb->board[i][j]->color = 'b';
          cb->black_active[b_count] = cb->board[i][j];
          ++b_count;
        }
        cb->board[i][j]->moved = false;
        cb->board[i][j]->square.rank = i;
        cb->board[i][j]->square.file  = j;
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
  bool last_endl = false;
  printf("White pieces:\n");
  for (int i = 0; i < cb->w_len; ++i) {
    if ((i + 1) % 8 == 0) {
      printf("%c%c\n", cb->white_active[i]->color, cb->white_active[i]->piece);
      last_endl = true;
    }
    else {
      printf("%c%c, ", cb->white_active[i]->color, cb->white_active[i]->piece);
      last_endl = false;
    }
  }
  if (!last_endl) {
    printf("\n");
  }
  printf("Black pieces:\n");
  for (int i = 0; i < cb->b_len; ++i) {
    if ((i + 1) % 8 == 0) {
      printf("%c%c\n", cb->black_active[i]->color, cb->black_active[i]->piece);
      last_endl = true;
    }
    else {
      printf("%c%c, ", cb->black_active[i]->color, cb->black_active[i]->piece);
      last_endl = false;
    }
  }
  if (!last_endl) {
    printf("\n");
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
