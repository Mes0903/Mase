#ifndef MAKING_MAZE_H
#define MAKING_MAZE_H

#include "maze.h"

class Maze;

class Making_Maze : public Maze {

  public:
    Making_Maze(Animator* A);
    ~Making_Maze();
    inline bool if_in_wall( const int &y, const int &x, const int &delta_y, const int &delta_x );
    void random_prim_make_maze( const int &first_y, const int &first_x );
};

#endif    // MAKING_MAZE_H
