#include "maze_solving.h"
#include "mainwindow.h"

Maze_Solving* Maze_Solving::solve = nullptr;

Maze_Solving* Maze_Solving::getInstance(){
    if(solve == nullptr)
        solve = new Maze_Solving();

    return solve;
}

Maze_Solving::Maze_Solving() {
}

Maze_Solving::~Maze_Solving() {
}

inline bool Maze_Solving::is_in_maze(const int &y, const int &x) {
    return ( y < MAZE_HEIGHT ) && ( x < MAZE_WIDTH ) && ( y >= 0 ) && ( x >= 0 );
}


bool Maze_Solving::dfs( std::pair<int, int> position , MainWindow* w){
    w->maze[position.first][position.second] = 2;
    w->repaint();
    QThread::msleep( 40 );
    if ( position.first == END_Y && position.second == END_X )
        return true;
    for ( const auto &[dir_y, dir_x] : directions ) {
        auto temp = std::make_pair(position.first + dir_y, position.second + dir_x);
        if ( is_in_maze( temp.first, temp.second ) ) {
            if ( w->maze[temp.first][temp.second] == 0 )
                if ( Maze_Solving::dfs( temp, w ) )
                    return true;
        }
    }
    return false;

}

void Maze_Solving::bfs( const int &first_y, const int &first_x , MainWindow* w){
    std::queue<std::pair<int, int>> result;
    result.push( std::make_pair( first_y, first_x ) );
    w->maze[first_y][first_x] = 2;

    while ( true ) {
        const auto &[temp_y, temp_x]{ result.front() };

        for ( const auto &dir : directions ) {
            const auto &&[y, x]{ ( int[] ){ temp_y + dir.first, temp_x + dir.second } };

            if ( is_in_maze( y, x ) ) {
                if ( w->maze[y][x] == 0 ) {
                    w->maze[y][x] = 2;
                    w->repaint();
                    QThread::msleep( 40 );
                    if ( y == END_Y && x == END_X )
                        return;
                    result.push( std::make_pair( y, x ) );
                }
            }
        }
        result.pop();
    }
}

void Maze_Solving::ucs( const int &first_y, const int &first_x , MainWindow* w){
    struct Node {
        int distance;    // 曼哈頓距離
        int y;    // y座標
        int x;    // x座標
        Node( int distance, int y, int x ) : distance( distance ), y( y ), x( x ) {}
        bool operator>( const Node &other ) const { return distance > other.distance; }
        bool operator<( const Node &other ) const { return distance < other.distance; }
    };

    std::priority_queue<Node, std::deque<Node>, std::greater<Node>> result;    //待走的結點
    result.emplace( Node( abs( 15 - first_x ) + abs( 10 - first_y ), 1, 0 ) );

    while ( true ) {
        if ( result.empty() )
            return;    //沒找到目標
        const auto temp = result.top();    //目前最優先的結點
        result.pop();    //取出結點判斷

        if ( temp.y == END_Y && temp.x == END_X ) {
            w->maze[temp.y][temp.x] = 2;    //探索過的點要改2
            w->repaint();
            QThread::msleep( 40 );
            return;    //如果取出的是目標就return
        }
        else if ( w->maze[temp.y][temp.x] == 0 ) {
            w->maze[temp.y][temp.x] = 2;    //探索過的點要改2
            w->repaint();
            QThread::msleep( 40 );

            for ( const auto &dir : directions ) {
                const auto &&[y, x]{ ( int[] ){ temp.y + dir.first, temp.x + dir.second } };

                if ( is_in_maze( y, x ) ) {
                    if ( w->maze[y][x] == 0 )    // 如果這個結點還沒走過，就把他加到待走的結點裡
                        result.emplace( Node( temp.distance + abs( 15 - temp.x ) + abs( 10 - temp.y ), y, x ) );
                }
            }
        }
    }

}
