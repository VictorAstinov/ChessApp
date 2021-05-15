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
  cb->board[new_s->rank][new_s->file]->valid_moves = cb->board[old_s->rank][old_s->file]->valid_moves;
  cb->board[new_s->rank][new_s->file]->valid_len = cb->board[old_s->rank][old_s->file]->valid_len;
  cb->board[new_s->rank][new_s->file]->valid_max_len = cb->board[old_s->rank][old_s->file]->valid_max_len;
  cb->board[new_s->rank][new_s->file]->promote_to = cb->board[old_s->rank][old_s->file]->promote_to;
  cb->board[new_s->rank][new_s->file]->moved = cb->board[old_s->rank][old_s->file]->moved;
  cb->board[new_s->rank][new_s->file]->square.file = new_s->file;
  cb->board[new_s->rank][new_s->file]->square.rank = new_s->rank;
}

static bool in_board(int rank, int file) {
  if (rank < 0 || rank > BOARD_DIMENSION - 1 || file < 0 || file > BOARD_DIMENSION - 1) {
    return false;
  }
  return true;
}

// TODO: ADD PINS TO REQUIREMNTS, NEEDS MORE TESTING
static void pawn_move(struct chess_board *cb, struct square square, char color) {
  assert(cb);
  int flip = 1;
  if (color == 'b') {
    flip *= -1;
  }
  // check for taking diagonally to the left
  int new_rank = square.rank + flip;
  int new_file = square.file - 1;
  if (in_board(new_rank, new_file)) {
    // check for piece of opposite color
    if (cb->board[new_rank][new_file] && (cb->board[square.rank][square.file]->color != cb->board[new_rank][new_file]->color)) {
      // if true add that square to list fo valid moves
      // double vector if needed
      if (cb->board[square.rank][square.file]->valid_len == cb->board[square.rank][square.file]->valid_max_len) {
         cb->board[square.rank][square.file]->valid_max_len *= 2;
          cb->board[square.rank][square.file]->valid_moves = realloc(cb->board[square.rank][square.file]->valid_moves, cb->board[square.rank][square.file]->valid_max_len * sizeof(struct square));
      }
      cb->board[square.rank][square.file]->valid_moves[cb->board[square.rank][square.file]->valid_len].rank = new_rank;
      cb->board[square.rank][square.file]->valid_moves[cb->board[square.rank][square.file]->valid_len].file = new_file;
      ++cb->board[square.rank][square.file]->valid_len;
    }
  }
  // check for diagonally to right
  new_rank = square.rank + flip;
  new_file = square.file + 1;
  if (in_board(new_rank, new_file)) {
    // check for piece of opposite color
    if (cb->board[new_rank][new_file] && cb->board[square.rank][square.file]->color != cb->board[new_rank][new_file]->color) {
      // if true add that square to list fo valid moves
      // double vector if needed
      if (cb->board[square.rank][square.file]->valid_len == cb->board[square.rank][square.file]->valid_max_len) {
         cb->board[square.rank][square.file]->valid_max_len *= 2;
          cb->board[square.rank][square.file]->valid_moves = realloc(cb->board[square.rank][square.file]->valid_moves, cb->board[square.rank][square.file]->valid_max_len * sizeof(struct square));
      }
      cb->board[square.rank][square.file]->valid_moves[cb->board[square.rank][square.file]->valid_len].rank = new_rank;
      cb->board[square.rank][square.file]->valid_moves[cb->board[square.rank][square.file]->valid_len].file = new_file;
      ++cb->board[square.rank][square.file]->valid_len;
    }
  }
  
  // check for forward 2 squares
  new_rank = square.rank + flip + flip;
  new_file = square.file;
  if (in_board(new_rank, new_file) && !cb->board[square.rank][square.file]->moved) {
    if (!cb->board[square.rank + flip][new_file] && !cb->board[new_rank][new_file]) {
      // if true add that square to list fo valid moves
      // double vector if needed
      if (cb->board[square.rank][square.file]->valid_len == cb->board[square.rank][square.file]->valid_max_len) {
         cb->board[square.rank][square.file]->valid_max_len *= 2;
          cb->board[square.rank][square.file]->valid_moves = realloc(cb->board[square.rank][square.file]->valid_moves, cb->board[square.rank][square.file]->valid_max_len * sizeof(struct square));
      }
      cb->board[square.rank][square.file]->valid_moves[cb->board[square.rank][square.file]->valid_len].rank = new_rank;
      cb->board[square.rank][square.file]->valid_moves[cb->board[square.rank][square.file]->valid_len].file = new_file;
      ++cb->board[square.rank][square.file]->valid_len;
    }
  }

  // check for moving one square
  new_rank = square.rank + flip;
  new_file = square.file;
  if (in_board(new_rank, new_file)) {
    if (!cb->board[new_rank][new_file]) {
      // if true add that square to list fo valid moves
      // double vector if needed
      if (cb->board[square.rank][square.file]->valid_len == cb->board[square.rank][square.file]->valid_max_len) {
         cb->board[square.rank][square.file]->valid_max_len *= 2;
          cb->board[square.rank][square.file]->valid_moves = realloc(cb->board[square.rank][square.file]->valid_moves, cb->board[square.rank][square.file]->valid_max_len * sizeof(struct square));
      }
      cb->board[square.rank][square.file]->valid_moves[cb->board[square.rank][square.file]->valid_len].rank = new_rank;
      cb->board[square.rank][square.file]->valid_moves[cb->board[square.rank][square.file]->valid_len].file = new_file;
      ++cb->board[square.rank][square.file]->valid_len;
    }
  } 
}

