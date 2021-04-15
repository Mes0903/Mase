#ifndef MAZE_SOLVING_H
#define MAZE_SOLVING_H

#include "mainwindow.h"
#include "mase.h"


class Maze_Solving {
  private:
    Maze_Solving();
    ~Maze_Solving();
    static Maze_Solving* slv;
    Mase* M;

  public:
    static Maze_Solving* getInstance();
    inline bool is_in_maze( const int &y, const int &x );
    bool dfs( std::pair<int, int> position);
    void bfs( const int &first_y, const int &first_x);
    void ucs( const int &first_y, const int &first_x);
};

#endif // MAZE_SOLVING_H
