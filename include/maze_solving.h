#ifndef MAZE_SOLVING_H
#define MAZE_SOLVING_H

#include "maze.h"

#include <utility>

class Maze;

class Maze_Solving : public Maze {

  public:
    Maze_Solving( Animator *A );
    ~Maze_Solving();
    inline bool is_in_maze( const int &y, const int &x );
    inline int pow_two_norm( const int &y, const int &x );
    bool dfs( const int y, const int x );
    void bfs();
    void ucs( const int &types );
    void greedy();
    void a_star( const int &types );
};

#endif    // MAZE_SOLVING_H