// TODO: ADD PINS TO REQUIREMNTS, NEEDS MORE TESTING
static void rook_move(struct chess_board *cb, struct square square) {
  assert(cb);
  // check for horizontal sliding to the left / right
  for (int l_r = -1; l_r <= 1; l_r += 2) {
    for (int i = 1; i < BOARD_DIMENSION; ++i) {
      int new_file = square.file + i * l_r;
      // check if squares are in_board
      if (in_board(square.rank, new_file)) {
        // double vector if needed
        if (cb->board[square.rank][square.file]->valid_len == cb->board[square.rank][square.file]->valid_max_len) {
          cb->board[square.rank][square.file]->valid_max_len *= 2;
          cb->board[square.rank][square.file]->valid_moves = realloc(cb->board[square.rank][square.file]->valid_moves, cb->board[square.rank][square.file]->valid_max_len * sizeof(struct square));
        }
        // check if square is occupied,if not add, if by same color dont add, if different then add
        if (!cb->board[square.rank][new_file]) {
          cb->board[square.rank][square.file]->valid_moves[cb->board[square.rank][square.file]->valid_len].rank = square.rank;
          cb->board[square.rank][square.file]->valid_moves[cb->board[square.rank][square.file]->valid_len].file = new_file;
          ++cb->board[square.rank][square.file]->valid_len;
        }
        else if (cb->board[square.rank][new_file] && cb->board[square.rank][new_file]->color == cb->board[square.rank][square.file]->color) {
          break;
        }
        else if (cb->board[square.rank][new_file] && cb->board[square.rank][new_file]->color != cb->board[square.rank][square.file]->color) {
          cb->board[square.rank][square.file]->valid_moves[cb->board[square.rank][square.file]->valid_len].rank = square.rank;
          cb->board[square.rank][square.file]->valid_moves[cb->board[square.rank][square.file]->valid_len].file = new_file;
          ++cb->board[square.rank][square.file]->valid_len;
          break;
        }
      }
    }
  }
  // check for vertical sliding up / down
  for (int u_d = -1; u_d <= 1; u_d += 2) {
    for (int i = 1; i < BOARD_DIMENSION; ++i) {
      int new_rank = square.rank + i * u_d;
      // check if squares are in_board
      if (in_board(new_rank, square.file)) {
        // double vector if needed
        if (cb->board[square.rank][square.file]->valid_len == cb->board[square.rank][square.file]->valid_max_len) {
          cb->board[square.rank][square.file]->valid_max_len *= 2;
          cb->board[square.rank][square.file]->valid_moves = realloc(cb->board[square.rank][square.file]->valid_moves, cb->board[square.rank][square.file]->valid_max_len * sizeof(struct square));
        }
        // check if square is occupied,if not add, if by same color dont add, if different then add
        if (!cb->board[new_rank][square.file]) {
          cb->board[square.rank][square.file]->valid_moves[cb->board[square.rank][square.file]->valid_len].rank = new_rank;
          cb->board[square.rank][square.file]->valid_moves[cb->board[square.rank][square.file]->valid_len].file = square.file;
          ++cb->board[square.rank][square.file]->valid_len;
        }
        else if (cb->board[new_rank][square.file] && cb->board[new_rank][square.file]->color == cb->board[square.rank][square.file]->color) {
          break;
        }
        else if (cb->board[new_rank][square.file] && cb->board[new_rank][square.file]->color != cb->board[square.rank][square.file]->color) {
          cb->board[square.rank][square.file]->valid_moves[cb->board[square.rank][square.file]->valid_len].rank = new_rank;
          cb->board[square.rank][square.file]->valid_moves[cb->board[square.rank][square.file]->valid_len].file = square.file;
          ++cb->board[square.rank][square.file]->valid_len;
          break;
        }
      }
    }
  }
}

