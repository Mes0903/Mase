#include "MazeController.h"
#include "MazeModel.h"
#include "MazeView.h"

#include <chrono>
#include <random>
#include <algorithm>
#include <iostream>
#include <deque>
#include <stack>
#include <queue>
#include <utility>
#include <memory>

MazeModel::MazeModel(int height, int width)
    : maze(height, std::vector<int>(width, 0)), bufferNode(-1, -1) {}

void MazeModel::setController(MazeController *controller_ptr)
{
  this->controller_ptr = std::unique_ptr<MazeController>(controller_ptr);
}

void MazeModel::resetMaze()
{
  for (auto &row : maze) {
    std::fill(row.begin(), row.end(), 0);
  }
}

void MazeModel::resetWallAroundMaze()
{
  for (int y = 0; y < MAZE_HEIGHT; ++y) {
    for (int x = 0; x < MAZE_WIDTH; ++x) {
      if (x == 0 || x == MAZE_WIDTH - 1 || y == 0 || y == MAZE_HEIGHT - 1) {
        maze[y][x] = 1;    // Wall
      }
      else {
        maze[y][x] = 0;    // Ground
      }
    }
  }
}

int MazeModel::getMazeCell(int y, int x) const
{
  return maze[y][x];
}

void MazeModel::setMazeCell(int y, int x, int value)
{
  maze[y][x] = value;
}

std::pair<int, int> MazeModel::getBufferNode() const
{
  return bufferNode;
}

void MazeModel::setBufferNode(int y, int x)
{
  bufferNode = { y, x };
}

/* --------------------maze generation methods -------------------- */

