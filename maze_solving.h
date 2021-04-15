#ifndef MAZE_SOLVING_H
#define MAZE_SOLVING_H


#include "mainwindow.h"
#include "maze.h"


#include <utility>


class MainWindow;
class Maze;


class Maze_Solving : public Maze {

  public:
    Maze_Solving();
    ~Maze_Solving();
    inline bool is_in_maze( const int &y, const int &x );
    inline int pow_two_norm(const int& y ,const int& x);
    bool dfs( std::pair<int, int> position, MainWindow* W);
    void bfs( const int &first_y, const int &first_x, MainWindow* W);
    void ucs( const int &first_y, const int &first_x, MainWindow* W);
    void greedy( const int &first_y, const int &first_x, MainWindow* W);
};

#endif // MAZE_SOLVING_H