static void knight_move(struct chess_board *cb, struct square square) {
  assert(cb);
  // check for forward/backwards knight movements
  for (int u_d = -1; u_d <= 1; u_d += 2) {
    int new_rank = square.rank + 2 * u_d;
    for (int l_r = -1; l_r <= 1; l_r += 2) {
      int new_file = square.file + 1 * l_r;
      // check if new square is in_board
      if (in_board(new_rank, new_file)) {
        // double vector if needed
        if (cb->board[square.rank][square.file]->valid_len == cb->board[square.rank][square.file]->valid_max_len) {
          cb->board[square.rank][square.file]->valid_max_len *= 2;
          cb->board[square.rank][square.file]->valid_moves = realloc(cb->board[square.rank][square.file]->valid_moves, cb->board[square.rank][square.file]->valid_max_len * sizeof(struct square));
        }
        // check if square is unoccupied, occupied by the same color, or by different color
        // unoccupied
        if (!cb->board[new_rank][new_file]) {
          cb->board[square.rank][square.file]->valid_moves[cb->board[square.rank][square.file]->valid_len].rank = new_rank;
          cb->board[square.rank][square.file]->valid_moves[cb->board[square.rank][square.file]->valid_len].file = new_file;
          ++cb->board[square.rank][square.file]->valid_len;
        }
        // diff color
        else if (cb->board[new_rank][new_file] && cb->board[new_rank][new_file]->color != cb->board[square.rank][square.file]->color) {
          cb->board[square.rank][square.file]->valid_moves[cb->board[square.rank][square.file]->valid_len].rank = new_rank;
          cb->board[square.rank][square.file]->valid_moves[cb->board[square.rank][square.file]->valid_len].file = new_file;
          ++cb->board[square.rank][square.file]->valid_len;
        }
        // else do nothing (same color on square)
      }
    }
  }
  // check for left/right movements
  for (int u_d = -1; u_d <= 1; u_d += 2) {
    int new_rank = square.rank + 1 * u_d;
    for (int l_r = -1; l_r <= 1; l_r += 2) {
      int new_file = square.file + 2 * l_r;
      // check if new square is in_board
      if (in_board(new_rank, new_file)) {
        // double vector if needed
        if (cb->board[square.rank][square.file]->valid_len == cb->board[square.rank][square.file]->valid_max_len) {
          cb->board[square.rank][square.file]->valid_max_len *= 2;
          cb->board[square.rank][square.file]->valid_moves = realloc(cb->board[square.rank][square.file]->valid_moves, cb->board[square.rank][square.file]->valid_max_len * sizeof(struct square));
        }
        // check if square is unoccupied, occupied by the same color, or by different color
        // unoccupied
        if (!cb->board[new_rank][new_file]) {
          cb->board[square.rank][square.file]->valid_moves[cb->board[square.rank][square.file]->valid_len].rank = new_rank;
          cb->board[square.rank][square.file]->valid_moves[cb->board[square.rank][square.file]->valid_len].file = new_file;
          ++cb->board[square.rank][square.file]->valid_len;
        }
        // diff color
        else if (cb->board[new_rank][new_file] && cb->board[new_rank][new_file]->color != cb->board[square.rank][square.file]->color) {
          cb->board[square.rank][square.file]->valid_moves[cb->board[square.rank][square.file]->valid_len].rank = new_rank;
          cb->board[square.rank][square.file]->valid_moves[cb->board[square.rank][square.file]->valid_len].file = new_file;
          ++cb->board[square.rank][square.file]->valid_len;
        }
        // else do nothing (same color on square)
      }
    }
  }
}

