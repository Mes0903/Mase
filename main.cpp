#include "mainwindow.h"

#include <QApplication>
#include <QColor>
#include <QComboBox>
#include <QDebug>
#include <QFile>
#include <QPainter>
#include <QPushButton>
#include <QTextStream>
#include <QThread>
#include <QTimer>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <queue>
#include <random>
#include <stack>
#include <utility>
#include <vector>

constexpr int MAZE_HEIGHT = 13;
constexpr int MAZE_WIDTH = 17;
constexpr int END_Y = MAZE_HEIGHT - 2;
constexpr int END_X = MAZE_WIDTH - 1;

template <typename T1, typename T2>
std::pair<T1, T2> operator+( const std::pair<T1, T2> &p1, const std::pair<T1, T2> &p2 ) {
    return std::move( std::make_pair( p1.first + p2.first, p1.second + p2.second ) );
}

bool if_in_maze( const int &y, const int &x, const int &delta_y, const int &delta_x ) {
    return ( y + delta_y < MAZE_HEIGHT - 1 ) && ( x + delta_x < MAZE_WIDTH - 1 ) && ( y + delta_y > 0 ) && ( x + delta_x > 0 );    // 下牆、右牆、上牆、左牆
};

bool is_in_maze(const int &y, const int &x) {
    return (y < MAZE_HEIGHT) && (x < MAZE_WIDTH) && (y >= 0) && (x >= 0);
}