void MazeModel::generateMazePrim(const MazeAction actions)
{
  int seed_y{}, seed_x{};
  std::deque<std::pair<int, int>> re_load;    // 之後要改回道路的座標清單
  set_begin_point(seed_y, seed_x, re_load);
  std::deque<std::pair<int, int>> wall;    // 待找的牆的列表
  std::vector<int> direction_order{ 0, 1, 2, 3 };
  std::mt19937 gen(std::chrono::high_resolution_clock::now().time_since_epoch().count());    // 產生亂數
  std::shuffle(direction_order.begin(), direction_order.end(), gen);
  for (const int &index : direction_order) {
    const auto &[dir_y, dir_x] = directions[index];
    if (if_in_wall(seed_y, seed_x, dir_y, dir_x))
      wall.emplace_back(std::make_pair(seed_y + dir_y, seed_x + dir_x));    // 將起點四周在迷宮內的牆加入wall列表中
  }

  int random_index;    // 要打通的牆的index，隨機找
  int up_node{ -1 }, down_node{ -1 }, left_node{ -1 }, right_node{ -1 };    // 目前這個牆的上下左右結點
  std::deque<std::pair<int, int>> confirm_wall;    // 確認是牆壁的列表
  while (!wall.empty()) {
    std::uniform_int_distribution<> wall_dis(0, wall.size() - 1);
    random_index = wall_dis(gen);
    int temp_y = wall.at(random_index).first, temp_x = wall.at(random_index).second;

    // 將四周點加進deque裡的函式
    auto Pushing_Wall = [&temp_y, &temp_x, &re_load, &wall, &direction_order, &gen, this]() {
      maze[temp_y][temp_x] = static_cast<int>(Maze_Elements::EXPLORED);    // 將現在的節點(牆壁上下左右其中一個，看哪個方向符合條件) 改為 EXPLORED
      bufferNode.first = temp_y, bufferNode.second = temp_x;
      re_load.emplace_back(std::make_pair(temp_y, temp_x));    // 將這個節點的座標記起來，等等要改回 GROUND
      std::shuffle(direction_order.begin(), direction_order.end(), gen);
      for (const int &index : direction_order) {    //(新的點的)上下左右遍歷
        const auto &[dir_y, dir_x] = directions[index];
        if (if_in_wall(temp_y, temp_x, dir_y, dir_x)) {    // 如果上(下左右)的牆在迷宮內
          if (maze[temp_y + dir_y][temp_x + dir_x] == static_cast<int>(Maze_Elements::WALL))    // 而且如果這個節點是牆
            wall.emplace_back(std::make_pair(temp_y + dir_y, temp_x + dir_x));    // 就將這個節點加入wall列表中
        }
      }
    };

    if (maze[temp_y][temp_x] == static_cast<int>(Maze_Elements::WALL)) {    // 如果抽到的那格確定是牆再去判斷，有時候會有一個牆重複被加到清單裡的情形

      if (if_in_wall(temp_y, temp_x, -1, 0))
        up_node = maze[temp_y - 1][temp_x];
      if (if_in_wall(temp_y, temp_x, 1, 0))
        down_node = maze[temp_y + 1][temp_x];
      if (if_in_wall(temp_y, temp_x, 0, -1))
        left_node = maze[temp_y][temp_x - 1];
      if (if_in_wall(temp_y, temp_x, 0, 1))
        right_node = maze[temp_y][temp_x + 1];

      if ((up_node == static_cast<int>(Maze_Elements::EXPLORED) && down_node == static_cast<int>(Maze_Elements::EXPLORED)) || (left_node == static_cast<int>(Maze_Elements::EXPLORED) && right_node == static_cast<int>(Maze_Elements::EXPLORED))) {    // 如果左右都探索過了，或上下都探索過了，就把這個牆留著，並且加到確定是牆壁的 deque 裡
        confirm_wall.emplace_back(wall.at(random_index));    // 把這個牆的位置加入deque
        wall.erase(wall.begin() + random_index);    // 如果「上下都走過」或「左右都走過」，那麼就把這個牆留著
      }
      else {
        // 不然就把牆打通
        maze[temp_y][temp_x] = static_cast<int>(Maze_Elements::EXPLORED);
        bufferNode.first = temp_y, bufferNode.second = temp_x;
        re_load.emplace_back(std::make_pair(temp_y, temp_x));
        wall.erase(wall.begin() + random_index);
        // animator->update(1);

        if (up_node == static_cast<int>(Maze_Elements::EXPLORED) && down_node == static_cast<int>(Maze_Elements::GROUND)) {    // 上面探索過，下面還沒
          ++temp_y;    // 將目前的節點改成牆壁 "下面" 那個節點
          Pushing_Wall();    // 判斷哪些點要加進哪種列表
          // animator->update(1);
        }
        else if (up_node == static_cast<int>(Maze_Elements::GROUND) && down_node == static_cast<int>(Maze_Elements::EXPLORED)) {    // 下面探索過，上面還沒
          --temp_y;    // 將目前的節點改成牆壁 "上面" 那個節點
          Pushing_Wall();    // 判斷哪些點要加進哪種列表
          // animator->update(1);
        }
        else if (left_node == static_cast<int>(Maze_Elements::EXPLORED) && right_node == static_cast<int>(Maze_Elements::GROUND)) {    // 左邊探索過，右邊還沒
          ++temp_x;    // 將目前的節點改成牆壁 "右邊" 那個節點
          Pushing_Wall();    // 判斷哪些點要加進哪種列表
          // animator->update(1);
        }
        else if (left_node == static_cast<int>(Maze_Elements::GROUND) && right_node == static_cast<int>(Maze_Elements::EXPLORED)) {    // 右邊探索過，左邊還沒
          --temp_x;    // 將目前的節點改成牆壁 "左邊" 那個節點
          Pushing_Wall();    // 判斷哪些點要加進哪種列表
          // animator->update(1);
        }
        else {    // 沒意外的話不會跑到這裡，加個 else 預防意外
          std::cout << "test else " << temp_y << " " << temp_x << '\n';
          break;
        }
      }

      up_node = -1;
      down_node = -1;
      left_node = -1;
      right_node = -1;
    }    // end if(maze[temp_y][temp_x] == static_cast<int>(Maze_Elements::WALL))
  }    // end while ( !wall.empty() )

  bufferNode.first = -1, bufferNode.second = -1;
  for (const auto &[temp_y, temp_x] : re_load) {    // 把剛剛探索過的點換成 GROUND ，因為我們在生成地圖
    maze[temp_y][temp_x] = static_cast<int>(Maze_Elements::GROUND);
    // // animator->update( 2 );
  }
  // animator->update(1);

  if (actions == MazeAction::G_PRIMS_BREAK_WALL) {
    int confirm_wall_num = confirm_wall.size() / 2;    // 最後牆的數量
    std::uniform_int_distribution<> conf_dis(0, confirm_wall_num / 4 - 1);
    for (int i = conf_dis(gen); i != 0; --i) {    // 把最多四分之一的牆打掉
      std::uniform_int_distribution<> conf_wall_dis(0, confirm_wall.size() - 1);
      random_index = conf_wall_dis(gen);    // 這裡面可能會選到重複的牆(因為每個牆都加進去兩次)
      maze[confirm_wall.at(random_index).first][confirm_wall.at(random_index).second] = static_cast<int>(Maze_Elements::GROUND);    // 把牆打掉
      confirm_wall.erase(confirm_wall.begin() + random_index);    // 把這個牆刪掉(裡面可能還有一個唷記得)
      // animator->update(1);    // 更新畫面
    }
  }
}    // end generateMazePrim()

