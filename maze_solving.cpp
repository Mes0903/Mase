#include "maze_solving.h"
#include "animator.h"
#include "mainwindow.h"

#include <QThread>

#include <iostream>
#include <math.h>
#include <queue>

Maze_Solving::Maze_Solving( Animator *A ) : Maze( A ) {
}

Maze_Solving::~Maze_Solving() {
}

//回傳是不是在迷宮裡面 (包含牆壁)
inline bool Maze_Solving::is_in_maze( const int &y, const int &x ) {
    return ( y < MAZE_HEIGHT ) && ( x < MAZE_WIDTH ) && ( y >= 0 ) && ( x >= 0 );
}

//回傳 Two_Norm 的平方
inline int Maze_Solving::pow_two_norm( const int &y, const int &x ) {
    return pow( ( END_Y - y ), 2 ) + pow( ( END_X - x ), 2 );
}

bool Maze_Solving::dfs( const int y, const int x ) {
    maze[1][0] = static_cast<int>( Maze_Elements::BEGIN );    //起點
    animator->update( 10 );
    maze[y][x] = static_cast<int>( Maze_Elements::EXPLORED );    //探索過的點
    animator->update( 10 );
    if ( y == END_Y && x == END_X ) {    //如果到終點了就回傳True
        maze[y][x] = static_cast<int>( Maze_Elements::END );    //終點
        animator->update( 10 );
        return true;
    }
    for ( const auto &[dir_y, dir_x] : directions ) {    //上下左右
        const auto [temp_y, temp_x] = ( int[] ){ y + dir_y, x + dir_x };
        if ( is_in_maze( temp_y, temp_x ) ) {    //如果這個節點在迷宮內
            if ( maze[temp_y][temp_x] == static_cast<int>( Maze_Elements::GROUND ) )    //而且如果這個節點還沒被探索過
                if ( Maze_Solving::dfs( temp_y, temp_x ) )    //就繼續遞迴，如果已經找到目標就會回傳 true ，所以這裡放在 if 裡面
                    return true;
        }
    }
    return false;
}    //end Maze_Solving::dfs

void Maze_Solving::bfs( const int &first_y, const int &first_x ) {
    std::queue<std::pair<int, int>> result;    //存節點的deque
    result.push( std::make_pair( first_y, first_x ) );    //將一開始的節點加入deque
    maze[first_y][first_x] = static_cast<int>( Maze_Elements::BEGIN );    //起點
    animator->update( 5 );

    while ( true ) {
        const auto [temp_y, temp_x]{ result.front() };    //目前的節點
        result.pop();    //將目前的節點拿出來

        for ( const auto &dir : directions ) {    //遍歷上下左右
            const auto [y, x] = ( int[] ){ temp_y + dir.first, temp_x + dir.second };    //上下左右的節點

            if ( is_in_maze( y, x ) ) {    //如果這個節點在迷宮內
                if ( maze[y][x] == static_cast<int>( Maze_Elements::GROUND ) ) {    //而且如果這個節點還沒被探索過，也不是牆壁
                    maze[y][x] = static_cast<int>( Maze_Elements::EXPLORED );    //那就探索他，改 EXPLORED
                    animator->update( 5 );
                    if ( y == END_Y && x == END_X ) {    //找到終點就return
                        maze[y][x] = static_cast<int>( Maze_Elements::END );    //終點
                        animator->update( 5 );
                        return;
                    }
                    else
                        result.push( std::make_pair( y, x ) );    //沒找到節點就加入節點
                }
            }
        }
    }    //end while
}    //end Maze_Solving::bfs

