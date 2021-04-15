#ifndef MAKING_MAZE_H
#define MAKING_MAZE_H


#include "mainwindow.h"
#include "maze.h"


class Maze;
class MainWindow;


class Making_Maze : public Maze{

  public:
    Making_Maze();
    ~Making_Maze();
    inline bool if_in_wall( const int &y, const int &x, const int &delta_y, const int &delta_x );
    void random_prim_make_maze( const int &first_y, const int &first_x, MainWindow* W);
};

#endif // MAKING_MAZE_H