void MazeModel::generateMazeRecursionBacktracker()
{
  std::deque<std::pair<int, int>> re_load;    // 之後要改回道路的座標清單
  int seed_y{}, seed_x{};    // 一開始 x,y 座標
  set_begin_point(seed_y, seed_x, re_load);
  std::mt19937 gen(std::chrono::high_resolution_clock::now().time_since_epoch().count());

  struct Node {
    int y, x, current_index;
    std::vector<int> walking_order{ 0, 1, 2, 3 };
    Node(int __y, int __x, int __current_index, std::mt19937 &gen) : y(__y), x(__x), current_index(__current_index)
    {
      std::shuffle(walking_order.begin(), walking_order.end(), gen);
    };
  };
  std::stack<Node> Walking_List;
  Node first_node(seed_y, seed_x, 0, gen);
  Walking_List.emplace(first_node);

  while (!Walking_List.empty()) {
    Node temp = Walking_List.top();
    int &Index = Walking_List.top().walking_order[Walking_List.top().current_index];
    const auto [dir_y, dir_x] = directions[Index];
    if (if_in_wall(temp.y, temp.x, 2 * dir_y, 2 * dir_x)) {
      if (maze[temp.y + 2 * dir_y][temp.x + 2 * dir_x] == static_cast<int>(Maze_Elements::GROUND)) {
        ++Walking_List.top().current_index;
        if (Walking_List.top().current_index > 3)
          Walking_List.pop();
        Walking_List.emplace(Node(temp.y + 2 * dir_y, temp.x + 2 * dir_x, 0, gen));

        maze[temp.y + dir_y][temp.x + dir_x] = static_cast<int>(Maze_Elements::EXPLORED);
        re_load.emplace_back(std::make_pair(temp.y + dir_y, temp.x + dir_x));
        bufferNode.first = temp.y + dir_y, bufferNode.second = temp.x + dir_x;

        maze[temp.y + 2 * dir_y][temp.x + 2 * dir_x] = static_cast<int>(Maze_Elements::EXPLORED);
        re_load.emplace_back(std::make_pair(temp.y + 2 * dir_y, temp.x + 2 * dir_x));
        bufferNode.first = temp.y + dir_y, bufferNode.second = temp.x + dir_x;
        // animator->update(1);
      }
      else {
        ++Walking_List.top().current_index;
        if (Walking_List.top().current_index > 3)
          Walking_List.pop();
      }
    }
    else {
      ++Walking_List.top().current_index;
      if (Walking_List.top().current_index > 3)
        Walking_List.pop();
    }
  }

  bufferNode.first = -1, bufferNode.second = -1;
  for (const auto &[temp_y, temp_x] : re_load) {    // 把剛剛探索過的點換成 GROUND ，因為我們在生成地圖
    maze[temp_y][temp_x] = static_cast<int>(Maze_Elements::GROUND);
    // // animator->update( 2 );
  }
  // animator->update(1);
}    // end generateMazeRecursionBacktracker()

