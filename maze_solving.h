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
    bool dfs( const int first_y, const int first_x );
    void bfs( const int &first_y, const int &first_x );
    void ucs( const int &first_y, const int &first_x, const int &types );
    void greedy( const int &first_y, const int &first_x );
    void a_star( const int &first_y, const int &first_x, const int &types );
};

#endif    // MAZE_SOLVING_H
