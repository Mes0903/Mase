#ifndef MAZE_MAKING_H
#define MAZE_MAKING_H

#include "maze.h"
#include <deque>

class Maze;

class Maze_Making : public Maze {
public:
  Maze_Making(Animator *A);
  ~Maze_Making();
  inline bool if_in_wall(const int &y, const int &x, const int &delta_y, const int &delta_x);
  void empty_map();
  void setFlag();
  void random_prim_make_maze(const int &types);
  void recursion_make_maze();
  void recursive_division(const int ly, const int lx, const int ry, const int rx);

private:
  void initializing_maze();
  inline void setBeginPoint(int &seed_y, int &seed_x, std::vector<std::pair<int, int>> &re_load);
};

#endif    // MAZE_MAKING_H