class PaintWindow : public MainWindow {
  private:
    const std::pair<int, int> directions[4]{ { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };

  public:
    int maze[MAZE_HEIGHT][MAZE_WIDTH]{{0}};

    bool dfs( std::pair<int, int> position ) {
        maze[position.first][position.second] = 2;
        repaint();
        QThread::msleep( 50 );
        if ( position.first == END_Y && position.second == END_X )
            return true;
        for ( const auto &dir : directions ) {
            auto temp = position + dir;
            if ( maze[temp.first][temp.second] == 0 ) {
                if ( dfs( temp ) )
                    return true;
            }
        }
        return false;
    }

    void bfs( const int &first_y, const int &first_x ) {
        std::queue<std::pair<int, int>> result;
        result.push( std::make_pair( first_y, first_x ) );
        maze[first_y][first_x] = 2;

        while ( true ) {
            const auto &[temp_y, temp_x]{ result.front() };

            for ( const auto &dir : directions ) {
                const auto &&[y, x]{ ( int[] ){ temp_y + dir.first, temp_x + dir.second } };

                if ( is_in_maze( y, x ) ) {
                    if ( maze[y][x] == 0 ) {
                        maze[y][x] = 2;
                        repaint();
                        QThread::msleep( 50 );
                        if ( y == END_Y && x == END_X )
                            return;
                        result.push( std::make_pair( y, x ) );
                    }
                }
            }
            result.pop();
        }
    }

    void ucs( const int &first_y, const int &first_x ) {
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
                maze[temp.y][temp.x] = 2;    //探索過的點要改2
                repaint();
                QThread::msleep( 50 );
                return;    //如果取出的是目標就return
            }
            else if ( maze[temp.y][temp.x] == 0 ) {
                maze[temp.y][temp.x] = 2;    //探索過的點要改2
                repaint();
                QThread::msleep( 50 );

                for ( const auto &dir : directions ) {
                    const auto &&[y, x]{ ( int[] ){ temp.y + dir.first, temp.x + dir.second } };

                    if ( is_in_maze( y, x ) ) {
                        if ( maze[y][x] == 0 )     // 如果這個結點還沒走過，就把他加到待走的結點裡
                            result.emplace( Node( temp.distance + abs( 15 - temp.x ) + abs( 10 - temp.y ), y, x ) );
                    }
                }
            }
        }
    }

    void random_prim_make_maze(const int& first_y, const int& first_x) {
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

                repaint();
                QThread::msleep( 50 );
            }
        }

        std::srand( std::chrono::system_clock::now().time_since_epoch().count() );    //設定隨機數種子
        int seed_y, seed_x;    //一開始 x,y 座標
        while ( true ) {
            seed_y = ( rand() % ( MAZE_HEIGHT - 2 ) ) + 1;
            seed_x = ( rand() % ( MAZE_WIDTH - 2 ) ) + 1;
            if ( seed_x % 2 == 1 && seed_y % 2 == 1 ) {
                std::cout << "begin y x:" << seed_y << " " << seed_x << '\n';
                maze[seed_y][seed_x] = 2;    // 隨機挑一點當作生成的起點
                re_load.emplace_back( std::make_pair( seed_y, seed_x ) );    //加進之後要改回道路的清單裡
                break;
            }
        }

        auto if_in_maze = []( const int &y, const int &x, const int &dir_y, const int &dir_x ) -> bool {
            return ( y + dir_y < MAZE_HEIGHT - 1 ) && ( x + dir_x < MAZE_WIDTH - 1 ) && ( y + dir_y > 0 ) && ( x + dir_x > 0 );    // 下牆、右牆、上牆、左牆
        };

        std::deque<std::pair<int, int>> wall;    //待找的牆的列表
        for ( const auto &[dir_y, dir_x] : directions ) {
            if ( if_in_maze( seed_y, seed_x, dir_y, dir_x ) )
                wall.emplace_back( std::make_pair( seed_y + dir_y, seed_x + dir_x ) );    //將起點四周在迷宮內的牆加入wall列表中
        }

        int random_index;    //要打通的牆的index，隨機找
        int up_node, down_node, left_node, right_node;    //目前這個牆的上下左右結點
        while ( !wall.empty() ) {
            random_index = rand() % wall.size();
            const auto [temp_y, temp_x]{ wall.at( random_index ) };

            if ( maze[temp_y][temp_x] == 1 ) {    //如果抽到的那格確定是牆再去判斷，有時候會有一個牆重複被加到清單裡的情形

                if ( if_in_maze( temp_y, temp_x, -1, 0 ) )
                    up_node = maze[temp_y - 1][temp_x];
                if ( if_in_maze( temp_y, temp_x, 1, 0 ) )
                    down_node = maze[temp_y + 1][temp_x];
                if ( if_in_maze( temp_y, temp_x, 0, -1 ) )
                    left_node = maze[temp_y][temp_x - 1];
                if ( if_in_maze( temp_y, temp_x, 0, 1 ) )
                    right_node = maze[temp_y][temp_x + 1];

                if ( ( up_node == 2 && down_node == 2 ) || ( left_node == 2 && right_node == 2 ) )
                    wall.erase( wall.begin() + random_index );    //如果「上下都走過」或「左右都走過」，那麼就把這個牆留著
                else {
                    //不然就把牆打通
                    maze[temp_y][temp_x] = 2;
                    re_load.emplace_back( std::make_pair( temp_y, temp_x ) );
                    repaint();
                    QThread::msleep( 50 );
                    wall.erase( wall.begin() + random_index );

                    if ( up_node == 2 && down_node == 0 ) {
                        maze[temp_y + 1][temp_x] = 2;
                        re_load.emplace_back( std::make_pair( temp_y + 1, temp_x ) );
                        repaint();
                        QThread::msleep( 50 );
                        for ( const auto &[dir_y, dir_x] : directions ) {
                            if ( if_in_maze( temp_y + 1, temp_x, dir_y, dir_x ) ) {    // 在迷宮內
                                if ( maze[temp_y + 1 + dir_y][temp_x + dir_x] == 1 )
                                    wall.emplace_back( std::make_pair( temp_y + 1 + dir_y, temp_x + dir_x ) );    //將起點四周在迷宮內的牆加入wall列表中
                            }
                        }

                        re_load.emplace_back( std::make_pair( temp_y, temp_x ) );
                    }
                    else if ( up_node == 0 && down_node == 2 ) {
                        maze[temp_y - 1][temp_x] = 2;
                        re_load.emplace_back( std::make_pair( temp_y - 1, temp_x ) );
                        repaint();
                        QThread::msleep( 50 );
                        for ( const auto &[dir_y, dir_x] : directions ) {
                            if ( if_in_maze( temp_y - 1, temp_x, dir_y, dir_x ) ) {    // 在迷宮內
                                if ( maze[temp_y - 1 + dir_y][temp_x + dir_x] == 1 )
                                    wall.emplace_back( std::make_pair( temp_y - 1 + dir_y, temp_x + dir_x ) );    //將起點四周在迷宮內的牆加入wall列表中
                            }
                        }
                        re_load.emplace_back( std::make_pair( temp_y, temp_x ) );
                    }
                    else if ( left_node == 2 && right_node == 0 ) {
                        maze[temp_y][temp_x + 1] = 2;
                        re_load.emplace_back( std::make_pair( temp_y, temp_x + 1 ) );
                        repaint();
                        QThread::msleep( 50 );
                        for ( const auto &[dir_y, dir_x] : directions ) {
                            if ( if_in_maze( temp_y, temp_x + 1, dir_y, dir_x ) ) {    // 在迷宮內
                                if ( maze[temp_y + dir_y][temp_x + 1 + dir_x] == 1 )
                                    wall.emplace_back( std::make_pair( temp_y + dir_y, temp_x + 1 + dir_x ) );    //將起點四周在迷宮內的牆加入wall列表中
                            }
                        }
                        re_load.emplace_back( std::make_pair( temp_y, temp_x ) );
                    }
                    else if ( left_node == 0 && right_node == 2 ) {    //四種情況
                        maze[temp_y][temp_x - 1] = 2;
                        re_load.emplace_back( std::make_pair( temp_y, temp_x - 1) );
                        repaint();
                        QThread::msleep( 50 );
                        for ( const auto &[dir_y, dir_x] : directions ) {
                            if ( if_in_maze( temp_y, temp_x - 1, dir_y, dir_x ) ) {    // 在迷宮內
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


        for(const auto&[temp_y, temp_x] : re_load){
            maze[temp_y][temp_x] = 0;
            repaint();
            QThread::msleep( 50 );
        }
        maze[first_y][first_x] = 0;
        maze[END_Y][END_X] = 0;
    }

    void reset() {
        for ( int y = 0; y < MAZE_HEIGHT; ++y )
            for ( int x = 0; x < MAZE_WIDTH; ++x )
                if ( maze[y][x] == 2 )
                    maze[y][x] = 0;
    }

    void paintEvent( QPaintEvent * ) override {
        const int GRID_SIZE = 30;
        QPainter painter( this );
        for ( int i = 0; i < MAZE_HEIGHT; ++i ) {
            for ( int j = 0; j < MAZE_WIDTH; ++j ) {
                switch ( maze[i][j] ) {
                    case 1:
                        painter.fillRect( j * GRID_SIZE, i * GRID_SIZE, GRID_SIZE, GRID_SIZE, QColor( qRgb( 115, 64, 70 ) ) );
                        break;
                    case 2:
                        painter.fillRect( j * GRID_SIZE, i * GRID_SIZE, GRID_SIZE, GRID_SIZE, QColor( qRgb( 231, 158, 79 ) ) );
                        break;
                }
                painter.drawRect( j * GRID_SIZE, i * GRID_SIZE, GRID_SIZE, GRID_SIZE );
            }
        }
    }
};

int main( int argc, char *argv[] ) {

    QApplication a( argc, argv );
    PaintWindow w;
    w.show();

    QComboBox box( &w );
    box.show();
    box.addItem( "DFS" );
    box.addItem( "BFS" );
    box.addItem("UCS");
    box.addItem("Gen");

    QPushButton *runButton = w.findChild<QPushButton *>( "runButton" );
    QObject::connect( runButton, &QPushButton::clicked, [&box, &w]() {
        w.reset();
        if ( box.currentText() == "BFS" )
            w.bfs( 1, 0 );
        else if ( box.currentText() == "UCS" )
            w.ucs( 1, 0 );
        else if ( box.currentText() == "DFS" )
            w.dfs( { 1, 0 } );
        else
            w.random_prim_make_maze(1,0);
        w.repaint();
    } );

    QFile file( QString( SOURCE_PATH ) + "/test.txt" );
    file.open( QIODevice::ReadOnly | QIODevice::Text );
    QString buff_line{};

    //for (int i = 0; i < MAZE_HEIGHT; ++i) {
    //    buff_line = file.readLine();
    //    buff_line = buff_line.simplified().remove(' ');
    //    for (int j = 0; j < MAZE_WIDTH; ++j){
    //        w.maze[i][j] = buff_line[j].digitValue();
    //    }
    //}

    return a.exec();
}
