#include "maze_making.h"
#include "animator.h"
#include "mainwindow.h"
#include "maze.h"

#include <QThread>
#include <algorithm>
#include <deque>
#include <chrono>
#include <stack>
#include <iostream>
#include <random>

Maze_Making::Maze_Making( Animator *__Animator ) : Maze( __Animator ) {
}

Maze_Making::~Maze_Making() {
}

void Maze_Making::set_Flag() {
    maze[BEGIN_Y][BEGIN_X] = static_cast<int>(Maze_Elements::BEGIN);
    maze[END_Y][END_X] = static_cast<int>(Maze_Elements::END);
    Buffer_Node.first = -1, Buffer_Node.second = -1;
    animator->update( 1 );
}

inline bool Maze_Making::if_in_wall( const int &y, const int &x, const int &delta_y, const int &delta_x ) {
    return ( y + delta_y < MAZE_HEIGHT - 1 ) && ( x + delta_x < MAZE_WIDTH - 1 ) && ( y + delta_y > 0 ) && ( x + delta_x > 0 );    // 下牆、右牆、上牆、左牆
}

void Maze_Making::empty_map() {
    for ( int y = 0; y < MAZE_HEIGHT; ++y ) {
        for ( int x = 0; x < MAZE_WIDTH; ++x )
            maze[y][x] = static_cast<int>( Maze_Elements::GROUND );
    }
    animator->update( 1 );
} // end empty_map()

inline void Maze_Making::initializing_maze(){
    //地圖初始化
    for ( int y = 0; y < MAZE_HEIGHT; ++y ) {
        for ( int x = 0; x < MAZE_WIDTH; ++x ) {
            if ( y == 0 or y == MAZE_HEIGHT - 1 )    //上牆或下牆
                maze[y][x] = static_cast<int>( Maze_Elements::WALL );
            else if ( x == 0 or x == MAZE_WIDTH - 1 )    //左牆或右牆
                maze[y][x] = static_cast<int>( Maze_Elements::WALL );
            else if ( x % 2 == 1 && y % 2 == 1 )    //xy都為奇數的點當作GROUND
                maze[y][x] = static_cast<int>( Maze_Elements::GROUND );
            else    //xy其他的點當牆做切割點的動作
                maze[y][x] = static_cast<int>( Maze_Elements::WALL );

            //animator->update( 2 );    //更新畫面
        }
    }
} //end initializing_maze()

inline void Maze_Making::set_begin_point(int& seed_y, int& seed_x, std::deque<std::pair<int ,int>>& re_load) {
    std::srand( std::chrono::system_clock::now().time_since_epoch().count() );    //設定隨機數種子
    while ( true ) {
        seed_y = ( rand() % ( MAZE_HEIGHT - 2 ) ) + 1;
        seed_x = ( rand() % ( MAZE_WIDTH - 2 ) ) + 1;
        if ( seed_x % 2 == 1 && seed_y % 2 == 1 ) {
            maze[seed_y][seed_x] = static_cast<int>( Maze_Elements::EXPLORED );    // 隨機挑一點當作生成的起點
            re_load.emplace_back( std::make_pair( seed_y, seed_x ) );    //加進之後要改回道路的清單裡
            break;
        }
    }
} //end set_begin_point


