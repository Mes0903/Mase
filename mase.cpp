#include "mase.h"

Mase* Mase::M = nullptr;

Mase* Mase::getInstance(){
    if(M == nullptr)
        M = new Mase();

    return M;
}

Mase::Mase() {

}

Mase::~Mase(){

}

void Mase::reset() {
    for ( int y = 0; y < MAZE_HEIGHT; ++y )
        for ( int x = 0; x < MAZE_WIDTH; ++x )
            if ( this->maze[y][x] == 2 )
                this->maze[y][x] = 0;
}