void MazeModel::generateMazeRecursionDivision(const int uy, const int lx, const int dy, const int rx)
{
  std::mt19937 gen(std::chrono::high_resolution_clock::now().time_since_epoch().count());    // 產生亂數
  int width = rx - lx + 1, height = dy - uy + 1;
  if (width < 2 && height < 2) return;
  if (!if_in_wall(uy, lx, height - 1, width - 1))
    return;

  bool is_horizontal = (width <= height) ? true : false;
  int wall_index;
  if (is_horizontal && height - 2 > 0) {
    std::uniform_int_distribution<> h_dis(uy + 1, uy + height - 2);
    wall_index = h_dis(gen);
    for (int i = lx; i <= rx; ++i) maze[wall_index][i] = static_cast<int>(Maze_Elements::WALL);    // 將這段距離都設圍牆壁
    // animator->update(1);
    generateMazeRecursionDivision(uy, lx, wall_index - 1, rx);    // 上面
    generateMazeRecursionDivision(wall_index + 1, lx, dy, rx);    // 下面
  }
  else if (!is_horizontal && width - 2 > 0) {
    std::uniform_int_distribution<> w_dis(lx + 1, lx + width - 2);
    wall_index = w_dis(gen);
    for (int i = uy; i <= dy; ++i) maze[i][wall_index] = static_cast<int>(Maze_Elements::WALL);    // 將這段距離都設圍牆壁
    // animator->update(1);
    generateMazeRecursionDivision(uy, lx, dy, wall_index - 1);    // 左邊
    generateMazeRecursionDivision(uy, wall_index + 1, dy, rx);    // 右邊
  }
  else
    return;

  int path_index;
  if (is_horizontal) {
    while (true) {
      std::uniform_int_distribution<> w_dis(lx, lx + width - 1);
      path_index = w_dis(gen);
      if (maze[wall_index - 1][path_index] + maze[wall_index + 1][path_index] + maze[wall_index][path_index - 1] + maze[wall_index][path_index + 1] <= 2 * static_cast<int>(Maze_Elements::WALL)) break;
    }
    maze[wall_index][path_index] = static_cast<int>(Maze_Elements::GROUND);
  }
  else {
    while (true) {
      std::uniform_int_distribution<> h_dis(uy, uy + height - 1);
      path_index = h_dis(gen);
      if (maze[path_index - 1][wall_index] + maze[path_index + 1][wall_index] + maze[path_index][wall_index - 1] + maze[path_index][wall_index + 1] <= 2 * static_cast<int>(Maze_Elements::WALL)) break;
    }
    maze[path_index][wall_index] = static_cast<int>(Maze_Elements::GROUND);
  }
}    // end generateMazeRecursionDivision()

/* --------------------maze solving methods -------------------- */

