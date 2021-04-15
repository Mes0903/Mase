#ifndef MAKING_MAZE_H
#define MAKING_MAZE_H

#include "mainwindow.h"
#include "mase.h"

class Making_Maze {
  private:
    Making_Maze();
    ~Making_Maze();
    static Making_Maze* mk;
    Mase* M;

  public:
    static Making_Maze* getInstance();
    inline bool if_in_wall( const int &y, const int &x, const int &delta_y, const int &delta_x );
    void random_prim_make_maze( const int &first_y, const int &first_x);
};

#endif // MAKING_MAZE_H
