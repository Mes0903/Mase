#include "maze.h"
#include "animator.h"
#include "mainwindow.h"
#include <iostream>

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
