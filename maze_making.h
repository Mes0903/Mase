#ifndef MAZE_MAKING_H
#define MAZE_MAKING_H

#include "maze.h"

class Maze;

class Maze_Making : public Maze {

  public:
    Maze_Making( Animator *A );
    ~Maze_Making();
    inline bool if_in_wall( const int &y, const int &x, const int &delta_y, const int &delta_x );
    void random_prim_make_maze( const int &first_y, const int &first_x );
    void empty_map();
};

#endif    // MAZE_MAKING_H
