#include "making_maze.h"
#include "mainwindow.h"


#include <QThread>
#include <iostream>
#include <deque>


Making_Maze::Making_Maze() {

}


Making_Maze::~Making_Maze(){

}


inline bool Making_Maze::if_in_wall( const int &y, const int &x, const int &delta_y, const int &delta_x ) {
    return ( y + delta_y < MAZE_HEIGHT - 1 ) && ( x + delta_x < MAZE_WIDTH - 1 ) && ( y + delta_y > 0 ) && ( x + delta_x > 0 );    // 下牆、右牆、上牆、左牆
}


void Making_Maze::random_prim_make_maze( const int &first_y, const int &first_x, MainWindow* W) {
    std::deque<std::pair<int, int>> re_load;    //之後要改回道路的座標清單

    //地圖初始化
    for ( int y = 0; y < MAZE_HEIGHT; ++y ) {
        for ( int x = 0; x < MAZE_WIDTH; ++x ) {
            if ( y == 0 or y == MAZE_HEIGHT - 1 )
                maze[y][x] = 1;
            else if ( x == 0 or x == MAZE_WIDTH - 1 )
                maze[y][x] = 1;
            else if ( x % 2 == 1 && y % 2 == 1 )
                maze[y][x] = 0;
            else
                maze[y][x] = 1;

            //W->repaint();
            //QThread::msleep(1);
        }
    }

    std::srand( std::chrono::system_clock::now().time_since_epoch().count() );    //設定隨機數種子
    int seed_y, seed_x;    //一開始 x,y 座標
    while ( true ) {
        seed_y = ( rand() % ( MAZE_HEIGHT - 2 ) ) + 1;
        seed_x = ( rand() % ( MAZE_WIDTH - 2 ) ) + 1;
        if ( seed_x % 2 == 1 && seed_y % 2 == 1 ) {
            maze[seed_y][seed_x] = 2;    // 隨機挑一點當作生成的起點
            re_load.emplace_back( std::make_pair( seed_y, seed_x ) );    //加進之後要改回道路的清單裡
            break;
        }
    }

    std::deque<std::pair<int, int>> wall;    //待找的牆的列表
    for ( const auto &[dir_y, dir_x] : directions ) {
        if ( if_in_wall( seed_y, seed_x, dir_y, dir_x ) )
            wall.emplace_back( std::make_pair( seed_y + dir_y, seed_x + dir_x ) );    //將起點四周在迷宮內的牆加入wall列表中
    }

    int random_index;    //要打通的牆的index，隨機找
    int up_node, down_node, left_node, right_node;    //目前這個牆的上下左右結點
    while ( !wall.empty() ) {
        random_index = rand() % wall.size();
        const auto [temp_y, temp_x]{ wall.at( random_index ) };

        if ( maze[temp_y][temp_x] == 1 ) {    //如果抽到的那格確定是牆再去判斷，有時候會有一個牆重複被加到清單裡的情形

            if ( if_in_wall( temp_y, temp_x, -1, 0 ) )
                up_node = maze[temp_y - 1][temp_x];
            if ( if_in_wall( temp_y, temp_x, 1, 0 ) )
                down_node = maze[temp_y + 1][temp_x];
            if ( if_in_wall( temp_y, temp_x, 0, -1 ) )
                left_node = maze[temp_y][temp_x - 1];
            if ( if_in_wall( temp_y, temp_x, 0, 1 ) )
                right_node = maze[temp_y][temp_x + 1];

            if ( ( up_node == 2 && down_node == 2 ) || ( left_node == 2 && right_node == 2 ) )
                wall.erase( wall.begin() + random_index );    //如果「上下都走過」或「左右都走過」，那麼就把這個牆留著
            else {
                //不然就把牆打通
                maze[temp_y][temp_x] = 2;
                re_load.emplace_back( std::make_pair( temp_y, temp_x ) );
                W->repaint();
                QThread::msleep(20);
                wall.erase( wall.begin() + random_index );

                if ( up_node == 2 && down_node == 0 ) {
                    maze[temp_y + 1][temp_x] = 2;
                    re_load.emplace_back( std::make_pair( temp_y + 1, temp_x ) );
                    W->repaint();
                    QThread::msleep(20);
                    for ( const auto &[dir_y, dir_x] : directions ) {
                        if ( if_in_wall( temp_y + 1, temp_x, dir_y, dir_x ) ) {    // 在迷宮內
                            if ( maze[temp_y + 1 + dir_y][temp_x + dir_x] == 1 )
                                wall.emplace_back( std::make_pair( temp_y + 1 + dir_y, temp_x + dir_x ) );    //將起點四周在迷宮內的牆加入wall列表中
                        }
                    }

                    re_load.emplace_back( std::make_pair( temp_y, temp_x ) );
                }
                else if ( up_node == 0 && down_node == 2 ) {
                    maze[temp_y - 1][temp_x] = 2;
                    re_load.emplace_back( std::make_pair( temp_y - 1, temp_x ) );
                    W->repaint();
                    QThread::msleep(20);
                    for ( const auto &[dir_y, dir_x] : directions ) {
                        if ( if_in_wall( temp_y - 1, temp_x, dir_y, dir_x ) ) {    // 在迷宮內
                            if ( maze[temp_y - 1 + dir_y][temp_x + dir_x] == 1 )
                                wall.emplace_back( std::make_pair( temp_y - 1 + dir_y, temp_x + dir_x ) );    //將起點四周在迷宮內的牆加入wall列表中
                        }
                    }
                    re_load.emplace_back( std::make_pair( temp_y, temp_x ) );
                }
                else if ( left_node == 2 && right_node == 0 ) {
                    maze[temp_y][temp_x + 1] = 2;
                    re_load.emplace_back( std::make_pair( temp_y, temp_x + 1 ) );
                    W->repaint();
                    QThread::msleep(20);
                    for ( const auto &[dir_y, dir_x] : directions ) {
                        if ( if_in_wall( temp_y, temp_x + 1, dir_y, dir_x ) ) {    // 在迷宮內
                            if ( maze[temp_y + dir_y][temp_x + 1 + dir_x] == 1 )
                                wall.emplace_back( std::make_pair( temp_y + dir_y, temp_x + 1 + dir_x ) );    //將起點四周在迷宮內的牆加入wall列表中
                        }
                    }
                    re_load.emplace_back( std::make_pair( temp_y, temp_x ) );
                }
                else if ( left_node == 0 && right_node == 2 ) {    //四種情況
                    maze[temp_y][temp_x - 1] = 2;
                    re_load.emplace_back( std::make_pair( temp_y, temp_x - 1 ) );
                    W->repaint();
                    QThread::msleep(20);
                    for ( const auto &[dir_y, dir_x] : directions ) {
                        if ( if_in_wall( temp_y, temp_x - 1, dir_y, dir_x ) ) {    // 在迷宮內
                            if ( maze[temp_y + dir_y][temp_x - 1 + dir_x] == 1 )
                                wall.emplace_back( std::make_pair( temp_y + dir_y, temp_x - 1 + dir_x ) );    //將起點四周在迷宮內的牆加入wall列表中
                        }
                    }
                }
                else {
                    std::cout << "test else " << temp_y << " " << temp_x << '\n';
                    break;
                }
            }

            up_node = -1;
            down_node = -1;
            left_node = -1;
            right_node = -1;
        }    // end if(maze[temp_y][temp_x] == 1)
    }    //end while ( !wall.empty() )

    for ( const auto &[temp_y, temp_x] : re_load ) {
        maze[temp_y][temp_x] = 0;
        W->repaint();
        QThread::msleep(20);
    }
    maze[first_y][first_x] = 0;
    maze[END_Y][END_X] = 0;
    W->repaint();
    QThread::msleep(20);
}    // end random_prim_make_maze