void Maze_Solving::ucs( const int &first_y, const int &first_x, const int &types ) {

    enum class Types : int {
        Manhattan_Distance = 0,    //Cost Function 為 曼哈頓距離，所以距離終點越遠 Cost 越大
        Two_Norm = 1,    //Cost Function 為 Two_Norm，所以距離終點越遠 Cost 越大
        Interval = 2    //Cost Function 以區間來計算，每一個區間 Cost 差10，距離終點越遠 Cost 越大
    };

    struct Node {
        int __Weight;    // 權重 (Cost Function)
        int y;    // y座標
        int x;    // x座標
        Node( int weight, int y, int x ) : __Weight( weight ), y( y ), x( x ) {}
        bool operator>( const Node &other ) const { return __Weight > other.__Weight; }    //priority比大小只看權重
        bool operator<( const Node &other ) const { return __Weight < other.__Weight; }    //priority比大小只看權重
    };

    std::priority_queue<Node, std::deque<Node>, std::greater<Node>> result;    //待走的結點，greater代表小的會在前面，由小排到大
    int weight{};    //用來計算的權重

    switch ( types ) {    //起點
        case static_cast<int>( Types::Manhattan_Distance ):
            weight = abs( END_X - first_x ) + abs( END_Y - first_y );    //權重為曼哈頓距離
            break;
        case static_cast<int>( Types::Two_Norm ):
            weight = pow_two_norm( first_y, first_x );    //權重為 two_norm
            break;
        case static_cast<int>( Types::Interval ):
            constexpr int interval_y = MAZE_HEIGHT / 10, interval_x = MAZE_WIDTH / 10;    //分 10 個區間
            weight = ( static_cast<int>( first_y / interval_y ) < static_cast<int>( first_x / interval_x ) ) ? ( 10 - static_cast<int>( first_y / interval_y ) ) : ( 10 - static_cast<int>( first_x / interval_x ) );    //權重以區間計算，兩個相除是看它在第幾個區間，然後用總區間數減掉，代表它的基礎權重，再乘以1000
            break;
    }

    result.emplace( Node( weight, first_y, first_x ) );    //將起點加進去

    while ( true ) {
        if ( result.empty() )
            return;    //沒找到目標

        const auto temp = result.top();    //目前最優先的結點
        result.pop();    //取出結點判斷

        if ( temp.y == END_Y && temp.x == END_X ) {
            maze[temp.y][temp.x] = static_cast<int>( Maze_Elements::END );    //終點
            animator->update( 10 );
            return;    //如果取出的點是終點就return
        }
        else if ( maze[temp.y][temp.x] == static_cast<int>( Maze_Elements::GROUND ) ) {
            if ( temp.y == first_y && temp.x == first_x )
                maze[temp.y][temp.x] = static_cast<int>( Maze_Elements::BEGIN );    //起點
            else
                maze[temp.y][temp.x] = static_cast<int>( Maze_Elements::EXPLORED );    //探索過的點要改EXPLORED

            animator->update( 10 );
            for ( const auto &dir : directions ) {
                const auto [y, x] = ( int[] ){ temp.y + dir.first, temp.x + dir.second };

                if ( is_in_maze( y, x ) ) {
                    if ( maze[y][x] == static_cast<int>( Maze_Elements::GROUND ) ) {    // 如果這個結點還沒走過，就把他加到待走的結點裡
                        switch ( types ) {
                            case static_cast<int>( Types::Manhattan_Distance ):
                                weight = abs( END_X - x ) + abs( END_Y - y );    //權重為曼哈頓距離
                                break;
                            case static_cast<int>( Types::Two_Norm ):
                                weight = pow_two_norm( y, x );    //權重為 Two_Norm
                                break;
                            case static_cast<int>( Types::Interval ):
                                constexpr int interval_y = MAZE_HEIGHT / 10, interval_x = MAZE_WIDTH / 10;    //分 10 個區間
                                weight = ( static_cast<int>( y / interval_y ) < static_cast<int>( x / interval_x ) ) ? ( 10 - static_cast<int>( y / interval_y ) ) : ( 10 - static_cast<int>( x / interval_x ) );    //權重為區間
                                break;
                        }
                        result.emplace( Node( temp.__Weight + weight, y, x ) );    //加入節點
                    }
                }
            }    //end for
        }
    }    //end while
}    //end Maze_Solving::ucs

void Maze_Solving::greedy( const int &first_y, const int &first_x ) {
    struct Node {
        int __Weight;    //權重為 Two_Norm 平方 (Heuristic function)
        int y;    // y座標
        int x;    // x座標
        Node( int weight, int y, int x ) : __Weight( weight ), y( y ), x( x ) {}
        bool operator>( const Node &other ) const { return __Weight > other.__Weight; }    //priority比大小只看權重
        bool operator<( const Node &other ) const { return __Weight < other.__Weight; }    //priority比大小只看權重
    };

    std::priority_queue<Node, std::deque<Node>, std::greater<Node>> result;    //待走的結點，greater代表小的會在前面，由小排到大
    result.emplace( Node( pow_two_norm( first_y, first_x ), first_y, first_x ) );    //將起點加進去

    while ( true ) {
        if ( result.empty() )
            return;    //沒找到目標
        const auto temp = result.top();    //目前最優先的結點
        result.pop();    //取出結點判斷

        if ( temp.y == END_Y && temp.x == END_X ) {
            maze[temp.y][temp.x] = static_cast<int>( Maze_Elements::END );    //終點
            animator->update( 10 );
            return;    //如果取出的點是終點就return
        }
        else if ( maze[temp.y][temp.x] == static_cast<int>( Maze_Elements::GROUND ) ) {
            if ( temp.y == first_y && temp.x == first_x )
                maze[temp.y][temp.x] = static_cast<int>( Maze_Elements::BEGIN );    //起點
            else
                maze[temp.y][temp.x] = static_cast<int>( Maze_Elements::EXPLORED );    //探索過的點要改EXPLORED

            animator->update( 10 );
            for ( const auto &dir : directions ) {
                const auto [y, x] = ( int[] ){ temp.y + dir.first, temp.x + dir.second };

                if ( is_in_maze( y, x ) ) {
                    if ( maze[y][x] == static_cast<int>( Maze_Elements::GROUND ) )    // 如果這個結點還沒走過，就把他加到待走的結點裡
                        result.emplace( Node( pow_two_norm( y, x ), y, x ) );
                }
            }
        }
    }    //end while
}    //end Maze_Solving::greedy