void Maze_Making::random_prim_make_maze(const int& types) {
    initializing_maze();

    int seed_y{}, seed_x{};
    std::deque<std::pair<int, int>> re_load;    //之後要改回道路的座標清單
    set_begin_point(seed_y, seed_x, re_load);
    std::deque<std::pair<int, int>> wall;    //待找的牆的列表
    std::vector<int> direction_order{0, 1, 2, 3};
    std::random_shuffle(direction_order.begin(), direction_order.end());
    for ( const int& index : direction_order ) {
        const auto &[dir_y, dir_x] = directions[index];
        if ( if_in_wall( seed_y, seed_x, dir_y, dir_x ) )
            wall.emplace_back( std::make_pair( seed_y + dir_y, seed_x + dir_x ) );    //將起點四周在迷宮內的牆加入wall列表中
    }

    int random_index;    //要打通的牆的index，隨機找
    int up_node, down_node, left_node, right_node;    //目前這個牆的上下左右結點
    std::deque<std::pair<int, int>> confirm_wall;    //確認是牆壁的列表
    while ( !wall.empty() ) {
        random_index = rand() % wall.size();
        int temp_y = wall.at( random_index ).first, temp_x = wall.at( random_index ).second;

        // 將四周點加進deque裡的函式
        auto Pushing_Wall = [&temp_y, &temp_x, &re_load, &wall, &direction_order, this]() {
            maze[temp_y][temp_x] = static_cast<int>( Maze_Elements::EXPLORED );    //將現在的節點(牆壁上下左右其中一個，看哪個方向符合條件) 改為 EXPLORED
            Buffer_Node.first = temp_y, Buffer_Node.second = temp_x;
            re_load.emplace_back( std::make_pair( temp_y, temp_x ) );    //將這個節點的座標記起來，等等要改回 GROUND
            std::random_shuffle(direction_order.begin(), direction_order.end());
            for ( const int &index : direction_order ) {    //(新的點的)上下左右遍歷
                const auto &[dir_y, dir_x] = directions[index];
                if ( if_in_wall( temp_y, temp_x, dir_y, dir_x ) ) {    //如果上(下左右)的牆在迷宮內
                    if ( maze[temp_y + dir_y][temp_x + dir_x] == static_cast<int>( Maze_Elements::WALL ) )    //而且如果這個節點是牆
                        wall.emplace_back( std::make_pair( temp_y + dir_y, temp_x + dir_x ) );    //就將這個節點加入wall列表中
                }
            }
        };

        if ( maze[temp_y][temp_x] == static_cast<int>( Maze_Elements::WALL ) ) {    //如果抽到的那格確定是牆再去判斷，有時候會有一個牆重複被加到清單裡的情形

            if ( if_in_wall( temp_y, temp_x, -1, 0 ) )
                up_node = maze[temp_y - 1][temp_x];
            if ( if_in_wall( temp_y, temp_x, 1, 0 ) )
                down_node = maze[temp_y + 1][temp_x];
            if ( if_in_wall( temp_y, temp_x, 0, -1 ) )
                left_node = maze[temp_y][temp_x - 1];
            if ( if_in_wall( temp_y, temp_x, 0, 1 ) )
                right_node = maze[temp_y][temp_x + 1];

            if ( ( up_node == static_cast<int>( Maze_Elements::EXPLORED ) && down_node == static_cast<int>( Maze_Elements::EXPLORED ) ) || ( left_node == static_cast<int>( Maze_Elements::EXPLORED ) && right_node == static_cast<int>( Maze_Elements::EXPLORED ) ) ) {    //如果左右都探索過了，或上下都探索過了，就把這個牆留著，並且加到確定是牆壁的 deque 裡
                confirm_wall.emplace_back( wall.at( random_index ) );    //把這個牆的位置加入deque
                wall.erase( wall.begin() + random_index );    //如果「上下都走過」或「左右都走過」，那麼就把這個牆留著
            }
            else {
                //不然就把牆打通
                maze[temp_y][temp_x] = static_cast<int>( Maze_Elements::EXPLORED );
                Buffer_Node.first = temp_y, Buffer_Node.second = temp_x;
                re_load.emplace_back( std::make_pair( temp_y, temp_x ) );
                wall.erase( wall.begin() + random_index );
                animator->update( 1 );

                if ( up_node == static_cast<int>( Maze_Elements::EXPLORED ) && down_node == static_cast<int>( Maze_Elements::GROUND ) ) {    //上面探索過，下面還沒
                    ++temp_y;    //將目前的節點改成牆壁 "下面" 那個節點
                    Pushing_Wall();    //判斷哪些點要加進哪種列表
                    animator->update( 1 );
                }
                else if ( up_node == static_cast<int>( Maze_Elements::GROUND ) && down_node == static_cast<int>( Maze_Elements::EXPLORED ) ) {    //下面探索過，上面還沒
                    --temp_y;    //將目前的節點改成牆壁 "上面" 那個節點
                    Pushing_Wall();    //判斷哪些點要加進哪種列表
                    animator->update( 1 );
                }
                else if ( left_node == static_cast<int>( Maze_Elements::EXPLORED ) && right_node == static_cast<int>( Maze_Elements::GROUND ) ) {    //左邊探索過，右邊還沒
                    ++temp_x;    //將目前的節點改成牆壁 "右邊" 那個節點
                    Pushing_Wall();    //判斷哪些點要加進哪種列表
                    animator->update( 1 );
                }
                else if ( left_node == static_cast<int>( Maze_Elements::GROUND ) && right_node == static_cast<int>( Maze_Elements::EXPLORED ) ) {    //右邊探索過，左邊還沒
                    --temp_x;    //將目前的節點改成牆壁 "左邊" 那個節點
                    Pushing_Wall();    //判斷哪些點要加進哪種列表
                    animator->update( 1 );
                }
                else {    //沒意外的話不會跑到這裡，加個 else 預防意外
                    std::cout << "test else " << temp_y << " " << temp_x << '\n';
                    break;
                }
            }

            up_node = -1;
            down_node = -1;
            left_node = -1;
            right_node = -1;
        }    // end if(maze[temp_y][temp_x] == static_cast<int>(Maze_Elements::WALL))
    }    //end while ( !wall.empty() )

    Buffer_Node.first = -1, Buffer_Node.second = -1;
    for ( const auto &[temp_y, temp_x] : re_load ) {    //把剛剛探索過的點換成 GROUND ，因為我們在生成地圖
        maze[temp_y][temp_x] = static_cast<int>( Maze_Elements::GROUND );
        //animator->update( 2 );
    }
    animator->update( 1 );

    if(types != 0) {
        int confirm_wall_num = confirm_wall.size() / 2;    //最後牆的數量
        for ( int i = rand() % ( confirm_wall_num / 4 ); i != 0; --i ) {    //把最多四分之一的牆打掉
            random_index = rand() % confirm_wall.size();    //這裡面可能會選到重複的牆(因為每個牆都加進去兩次)
            maze[confirm_wall.at( random_index ).first][confirm_wall.at( random_index ).second] = static_cast<int>( Maze_Elements::GROUND );    //把牆打掉
            confirm_wall.erase( confirm_wall.begin() + random_index );    //把這個牆刪掉(裡面可能還有一個唷記得)
            animator->update( 1 );    //更新畫面
        }
    }
}    // end random_prim_make_maze


