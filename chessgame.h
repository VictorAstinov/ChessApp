#ifndef __CHESSGAME_H_INCLUDED__
#define __CHESSGAME_H_INCLUDED__
#include "chessboard.h"


//struct chess_game;
struct chess_game {
  struct chess_board *cb;
  int move_count;
  char result;
};

struct chess_game *create_game(void);
void destroy_game(struct chess_game *cg);
void play_chess(struct chess_game *cg);
#endif