bool MazeModel::solveMazeDFS(const int y, const int x)
{
  maze[1][0] = static_cast<int>(Maze_Elements::BEGIN);    // 起點
  bufferNode.first = 1, bufferNode.second = 0;
  // // animator->update( 1 );
  maze[y][x] = static_cast<int>(Maze_Elements::EXPLORED);    // 探索過的點
  bufferNode.first = y, bufferNode.second = x;
  // animator->update(1);
  if (y == END_Y && x == END_X) {    // 如果到終點了就回傳True
    maze[y][x] = static_cast<int>(Maze_Elements::END);    // 終點
    bufferNode.first = -1, bufferNode.second = -1;
    // animator->update(1);
    return true;
  }
  for (const auto &[dir_y, dir_x] : directions) {    // 上下左右
    const int temp_y = y + dir_y, temp_x = x + dir_x;
    if (is_in_maze(temp_y, temp_x)) {    // 如果這個節點在迷宮內
      if (maze[temp_y][temp_x] == static_cast<int>(Maze_Elements::GROUND))    // 而且如果這個節點還沒被探索過
        if (solveMazeDFS(temp_y, temp_x))    // 就繼續遞迴，如果已經找到目標就會回傳 true ，所以這裡放在 if 裡面
          return true;
    }
  }
  return false;
}    // end solveMazeDFS()

void MazeModel::solveMazeBFS()
{
  std::queue<std::pair<int, int>> result;    // 存節點的deque
  result.push(std::make_pair(BEGIN_Y, BEGIN_X));    // 將一開始的節點加入deque
  maze[BEGIN_Y][BEGIN_X] = static_cast<int>(Maze_Elements::BEGIN);    // 起點
  // animator->update(1);

  while (!result.empty()) {
    const auto [temp_y, temp_x]{ result.front() };    // 目前的節點
    result.pop();    // 將目前的節點拿出來

    for (const auto &dir : directions) {    // 遍歷上下左右
      const int y = temp_y + dir.first, x = temp_x + dir.second;    // 上下左右的節點

      if (is_in_maze(y, x)) {    // 如果這個節點在迷宮內
        if (maze[y][x] == static_cast<int>(Maze_Elements::GROUND)) {    // 而且如果這個節點還沒被探索過，也不是牆壁
          maze[y][x] = static_cast<int>(Maze_Elements::EXPLORED);    // 那就探索他，改 EXPLORED
          bufferNode.first = y, bufferNode.second = x;
          // animator->update(1);
          if (y == END_Y && x == END_X) {    // 找到終點就return
            maze[y][x] = static_cast<int>(Maze_Elements::END);    // 終點
            bufferNode.first = -1, bufferNode.second = -1;
            // animator->update(1);
            return;
          }
          else
            result.push(std::make_pair(y, x));    // 沒找到節點就加入節點
        }
      }
    }
  }    // end while
}    // end solveMazeBFS()