void Maze_Solving::a_star( const int &first_y, const int &first_x, const int &types ) {

    enum class Types : int {
        Normal = 0,    //Cost Function 為 50
        Interval = 1,    //Cost Function 以區間來計算，每一個區間 Cost 差10，距離終點越遠 Cost 越大
    };

    struct Node {
        int __Cost;    //Cost Function 有兩種，以區間計算，每個區間 Cost 差10
        int __Weight;    //權重以區間(Cost Function) + Two_Norm 平方(Heuristic Function) 計算，每個區間 Cost 差1000
        int y;    // y座標
        int x;    // x座標
        Node( int cost, int weight, int y, int x ) : __Cost( cost ), __Weight( weight ), y( y ), x( x ) {}
        bool operator>( const Node &other ) const { return __Weight > other.__Weight; }    //priority比大小只看權重
        bool operator<( const Node &other ) const { return __Weight < other.__Weight; }    //priority比大小只看權重
    };

    std::priority_queue<Node, std::deque<Node>, std::greater<Node>> result;    //待走的結點，greater代表小的會在前面，由小排到大
    constexpr int interval_y = MAZE_HEIGHT / 10, interval_x = MAZE_WIDTH / 10;    //分 10 個區間
    int cost{}, weight{};

    if ( types == static_cast<int>( Types::Normal ) ) {
        cost = 50;
        weight = cost + abs( END_X - first_x ) + abs( END_Y - first_y );
    }
    else if ( types == static_cast<int>( Types::Interval ) ) {
        cost = ( static_cast<int>( first_y / interval_y ) < static_cast<int>( first_x / interval_x ) ) ? ( 10 - static_cast<int>( first_y / interval_y ) ) * 10 : ( 10 - static_cast<int>( first_x / interval_x ) ) * 10;    //Cost 以區間計算，兩個相除是看它在第幾個區間，然後用總區間數減掉，代表它的基礎 Cost，再乘以1000
        weight = cost + pow_two_norm( first_y, first_x );    //權重以區間(Cost) + Two_Norm 計算
    }
    result.emplace( Node( cost, weight, first_y, first_x ) );    //將起點加進去

    while ( true ) {
        if ( result.empty() )
            return;    //沒找到目標
        const auto temp = result.top();    //目前最優先的結點
        result.pop();    //取出結點

        if ( temp.y == END_Y && temp.x == END_X ) {
            maze[temp.y][temp.x] = static_cast<int>( Maze_Elements::END );    //終點
            animator->update( 5 );
            return;    //如果取出的點是終點就return
        }
        else if ( maze[temp.y][temp.x] == static_cast<int>( Maze_Elements::GROUND ) ) {
            if ( temp.y == first_y && temp.x == first_x )
                maze[temp.y][temp.x] = static_cast<int>( Maze_Elements::BEGIN );    //起點
            else
                maze[temp.y][temp.x] = static_cast<int>( Maze_Elements::EXPLORED );    //探索過的點要改EXPLORED

            animator->update( 5 );
            for ( const auto &dir : directions ) {
                const auto [y, x] = ( int[] ){ temp.y + dir.first, temp.x + dir.second };

                if ( is_in_maze( y, x ) ) {
                    if ( maze[y][x] == static_cast<int>( Maze_Elements::GROUND ) ) {    // 如果這個結點還沒走過，就把他加到待走的結點裡
                        if ( types == static_cast<int>( Types::Normal ) ) {
                            cost = 50;    //cost function設為常數 50
                            weight = cost + abs( END_X - x ) + abs( END_Y - y );    //heuristic function 設為曼哈頓距離
                        }
                        else if ( types == static_cast<int>( Types::Interval ) ) {
                            cost = ( static_cast<int>( y / interval_y ) < static_cast<int>( x / interval_x ) ) ? temp.__Cost + ( 10 - static_cast<int>( y / interval_y ) ) * 10 : temp.__Cost + ( 10 - static_cast<int>( x / interval_x ) ) * 10;    //Cost 以區間計算，兩個相除是看它在第幾個區間，然後用總區間數減掉，代表它的基礎 Cost，再乘以1000
                            weight = cost + pow_two_norm( y, x );    //heuristic function 設為 two_norm 平方
                        }
                        result.emplace( Node( cost, weight, y, x ) );
                    }
                }
            }
        }
    }    //end while
}    //end Maze_Solving::a_star
