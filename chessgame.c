#include "chessgame.h"
#include "chessboard.h"
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define CURRENT 'c'
#define WHITE_WIN 'w'
#define BLACK_WIN 'b'
#define QUEEN 'Q'
#define KNIGHT 'N'
#define BISHOP 'B'
#define ROOK 'R'
#define KING 'K'
#define PAWN 'P'
#define BOARD_DIMENSION 8

/*struct chess_game {
  struct chess_board *cb;
  int move_count;
  char result;
};*/

struct chess_game *create_game(void) {
  struct chess_game *cg = malloc(sizeof(struct chess_game));
  cg->cb = create_board();
  cg->move_count = 0;
  cg->result = CURRENT;
  return cg;
}

void destroy_game(struct chess_game *cg) {
  assert(cg);
  destroy_board(cg->cb);
  free(cg);
}

/* static bool search_piece(struct chess_board *cb, int rank, int file, char piece, int *a) {
  assert(cb);
  for (int i = 0; i < cb->len; ++i) {
    if (cb->active[i]->rank == rank && cb->active[i]->file == file && 
        cb->active[i]->piece == piece) {
          *a = i;
          return true;
      }
  }
  return false;
}

static bool move_piece(struct chess_board *cb, int rank, int file, int idx) {
  assert(cb);
  int old_s.rank = cb->active[idx]->rank;
  int old_s.file = cb->active[idx]->file;
  if (!cb->board[rank][file]) {
    cb->board[rank][file] = malloc(sizeof(struct chess_piece));
    cb->board[rank][file]->moved = true;
    cb->board[rank][file]->color = cb->active[idx]->color;
    cb->board[rank][file]->piece = cb->active[idx]->piece;
    cb->board[rank][file]->rank = rank;
    cb->board[rank][file]->file = file;
    free(cb->board[old_s.rank][old_s.file]);
    cb->board[old_s.rank][old_s.file] = NULL;
    return true;
  }
  return false;
} */

static char* make_move(struct chess_game *cg, char color) {
  assert(cg);
  const int input_len = 8;
  char piece = 0;
  struct square old_s = {-1,-1};
  struct square new_s = {-1,-1};
  char input = 0;
  bool first = true;
  char *str = malloc(input_len * sizeof(char));
  for (int i = 0; i < input_len; ++i) {
    str[i] = '\0';
  }
  for (int count = 0; count < input_len - 1; ++count) {
    scanf("%c", &input);
    if (input == '\n' || input == EOF || input == ' ') {
      break;
    }
    str[count] = input;
  }

  for (int i = 0; i < input_len - 1; ++i) {
    if (first) {
      if (str[i] >= 'a' && str[i] <= 'h') {
        piece = PAWN;
        old_s.file = str[i] - 'a';
        old_s.rank = str[i + 1] - '0' - 1;
      }
      else if (str[i] == KING) {
        piece = KING;
        old_s.file = str[i + 1] - 'a';
        old_s.rank = str[i + 2] - '0' - 1;
      }
      else if (str[i] == QUEEN) {
        piece = QUEEN;
        old_s.file = str[i + 1] - 'a';
        old_s.rank = str[i + 2] - '0' - 1;
      }
      else if (str[i] == ROOK) {
        piece = ROOK;
        old_s.file = str[i + 1] - 'a';
        old_s.rank = str[i + 2] - '0' - 1;
      }
      else if (str[i] == BISHOP) {
        piece = BISHOP;
        old_s.file = str[i + 1] - 'a';
        old_s.rank = str[i + 2] - '0' - 1;
      }
      else if (str[i] == KNIGHT) {
        piece = KNIGHT;
        old_s.file = str[i + 1] - 'a';
        old_s.rank = str[i + 2] - '0' - 1;
      }
      else {
        piece = 0;
      }
      first = false;
    }
    if (str[i] == '>') {
      new_s.file = str[i + 1] - 'a';
      new_s.rank = str[i + 2] - '0' - 1;
      break;
    }
  }
  int index = 0;
  // printf("Piece %c, rank %d, file %d\n", piece, old_s.rank, old_s.file);
  /*bool p_exists = search_piece(cg->cb, old_s.rank, old_s.file, piece, &index);
  if (!p_exists) {
    // printf("Search failed");
    free(str);
    str = NULL;
  } */

  bool success = cg->cb->board[old_s.rank][old_s.file]->valid_move(cg->cb, old_s, new_s);
  if (!success) {
    free(str);
    str = NULL;
  }
  return str;
}

void play_chess(struct chess_game *cg) {
  assert(cg);
  printf("Game begins!\n");
  print_board(cg->cb);
  for (; cg->result == CURRENT; ++cg->move_count) {
    char color = 0;
    if (cg->move_count % 2 == 0) {
      printf("White to move:\n");
      color = 'w';
    }
    else {
      printf("Black to move:\n");
      color = 'b';
    }
    while (1) {
      char *move = make_move(cg, color);
      if (move) {
        printf("%s\n", move);
        free(move);
        print_board(cg->cb);
        break;
      }
      else {
        printf("Invalid move!\n");
      }
    }
  }
  if (cg->result == BLACK_WIN) {
    printf("Black wins!\n");
  }
  else if (cg->result == WHITE_WIN) {
    printf("White wins!\n");
  }
  else {
    printf("Draw!\n");
  }
}