void MazeModel::solveMazeUCS(const MazeAction actions)
{
  struct Node {
    int __Weight;    // 權重 (Cost Function)
    int y;    // y座標
    int x;    // x座標
    Node(int weight, int y, int x) : __Weight(weight), y(y), x(x) {}
    bool operator>(const Node &other) const { return __Weight > other.__Weight; }    // priority比大小只看權重
    bool operator<(const Node &other) const { return __Weight < other.__Weight; }    // priority比大小只看權重
  };

  std::priority_queue<Node, std::deque<Node>, std::greater<Node>> result;    // 待走的結點，greater代表小的會在前面，由小排到大
  int weight{};    // 用來計算的權重

  switch (actions) {    // 起點
  case MazeAction::S_UCS_MANHATTAN:
    weight = abs(END_X - BEGIN_X) + abs(END_Y - BEGIN_Y);    // 權重為曼哈頓距離
    break;
  case MazeAction::S_UCS_TWO_NORM:
    weight = pow_two_norm(BEGIN_Y, BEGIN_X);    // 權重為 two_norm
    break;
  case MazeAction::S_UCS_INTERVAL:
    constexpr int interval_y = MAZE_HEIGHT / 10, interval_x = MAZE_WIDTH / 10;    // 分 10 個區間
    weight = (static_cast<int>(BEGIN_Y / interval_y) < static_cast<int>(BEGIN_X / interval_x)) ? (10 - static_cast<int>(BEGIN_Y / interval_y)) : (10 - static_cast<int>(BEGIN_X / interval_x));    // 權重以區間計算，兩個相除是看它在第幾個區間，然後用總區間數減掉，代表它的基礎權重，再乘以1000
    break;
  }

  result.emplace(Node(weight, BEGIN_Y, BEGIN_Y));    // 將起點加進去

  while (true) {
    if (result.empty())
      return;    // 沒找到目標

    const auto temp = result.top();    // 目前最優先的結點
    result.pop();    // 取出結點判斷

    if (temp.y == END_Y && temp.x == END_X) {
      maze[temp.y][temp.x] = static_cast<int>(Maze_Elements::END);    // 終點
      bufferNode.first = -1, bufferNode.second = -1;
      // animator->update(1);
      return;    // 如果取出的點是終點就return
    }
    else if (maze[temp.y][temp.x] == static_cast<int>(Maze_Elements::GROUND)) {
      if (temp.y == BEGIN_Y && temp.x == BEGIN_X)
        maze[temp.y][temp.x] = static_cast<int>(Maze_Elements::BEGIN);    // 起點
      else {
        maze[temp.y][temp.x] = static_cast<int>(Maze_Elements::EXPLORED);    // 探索過的點要改EXPLORED
        bufferNode.first = temp.y, bufferNode.second = temp.x;
      }
      // animator->update(1);
      for (const auto &dir : directions) {
        const int y = temp.y + dir.first, x = temp.x + dir.second;

        if (is_in_maze(y, x)) {
          if (maze[y][x] == static_cast<int>(Maze_Elements::GROUND)) {    // 如果這個結點還沒走過，就把他加到待走的結點裡
            switch (actions) {
            case MazeAction::S_UCS_MANHATTAN:
              weight = abs(END_X - x) + abs(END_Y - y);    // 權重為曼哈頓距離
              break;
            case MazeAction::S_UCS_TWO_NORM:
              weight = pow_two_norm(y, x);    // 權重為 Two_Norm
              break;
            case MazeAction::S_UCS_INTERVAL:
              constexpr int interval_y = MAZE_HEIGHT / 10, interval_x = MAZE_WIDTH / 10;    // 分 10 個區間
              weight = (static_cast<int>(y / interval_y) < static_cast<int>(x / interval_x)) ? (10 - static_cast<int>(y / interval_y)) : (10 - static_cast<int>(x / interval_x));    // 權重為區間
              break;
            }
            result.emplace(Node(temp.__Weight + weight, y, x));    // 加入節點
          }
        }
      }    // end for
    }
  }    // end while
}    // end solveMazeUCS()

void MazeModel::solveMazeGreedy()
{
  struct Node {
    int __Weight;    // 權重為 Two_Norm 平方 (Heuristic function)
    int y;    // y座標
    int x;    // x座標
    Node(int weight, int y, int x) : __Weight(weight), y(y), x(x) {}
    bool operator>(const Node &other) const { return __Weight > other.__Weight; }    // priority比大小只看權重
    bool operator<(const Node &other) const { return __Weight < other.__Weight; }    // priority比大小只看權重
  };

  std::priority_queue<Node, std::deque<Node>, std::greater<Node>> result;    // 待走的結點，greater代表小的會在前面，由小排到大
  result.emplace(Node(pow_two_norm(BEGIN_Y, BEGIN_X), BEGIN_Y, BEGIN_X));    // 將起點加進去

  while (true) {
    if (result.empty())
      return;    // 沒找到目標
    const auto temp = result.top();    // 目前最優先的結點
    result.pop();    // 取出結點判斷

    if (temp.y == END_Y && temp.x == END_X) {
      maze[temp.y][temp.x] = static_cast<int>(Maze_Elements::END);    // 終點
      bufferNode.first = -1, bufferNode.second = -1;
      // animator->update(1);
      return;    // 如果取出的點是終點就return
    }
    else if (maze[temp.y][temp.x] == static_cast<int>(Maze_Elements::GROUND)) {
      if (temp.y == BEGIN_Y && temp.x == BEGIN_X)
        maze[temp.y][temp.x] = static_cast<int>(Maze_Elements::BEGIN);    // 起點
      else {
        maze[temp.y][temp.x] = static_cast<int>(Maze_Elements::EXPLORED);    // 探索過的點要改EXPLORED
        bufferNode.first = temp.y, bufferNode.second = temp.x;
      }
      // animator->update(1);
      for (const auto &dir : directions) {
        const int y = temp.y + dir.first, x = temp.x + dir.second;

        if (is_in_maze(y, x)) {
          if (maze[y][x] == static_cast<int>(Maze_Elements::GROUND))    // 如果這個結點還沒走過，就把他加到待走的結點裡
            result.emplace(Node(pow_two_norm(y, x), y, x));
        }
      }
    }
  }    // end while
}    // end solveMazeGreedy()