void Maze_Making::recursion_make_maze(){
    initializing_maze();

    std::deque<std::pair<int, int>> re_load;    //之後要改回道路的座標清單
    int seed_y{}, seed_x{};    //一開始 x,y 座標
    set_begin_point(seed_y, seed_x, re_load);
    std::srand( std::chrono::system_clock::now().time_since_epoch().count() );    //設定隨機數種子

    struct Node {
        int y, x, current_index;
        std::vector<int> walking_order{0,1,2,3};
        Node(int __y, int __x, int __current_index) : y(__y), x(__x), current_index(__current_index){
            std::random_shuffle(walking_order.begin(), walking_order.end());
        };
    };
    std::stack<Node> Walking_List;
    Node first_node(seed_y, seed_x, 0);
    Walking_List.emplace(first_node);

    while(!Walking_List.empty()) {
        Node temp = Walking_List.top();
        int &Index = Walking_List.top().walking_order[Walking_List.top().current_index];
        const auto [dir_y, dir_x] = directions[Index];
        if( if_in_wall( temp.y, temp.x, 2*dir_y, 2*dir_x ) ) {
            if(maze[temp.y + 2*dir_y][temp.x + 2*dir_x] == static_cast<int>(Maze_Elements::GROUND)) {
                ++Walking_List.top().current_index;
                if(Walking_List.top().current_index > 3)
                    Walking_List.pop();
                Walking_List.emplace(Node(temp.y+2*dir_y, temp.x+2*dir_x, 0));

                maze[temp.y + dir_y][temp.x + dir_x] = static_cast<int>(Maze_Elements::EXPLORED);
                re_load.emplace_back(std::make_pair(temp.y + dir_y, temp.x + dir_x));
                Buffer_Node.first = temp.y + dir_y, Buffer_Node.second = temp.x + dir_x;

                maze[temp.y + 2*dir_y][temp.x + 2*dir_x] = static_cast<int>(Maze_Elements::EXPLORED);
                re_load.emplace_back(std::make_pair(temp.y + 2*dir_y, temp.x + 2*dir_x));
                Buffer_Node.first = temp.y + dir_y, Buffer_Node.second = temp.x + dir_x;
                animator->update( 1 );
            }
            else {
                ++Walking_List.top().current_index;
                if(Walking_List.top().current_index > 3)
                    Walking_List.pop();
            }
        }
        else {
            ++Walking_List.top().current_index;
            if(Walking_List.top().current_index > 3)
                Walking_List.pop();
        }
    }

    Buffer_Node.first = -1, Buffer_Node.second = -1;
    for ( const auto &[temp_y, temp_x] : re_load ) {    //把剛剛探索過的點換成 GROUND ，因為我們在生成地圖
        maze[temp_y][temp_x] = static_cast<int>( Maze_Elements::GROUND );
        //animator->update( 2 );
    }
    animator->update( 1 );
}    //end recursion_make_maze

