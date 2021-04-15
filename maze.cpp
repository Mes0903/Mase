#include "maze.h"


Maze::Maze() {

}


Maze::~Maze(){

}


void Maze::reset() {
    for ( int y = 0; y < MAZE_HEIGHT; ++y )
        for ( int x = 0; x < MAZE_WIDTH; ++x )
            if ( maze[y][x] == 2 )
                maze[y][x] = 0;
}