void MazeModel::solveMazeAStar(const MazeAction actions)
{
  enum class Types : int {
    Normal = 0,    // Cost Function 為 50
    Interval = 1,    // Cost Function 以區間來計算，每一個區間 Cost 差10，距離終點越遠 Cost 越大
  };

  struct Node {
    int __Cost;    // Cost Function 有兩種，以區間計算，每個區間 Cost 差10
    int __Weight;    // 權重以區間(Cost Function) + Two_Norm 平方(Heuristic Function) 計算，每個區間 Cost 差1000
    int y;    // y座標
    int x;    // x座標
    Node(int cost, int weight, int y, int x) : __Cost(cost), __Weight(weight), y(y), x(x) {}
    bool operator>(const Node &other) const { return __Weight > other.__Weight; }    // priority比大小只看權重
    bool operator<(const Node &other) const { return __Weight < other.__Weight; }    // priority比大小只看權重
  };

  std::priority_queue<Node, std::deque<Node>, std::greater<Node>> result;    // 待走的結點，greater代表小的會在前面，由小排到大
  constexpr int interval_y = MAZE_HEIGHT / 10, interval_x = MAZE_WIDTH / 10;    // 分 10 個區間
  int cost{}, weight{};

  if (actions == MazeAction::S_ASTAR_INTERVAL) {
    cost = 50;
    weight = cost + abs(END_X - BEGIN_X) + abs(END_Y - BEGIN_Y);
  }
  else if (actions == MazeAction::S_ASTAR_INTERVAL) {
    cost = (static_cast<int>(BEGIN_Y / interval_y) < static_cast<int>(BEGIN_X / interval_x)) ? (10 - static_cast<int>(BEGIN_Y / interval_y)) * 8 : (10 - static_cast<int>(BEGIN_X / interval_x)) * 8;    // Cost 以區間計算，兩個相除是看它在第幾個區間，然後用總區間數減掉，代表它的基礎 Cost，再乘以8
    weight = cost + pow_two_norm(BEGIN_Y, BEGIN_X);    // 權重以區間(Cost) + Two_Norm 計算
  }
  result.emplace(Node(cost, weight, BEGIN_Y, BEGIN_X));    // 將起點加進去

  while (true) {
    if (result.empty())
      return;    // 沒找到目標
    const auto temp = result.top();    // 目前最優先的結點
    result.pop();    // 取出結點

    if (temp.y == END_Y && temp.x == END_X) {
      maze[temp.y][temp.x] = static_cast<int>(Maze_Elements::END);    // 終點
      bufferNode.first = -1, bufferNode.second = -1;
      // animator->update(1);
      return;    // 如果取出的點是終點就return
    }
    else if (maze[temp.y][temp.x] == static_cast<int>(Maze_Elements::GROUND)) {
      if (temp.y == BEGIN_Y && temp.x == BEGIN_X)
        maze[temp.y][temp.x] = static_cast<int>(Maze_Elements::BEGIN);    // 起點
      else {
        maze[temp.y][temp.x] = static_cast<int>(Maze_Elements::EXPLORED);    // 探索過的點要改EXPLORED
        bufferNode.first = temp.y, bufferNode.second = temp.x;
      }
      // animator->update(1);
      for (const auto &dir : directions) {
        const int y = temp.y + dir.first, x = temp.x + dir.second;

        if (is_in_maze(y, x)) {
          if (maze[y][x] == static_cast<int>(Maze_Elements::GROUND)) {    // 如果這個結點還沒走過，就把他加到待走的結點裡
            if (actions == MazeAction::S_ASTAR_INTERVAL) {
              cost = 50;    // cost function設為常數 50
              weight = cost + abs(END_X - x) + abs(END_Y - y);    // heuristic function 設為曼哈頓距離
            }
            else if (actions == MazeAction::S_ASTAR_INTERVAL) {
              cost = (static_cast<int>(y / interval_y) < static_cast<int>(x / interval_x)) ? temp.__Cost + (10 - static_cast<int>(y / interval_y)) * 8 : temp.__Cost + (10 - static_cast<int>(x / interval_x)) * 8;    // Cost 以區間計算，兩個相除是看它在第幾個區間，然後用總區間數減掉，代表它的基礎 Cost，再乘以8
              weight = cost + pow_two_norm(y, x);    // heuristic function 設為 two_norm 平方
            }
            result.emplace(Node(cost, weight, y, x));
          }
        }
      }
    }
  }    // end while
}    // end solveMazeAStar()

