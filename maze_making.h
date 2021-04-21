#ifndef MAZE_MAKING_H
#define MAZE_MAKING_H

#include "maze.h"
#include <deque>

class Maze;

class Maze_Making : public Maze {

  public:
    Maze_Making( Animator *A );
    ~Maze_Making();
    inline bool if_in_wall( const int &y, const int &x, const int &delta_y, const int &delta_x );
    void empty_map();
    void random_prim_make_maze( const int &first_y, const int &first_x );
    void recursion_make_maze(const int &first_y, const int &first_x);

  private:
    void initializing_maze();
    inline void set_begin_point(int& seed_y, int& seed_x, std::deque<std::pair<int ,int>>& re_load);
};

#endif    // MAZE_MAKING_H