static void bishop_move(struct chess_board *cb, struct square square) {
  assert(cb);
  // for diagonally moves, goes 1) down/right 2) up/left 3)down/left 4)up/right
  for (int swap_diag = -1; swap_diag <= 1; swap_diag += 2) {
    for (int l_r = -1; l_r <= 1; l_r += 2) {
      for (int i = 1; i < BOARD_DIMENSION; ++i) {
        int new_file = (square.file + i * l_r) * swap_diag;
        int new_rank = square.rank + i * l_r;
        // check if squares are in_board
        if (in_board(new_rank, new_file)) {
          // double vector if needed
          if (cb->board[square.rank][square.file]->valid_len == cb->board[square.rank][square.file]->valid_max_len) {
            cb->board[square.rank][square.file]->valid_max_len *= 2;
            cb->board[square.rank][square.file]->valid_moves = realloc(cb->board[square.rank][square.file]->valid_moves, cb->board[square.rank][square.file]->valid_max_len * sizeof(struct square));
          }
          // check if square is occupied,if not add, if by same color dont add, if different then add
          if (!cb->board[new_rank][new_file]) {
            cb->board[square.rank][square.file]->valid_moves[cb->board[square.rank][square.file]->valid_len].rank = new_rank;
            cb->board[square.rank][square.file]->valid_moves[cb->board[square.rank][square.file]->valid_len].file = new_file;
            ++cb->board[square.rank][square.file]->valid_len;
          }
          else if (cb->board[new_rank][new_file] && cb->board[new_rank][new_file]->color == cb->board[square.rank][square.file]->color) {
            break;
          }
          else if (cb->board[new_rank][new_file] && cb->board[new_rank][new_file]->color != cb->board[square.rank][square.file]->color) {
            cb->board[square.rank][square.file]->valid_moves[cb->board[square.rank][square.file]->valid_len].rank = new_rank;
            cb->board[square.rank][square.file]->valid_moves[cb->board[square.rank][square.file]->valid_len].file = new_file;
            ++cb->board[square.rank][square.file]->valid_len;
            break;
          }
        }
      }
    }
  }
}

static void queen_move(struct chess_board *cb, struct square square) {
  assert(cb);
  // queen just moves like a bishop and rook
  bishop_move(cb, square);
  rook_move(cb, square);
}