/* -------------------- private utility function --------------------   */

void MazeModel::set_Flag()
{
  maze[BEGIN_Y][BEGIN_X] = static_cast<int>(Maze_Elements::BEGIN);
  bufferNode.first = -1, bufferNode.second = -1;
  // animator->update(1);
}

bool MazeModel::if_in_wall(const int y, const int x, const int delta_y, const int delta_x)
{
  return (y + delta_y < MAZE_HEIGHT - 1) && (x + delta_x < MAZE_WIDTH - 1) && (y + delta_y > 0) && (x + delta_x > 0);    // 下牆、右牆、上牆、左牆
}

void MazeModel::empty_map()
{
  for (int y = 0; y < MAZE_HEIGHT; ++y) {
    for (int x = 0; x < MAZE_WIDTH; ++x)
      maze[y][x] = static_cast<int>(Maze_Elements::GROUND);
  }
  // animator->update(1);
}

void MazeModel::initializing_maze()
{
  // 地圖初始化
  for (int y = 0; y < MAZE_HEIGHT; ++y) {
    for (int x = 0; x < MAZE_WIDTH; ++x) {
      if (y == 0 || y == MAZE_HEIGHT - 1)    // 上牆或下牆
        maze[y][x] = static_cast<int>(Maze_Elements::WALL);
      else if (x == 0 || x == MAZE_WIDTH - 1)    // 左牆或右牆
        maze[y][x] = static_cast<int>(Maze_Elements::WALL);
    }
  }
}

void MazeModel::set_begin_point(int &seed_y, int &seed_x, std::deque<std::pair<int, int>> &re_load)
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 3);

  seed_y = BEGIN_Y, seed_x = BEGIN_X;
  re_load.push_back(std::make_pair(seed_y, seed_x));
}

void MazeModel::set_end_point(int &seed_y, int &seed_x, std::deque<std::pair<int, int>> &re_load)
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 3);

  seed_y = END_Y, seed_x = END_X;
  re_load.push_back(std::make_pair(seed_y, seed_x));
}

bool MazeModel::is_in_maze(const int y, const int x)
{
  return (y < MAZE_HEIGHT) && (x < MAZE_WIDTH) && (y >= 0) && (x >= 0);
}

int MazeModel::pow_two_norm(const int y, const int x)
{
  return pow((END_Y - y), 2) + pow((END_X - x), 2);
}
