#include "maze.h"
#include "animator.h"
#include "mainwindow.h"
#include <iostream>

int Maze::maze [MAZE_HEIGHT][MAZE_WIDTH] = {{0}};
std::pair<int, int> Maze::Buffer_Node = {-1,-1};

Maze::Maze( Animator *A ) : animator( A ) {
}

Maze::~Maze() {
}

void Maze::reset() {
    for ( int y = 0; y < MAZE_HEIGHT; ++y )
        for ( int x = 0; x < MAZE_WIDTH; ++x )
            if ( maze[y][x] != static_cast<int>( Maze_Elements::WALL ) )
                maze[y][x] = 0;
}

void Maze::reset_wall_around_maze() {
    for ( int y = 0; y < MAZE_HEIGHT; ++y ) {
        for ( int x = 0; x < MAZE_WIDTH; ++x ) {
            if( x == 0 )
                maze[y][0] = static_cast<int>( Maze_Elements::WALL );
            else if( x == MAZE_WIDTH - 1)
                maze[y][MAZE_WIDTH - 1] = static_cast<int>(Maze_Elements::WALL);
            else if( y == 0 )
                maze[0][x] = static_cast<int>(Maze_Elements::WALL);
            else if( y == MAZE_HEIGHT - 1 )
                maze[MAZE_HEIGHT - 1][x] = static_cast<int>(Maze_Elements::WALL);
            else
                maze[y][x] = static_cast<int>( Maze_Elements::GROUND );

        }
        animator->update(1);
    }
}