static void king_move(struct chess_board *cb, struct square square) {
  assert(cb);
  // diagonal king movement and horizontal / vertical movement
  for (int u_d = -1; u_d <= 1; ++u_d) {
    for (int l_r = -1; l_r <= 1; ++l_r) {
      int new_rank = square.rank + u_d;
      int new_file = square.file + l_r;
      if (in_board(new_rank, new_file)) {
        // double vector if needed
        if (cb->board[square.rank][square.file]->valid_len == cb->board[square.rank][square.file]->valid_max_len) {
            cb->board[square.rank][square.file]->valid_max_len *= 2;
            cb->board[square.rank][square.file]->valid_moves = realloc(cb->board[square.rank][square.file]->valid_moves, cb->board[square.rank][square.file]->valid_max_len * sizeof(struct square));
        }
        // check for free square or if square is occupied by diff color
        if (!cb->board[new_rank][new_file]) {
            cb->board[square.rank][square.file]->valid_moves[cb->board[square.rank][square.file]->valid_len].rank = new_rank;
            cb->board[square.rank][square.file]->valid_moves[cb->board[square.rank][square.file]->valid_len].file = new_file;
            ++cb->board[square.rank][square.file]->valid_len;
        }
        else if (cb->board[new_rank][new_file] && cb->board[new_rank][new_file]->color != cb->board[square.rank][square.file]->color) {
            cb->board[square.rank][square.file]->valid_moves[cb->board[square.rank][square.file]->valid_len].rank = new_rank;
            cb->board[square.rank][square.file]->valid_moves[cb->board[square.rank][square.file]->valid_len].file = new_file;
            ++cb->board[square.rank][square.file]->valid_len;
        }
        // else same color piece on new_sqaure, do nothing 
      }
    }
  }
}

struct chess_board *create_board(void) {
  struct chess_board *cb = malloc(sizeof(struct chess_board));
  cb->w_len = cb->b_len = 16;
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
        cb->board[i][j]->square.rank = i;
        cb->board[i][j]->square.file  = j;
        cb->board[i][j]->valid_len = 0;
        cb->board[i][j]->valid_max_len = 1;
        cb->board[i][j]->valid_moves = malloc(sizeof(struct square));
      }
      else if (i == 0 || i == BOARD_DIMENSION - 1) {
        cb->board[i][j] = malloc(sizeof(struct chess_piece));
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
        cb->board[i][j]->square.file = j;
        cb->board[i][j]->valid_len = 0;
        cb->board[i][j]->valid_max_len = 1;
        cb->board[i][j]->valid_moves = malloc(sizeof(struct square));
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
      }
      else {
        cb->board[i][j] = NULL;
      }
    }
  }
  // load in all valid starting moves
  for (int i = 0; i < BOARD_DIMENSION; ++i) {
    for (int j = 0; j < BOARD_DIMENSION; ++j) {
      if (i == 1 || i == BOARD_DIMENSION - 2) {
        pawn_move(cb, cb->board[i][j]->square, cb->board[i][j]->color);
      }
      else if (i == 0 || i == BOARD_DIMENSION - 1) {
        if (j == 0 || j == BOARD_DIMENSION - 1) {
          rook_move(cb, cb->board[i][j]->square);
        }
        else if (j == 1 || j == BOARD_DIMENSION - 2) {
          knight_move(cb, cb->board[i][j]->square);
        }
        else if (j == 2 || j == BOARD_DIMENSION - 3) {
          bishop_move(cb, cb->board[i][j]->square);
        }
        else if (j == 3) {
          queen_move(cb, cb->board[i][j]->square);
        }
        else {
          king_move(cb, cb->board[i][j]->square);
        }
      }
      // else piece is NULL
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
      if (j == 0) {
        printf("  ");
      }
      printf("----");
    }
    printf("\n");
    if (i != 0) {
      for (int j = 0; j < BOARD_DIMENSION; ++j) {
        if (j == 0) {
          printf("%d ", i);
        }
        if (cb->board[i - 1][j]) {
          printf("|%c%c ", cb->board[i - 1][j]->color, cb->board[i - 1][j]->piece);
        }
        else {
          printf("|   ");
        }
      }
      printf("|\n");
    }
    else {
      for (int x = 0; x < BOARD_DIMENSION; ++x) {
        if (x == 0) {
          printf("  ");
        }
        printf("  %c ", 'a' + x);
      }
      printf("\n");
    }
  }
}