void Maze_Making::recursive_division(const int uy, const int lx, const int dy, const int rx) {
    std::srand( std::chrono::system_clock::now().time_since_epoch().count() );    //設定隨機數種子
    int width = rx - lx + 1, height = dy - uy + 1;
    if( width < 2 && height < 2) return;
    if( !if_in_wall( uy, lx, height - 1, width - 1 ) )
        return;

    bool is_horizontal = ( width <= height ) ? true : false;
    int wall_index;
    if (is_horizontal && height - 2 > 0) {
        wall_index = uy + rand()%(height - 2) + 1;
        for(int i = lx; i <= rx; ++i) maze[wall_index][i] = static_cast<int>(Maze_Elements::WALL);    //將這段距離都設圍牆壁
        animator->update( 1 );
        recursive_division ( uy, lx, wall_index - 1, rx );    //上面
        recursive_division ( wall_index + 1, lx, dy, rx );    //下面
    }
    else if (!is_horizontal && width - 2 > 0) {
        wall_index = lx + rand()%(width - 2) + 1;
        for(int i = uy; i <= dy; ++i) maze[i][wall_index] = static_cast<int>(Maze_Elements::WALL);    //將這段距離都設圍牆壁
        animator->update( 1 );
        recursive_division ( uy, lx, dy, wall_index - 1 );    //左邊
        recursive_division ( uy, wall_index + 1, dy, rx );    //右邊
    }
    else
        return;

    int path_index;
    if (is_horizontal) {
        while(true) {
            path_index = lx + rand()%(width);
            if (maze[wall_index - 1][path_index] + maze[wall_index + 1][path_index] + maze[wall_index][path_index - 1] + maze[wall_index][path_index + 1] <= 2 * static_cast<int>(Maze_Elements::WALL)) break;
        }
        maze[wall_index][path_index] = static_cast<int>(Maze_Elements::GROUND);
    }
    else {
        while(true) {
            path_index = uy + rand()%(height);
            if (maze[path_index - 1][wall_index] + maze[path_index + 1][wall_index] + maze[path_index][wall_index - 1] + maze[path_index][wall_index + 1] <= 2 * static_cast<int>(Maze_Elements::WALL)) break;
        }
        maze[path_index][wall_index] = static_cast<int>(Maze_Elements::GROUND);
    }
} //end Maze_Making::recursive_division
