#include "MazeController.h"
#include "MazeModel.h"
#include "MazeView.h"

#include <chrono>
#include <random>
#include <algorithm>
#include <stack>
#include <queue>
#include <array>
#include <utility>

MazeModel::MazeModel(uint32_t height, uint32_t width)
    : maze{ height, std::vector<MazeElement>{ width, MazeElement::GROUND } }, solve_cost__(0), solve_cell__(0)
{
  maze[BEGIN_Y][BEGIN_X] = MazeElement::BEGIN;
  maze[END_Y][END_X] = MazeElement::END;
}

void MazeModel::setController(MazeController *controller_ptr)
{
  this->controller_ptr__ = controller_ptr;
}

int32_t MazeModel::getSolveCost() const
{
  return solve_cost__;
}

int32_t MazeModel::getSolveCell() const
{
  return solve_cell__;
}

void MazeModel::emptyMap()
{
  for (auto &row : maze)
    std::fill(row.begin(), row.end(), MazeElement::GROUND);

  controller_ptr__->enFramequeue(maze);
  setFlag__();
}

/* --------------------maze generation methods -------------------- */

/**
 * @brief Generate the maze by Randomized Prim's algorithm.
 * @param actions Two types of actions are supported:
 *        - G_PRIM: generate the maze by Randomized Prim's algorithm without breaking any wall.
 *        - G_PRIM_BREAK: after generating the maze by Randomized Prim's algorithm, it would break some walls randomly.
 */
void MazeModel::generateMazePrim(const MazeAction actions)
{
  initMaze__();

  std::mt19937 gen(std::chrono::high_resolution_clock::now().time_since_epoch().count());
  std::array<int32_t, 4> direction_order{ 0, 1, 2, 3 };
  std::vector<MazeNode> break_list;    // list for breaking wall
  std::vector<MazeNode> candidate_list;    // list for wall candidate

  {
    MazeNode seed_node;
    setBeginPoint__(seed_node);    // the begin point for the maze generation

    std::shuffle(direction_order.begin(), direction_order.end(), gen);
    for (const int32_t index : direction_order) {
      const auto [dir_y, dir_x] = dir_vec[index];
      if (inWall__(seed_node.y + dir_y, seed_node.x + dir_x))
        candidate_list.emplace_back(MazeNode{ seed_node.y + dir_y, seed_node.x + dir_x, maze[seed_node.y + dir_y][seed_node.x + dir_x] });    // add the wall around the begin point to the candidate list
    }
  }

  while (!candidate_list.empty()) {
    std::uniform_int_distribution<> wall_dis(0, candidate_list.size() - 1);
    int32_t random_index = wall_dis(gen);
    MazeNode current_node = candidate_list[random_index];    // pick one wall out
    MazeElement up_element{ MazeElement::INVALID }, down_element{ MazeElement::INVALID }, left_element{ MazeElement::INVALID }, right_element{ MazeElement::INVALID };    // the element of the wall's up, down, left, right
    // if the wall is confirmed to be a wall, then check its up, down, left, right
    if (current_node.element == MazeElement::WALL) {
      if (inWall__(current_node.y - 1, current_node.x))
        up_element = maze[current_node.y - 1][current_node.x];
      if (inWall__(current_node.y + 1, current_node.x))
        down_element = maze[current_node.y + 1][current_node.x];
      if (inWall__(current_node.y, current_node.x - 1))
        left_element = maze[current_node.y][current_node.x - 1];
      if (inWall__(current_node.y, current_node.x + 1))
        right_element = maze[current_node.y][current_node.x + 1];

      // if the wall's up and down are explored, or the wall's left and right are explored, then add this wall to the break list
      if ((up_element == MazeElement::EXPLORED && down_element == MazeElement::EXPLORED) || (left_element == MazeElement::EXPLORED && right_element == MazeElement::EXPLORED)) {
        break_list.emplace_back(current_node);
        candidate_list.erase(candidate_list.begin() + random_index);
      }
      else {
        // else, set the wall to be explored
        current_node.element = MazeElement::EXPLORED;
        maze[current_node.y][current_node.x] = MazeElement::EXPLORED;
        candidate_list.erase(candidate_list.begin() + random_index);

        controller_ptr__->enFramequeue(maze, current_node);    // append the maze to the framequeue

        // changed the current node to the next node
        if (up_element == MazeElement::EXPLORED && down_element == MazeElement::GROUND)
          ++current_node.y;
        else if (up_element == MazeElement::GROUND && down_element == MazeElement::EXPLORED)
          --current_node.y;
        else if (left_element == MazeElement::EXPLORED && right_element == MazeElement::GROUND)
          ++current_node.x;
        else if (left_element == MazeElement::GROUND && right_element == MazeElement::EXPLORED)
          --current_node.x;

        current_node.element = MazeElement::EXPLORED;
        maze[current_node.y][current_node.x] = MazeElement::EXPLORED;
        std::shuffle(direction_order.begin(), direction_order.end(), gen);

        // iterate the new node's up, down, left, right
        for (const int32_t index : direction_order) {
          const auto [dir_y, dir_x] = dir_vec[index];

          // if the node is in the maze and the node is a wall
          if (inWall__(current_node.y + dir_y, current_node.x + dir_x)) {
            if (maze[current_node.y + dir_y][current_node.x + dir_x] == MazeElement::WALL)
              candidate_list.emplace_back(MazeNode{ current_node.y + dir_y, current_node.x + dir_x, maze[current_node.y + dir_y][current_node.x + dir_x] });
          }
        }

        controller_ptr__->enFramequeue(maze, current_node);    // append the maze to the framequeue
      }
    }
  }

  // if the action is G_PRIM_BREAK, then break some walls
  if (actions == MazeAction::G_PRIM_BREAK) {
    std::shuffle(break_list.begin(), break_list.end(), gen);
    std::uniform_int_distribution<> dis(0, break_list.size() / 5 - 1);    // at most break 1/5 walls
    for (int i = dis(gen); i > 0; --i) {
      std::uniform_int_distribution<> wall_dis(0, break_list.size() - 1);
      int32_t random_index = wall_dis(gen);
      maze[break_list[random_index].y][break_list[random_index].x] = MazeElement::GROUND;
      break_list.erase(break_list.begin() + random_index);
    }
  }

  setFlag__();
  cleanExplored__();
  controller_ptr__->setModelComplete();
}    // end generateMazePrim()

/**
 * @brief Generate the maze by Recursive Backtracker algorithm, also known as Randomized depth-first search algorithm.
 */
void MazeModel::generateMazeRecursionBacktracker()
{
  initMaze__();

  // the node for the recursion backtracker algorithm
  struct TraceNode {
    MazeNode node;
    int8_t index = 0;
    std::array<uint8_t, 4> direction_order = { 0, 1, 2, 3 };
  };

  std::mt19937 gen(std::chrono::high_resolution_clock::now().time_since_epoch().count());
  std::stack<TraceNode> candidate_list;

  {
    TraceNode seed_node;
    std::shuffle(seed_node.direction_order.begin(), seed_node.direction_order.end(), gen);
    setBeginPoint__(seed_node.node);    // set the begin point for the maze generation
    candidate_list.push(seed_node);
  }

  while (!candidate_list.empty()) {
    TraceNode &current_node = candidate_list.top();
    if (current_node.index == 4) {
      candidate_list.pop();
      continue;    // if the index is 4, then pop the node and continue
    }

    int8_t dir = current_node.direction_order[current_node.index++];    // get the direction of the node
    const auto [dir_y, dir_x] = dir_vec[dir];

    if (!inWall__(current_node.node.y + 2 * dir_y, current_node.node.x + 2 * dir_x))
      continue;    // if the node is not in the maze, then continue

    if (maze[current_node.node.y + 2 * dir_y][current_node.node.x + 2 * dir_x] == MazeElement::GROUND) {
      TraceNode target_node{ { current_node.node.y + 2 * dir_y, current_node.node.x + 2 * dir_x, MazeElement::GROUND }, 0, { 0, 1, 2, 3 } };    // walk through the node
      std::shuffle(target_node.direction_order.begin(), target_node.direction_order.end(), gen);

      // since one step would go through two cells, so two assignment here
      current_node.node.element = MazeElement::EXPLORED;
      maze[current_node.node.y + dir_y][current_node.node.x + dir_x] = MazeElement::EXPLORED;
      controller_ptr__->enFramequeue(maze, current_node.node);    // append the maze to the framequeue

      target_node.node.element = MazeElement::EXPLORED;
      maze[target_node.node.y][target_node.node.x] = MazeElement::EXPLORED;
      controller_ptr__->enFramequeue(maze, target_node.node);    // append the maze to the framequeue

      candidate_list.push(target_node);    // push the node to the candidate list
    }
  }

  setFlag__();
  cleanExplored__();
  controller_ptr__->setModelComplete();
}    // end generateMazeRecursionBacktracker()


/**
 * @brief Generate the maze by Recursive Division algorithm.
 * @param uy The upper y-coordinate of target area.
 * @param lx The left x-coordinate of target area.
 * @param dy The lower y-coordinate of target area.
 * @param rx The right x-coordinate of target area.
 * @param is_first_call Whether it is the first call of the function, used to initialize the wall and set the ModelComplete flag.
 */
void MazeModel::generateMazeRecursionDivision(const int32_t uy, const int32_t lx, const int32_t dy, const int32_t rx, bool is_first_call)
{
  if (is_first_call)
    resetWallAroundMaze__();

  std::mt19937 gen(std::chrono::high_resolution_clock::now().time_since_epoch().count());
  int32_t width = rx - lx + 1, height = dy - uy + 1;
  if (width < 3 || height < 3) return;    // if the width or height is less than 3, then return

  const bool cut_horizontal = (height > width);    // if the height is greater than the width, then cut horizontally
  int32_t wall_index;
  if (cut_horizontal) {
    std::uniform_int_distribution<> h_dis(1, (height - 1) / 2);
    wall_index = uy + 2 * h_dis(gen) - 1;    // get the index of the wall, here we insure the index is even.

    for (int32_t i = lx; i <= rx; ++i) {
      maze[wall_index][i] = MazeElement::WALL;
      controller_ptr__->enFramequeue(maze, MazeNode{ wall_index, i, MazeElement::WALL });
    }

    // break one wall to create a passage
    std::uniform_int_distribution<> w_dis(1, (width - 1) / 2);
    int32_t break_point = lx + 2 * w_dis(gen);
    maze[wall_index][break_point] = MazeElement::GROUND;
    controller_ptr__->enFramequeue(maze, MazeNode{ wall_index, break_point, MazeElement::GROUND });

    generateMazeRecursionDivision(uy, lx, wall_index - 1, rx);    // top side
    generateMazeRecursionDivision(wall_index + 1, lx, dy, rx);    // bottom side
  }
  else {
    std::uniform_int_distribution<> w_dis(1, (width - 1) / 2);
    wall_index = lx + 2 * w_dis(gen) - 1;    // get the index of the wall, here we insure the index is even.

    for (int32_t i = uy; i <= dy; ++i) {
      maze[i][wall_index] = MazeElement::WALL;
      controller_ptr__->enFramequeue(maze, MazeNode{ i, wall_index, MazeElement::WALL });
    }

    // break one wall to create a passage
    std::uniform_int_distribution<> h_dis(1, (height - 1) / 2);
    int32_t break_point = uy + 2 * h_dis(gen);
    maze[break_point][wall_index] = MazeElement::GROUND;
    controller_ptr__->enFramequeue(maze, MazeNode{ break_point, wall_index, MazeElement::GROUND });

    generateMazeRecursionDivision(uy, lx, dy, wall_index - 1);    // left side
    generateMazeRecursionDivision(uy, wall_index + 1, dy, rx);    // right side
  }

  if (is_first_call) {
    setFlag__();
    cleanExplored__();
    controller_ptr__->setModelComplete();
  }
}    // end generateMazeRecursionDivision()

/* --------------------maze solving methods -------------------- */

/**
 * @brief Solve the maze by DFS algorithm.
 * @param y The y-coordinate of the current node.
 * @param x The x-coordinate of the current node.
 * @param is_first_call Whether it is the first call of the function, used to initialize the explored and set the ModelComplete flag.
 * @return Whether the maze is solved.
 */
bool MazeModel::solveMazeDFS(const int32_t y, const int32_t x, bool is_first_call)
{
  if (maze[y][x] == MazeElement::END)
    return true;

  if (is_first_call) {
    cleanExplored__();
    solve_cost__ = 0;
    solve_cell__ = 0;
  }
  else {
    maze[y][x] = MazeElement::EXPLORED;
    controller_ptr__->enFramequeue(maze, MazeNode{ y, x, MazeElement::EXPLORED });
  }

  // iterate the current node's up, down, left, right
  for (const auto &[dir_y, dir_x] : dir_vec) {
    const int32_t target_y = y + dir_y;
    const int32_t target_x = x + dir_x;

    if (!inMaze__(target_y, target_x))
      continue;

    if (maze[target_y][target_x] != MazeElement::GROUND && maze[target_y][target_x] != MazeElement::END)
      continue;

    // keep searching the next node via recursion call DFS
    if (solveMazeDFS(target_y, target_x)) {
      if (maze[target_y][target_x] != MazeElement::END) {
        maze[target_y][target_x] = MazeElement::ANSWER;
        controller_ptr__->enFramequeue(maze);    // append the maze to the framequeue, I dont want to render the update node here so only one argument here

        solve_cost__++;
        solve_cell__++;
      }

      if (is_first_call)
        controller_ptr__->setModelComplete();

      return true;
    }
  }

  // If we get here, no path was found, it should not happen
  if (is_first_call)
    controller_ptr__->setModelComplete();

  return false;
}    // end solveMazeDFS()

/**
 * @brief Solve the maze by BFS algorithm.
 */
void MazeModel::solveMazeBFS()
{
  cleanExplored__();
  solve_cost__ = 0;
  solve_cell__ = 0;

  std::vector<std::vector<MazeNode>> come_from(MAZE_HEIGHT, std::vector<MazeNode>(MAZE_WIDTH, { -1, -1, MazeElement::INVALID }));    // the vector to reconstruct the answer open_list
  std::queue<std::pair<int32_t, int32_t>> open_list;    // queue to store the nodes
  open_list.push(std::make_pair(BEGIN_Y, BEGIN_X));

  while (!open_list.empty()) {
    const auto [current_y, current_x] = std::move(open_list.front());
    open_list.pop();

    // iterate the current node's up, down, left, right
    for (const auto &[dir_y, dir_x] : dir_vec) {
      const int32_t target_y = current_y + dir_y;
      const int32_t target_x = current_x + dir_x;

      if (!inMaze__(target_y, target_x) || maze[target_y][target_x] == MazeElement::WALL)
        continue;

      // find the goal node
      if (maze[target_y][target_x] == MazeElement::END) {
        int32_t ans_y = current_y;
        int32_t ans_x = current_x;

        // reconstruct the answer path
        while (ans_y != BEGIN_Y || ans_x != BEGIN_X) {
          maze[ans_y][ans_x] = MazeElement::ANSWER;
          controller_ptr__->enFramequeue(maze);    // append the maze to the framequeue, I dont want to render the update node here so only one argument here

          solve_cost__++;
          solve_cell__++;

          const auto [parent_y, parent_x, _] = come_from[ans_y][ans_x];
          ans_y = parent_y;
          ans_x = parent_x;
        }

        controller_ptr__->setModelComplete();
        return;
      }

      if (maze[target_y][target_x] == MazeElement::GROUND) {
        maze[target_y][target_x] = MazeElement::EXPLORED;
        controller_ptr__->enFramequeue(maze, MazeNode{ target_y, target_x, MazeElement::EXPLORED });

        come_from[target_y][target_x] = { current_y, current_x, MazeElement::EXPLORED };
        open_list.push(std::make_pair(target_y, target_x));
      }
    }    // end for(const auto &[dir_y, dir_x] : dir_vec)
  }    // end while

  // If we get here, no path was found, it should not happen
  controller_ptr__->setModelComplete();
}    // end solveMazeBFS()

/**
 * @brief Solve the maze by A* algorithm, because the UCS and Greedy best-first search algorithm are special cases of A* algorithm, so I merge them into one function to solve the maze.
 * @param actions The action of the maze solving algorithm.
 *        - S_ASTAR_MANHATTAN: A* algorithm with Manhattan distance as heuristic function
 *        - S_ASTAR_EUCLIDEAN: A* algorithm with Euclidean distance as heuristic function
 *        - S_GREEDY_MANHATTAN: Greedy best-first search algorithm with Manhattan distance as heuristic function
 *        - S_GREEDY_TWO_NORM: Greedy best-first search algorithm with 2-norm distance as heuristic function
 *        - S_UCS: Uniform-cost search algorithm
 */
void MazeModel::solveMazeAStar(const MazeAction actions)
{
  cleanExplored__();
  solve_cost__ = 0;
  solve_cell__ = 0;

  struct TraceNode {
    int32_t f_score;    // f(n) = g(n) + h(n)
    int32_t g_score;    // g(n): cost from start to current node
    int32_t h_score;    // h(n): heuristic cost from current node to goal
    int32_t y;
    int32_t x;
    TraceNode(int32_t f, int32_t g, int32_t h, int32_t y, int32_t x)
        : f_score(f), g_score(g), h_score(h), y(y), x(x) {}
    bool operator>(const TraceNode &other) const { return f_score > other.f_score; }
  };

  auto calcHeuristic = [&](const int32_t y, const int32_t x) -> int32_t {
    switch (actions) {
    case MazeAction::S_ASTAR_MANHATTAN:
    case MazeAction::S_GREEDY_MANHATTAN:
      return std::abs(END_X - x) + std::abs(END_Y - y);    // Manhattan distance
    default:
      return std::sqrt(std::pow(END_Y - y, 2) + std::pow(END_X - x, 2));    // Euclidean distance
    }
  };

  auto calcFScore = [&](const int32_t g, const int32_t h) -> int32_t {
    switch (actions) {
    case MazeAction::S_UCS:
      return g;
    case MazeAction::S_GREEDY_MANHATTAN:
    case MazeAction::S_GREEDY_TWO_NORM:
      return h;
    default:
      return g + h;
    }
  };

  std::vector<std::vector<std::pair<int32_t, int32_t>>> come_from(MAZE_HEIGHT, std::vector<std::pair<int32_t, int32_t>>(MAZE_WIDTH, { -1, -1 }));
  std::vector<std::vector<int32_t>> f_map(MAZE_HEIGHT, std::vector<int32_t>(MAZE_WIDTH, std::numeric_limits<int32_t>::max()));
  std::vector<std::vector<int32_t>> g_map(MAZE_HEIGHT, std::vector<int32_t>(MAZE_WIDTH, 1));
  std::priority_queue<TraceNode, std::vector<TraceNode>, std::greater<TraceNode>> open_list;

  {
    int32_t h_score = calcHeuristic(BEGIN_Y, BEGIN_X);
    int32_t g_score = g_map[BEGIN_Y][BEGIN_X];
    int32_t f_score = calcFScore(g_score, h_score);
    open_list.emplace(f_score, g_score, h_score, BEGIN_Y, BEGIN_X);
    f_map[BEGIN_Y][BEGIN_X] = f_score;
  }

  while (!open_list.empty()) {
    const auto current = std::move(open_list.top());
    open_list.pop();

    for (const auto &[dir_y, dir_x] : dir_vec) {
      const int32_t ny = current.y + dir_y;
      const int32_t nx = current.x + dir_x;

      if (!inMaze__(ny, nx) || maze[ny][nx] == MazeElement::WALL)
        continue;

      const int32_t h_score = calcHeuristic(ny, nx);
      const int32_t g_score = current.g_score + g_map[ny][nx];
      const int32_t f_score = calcFScore(g_score, h_score);
      TraceNode target_node(f_score, g_score, h_score, ny, nx);

      if (maze[ny][nx] == MazeElement::END) {
        int32_t ans_y = current.y;
        int32_t ans_x = current.x;
        while (ans_y != BEGIN_Y || ans_x != BEGIN_X) {
          maze[ans_y][ans_x] = MazeElement::ANSWER;
          controller_ptr__->enFramequeue(maze);

          solve_cost__ += f_map[ans_y][ans_x];
          solve_cell__++;

          const auto [parent_y, parent_x] = come_from[ans_y][ans_x];
          ans_y = parent_y;
          ans_x = parent_x;
        }

        controller_ptr__->setModelComplete();
        return;
      }

      if (maze[ny][nx] == MazeElement::GROUND) {
        maze[ny][nx] = MazeElement::EXPLORED;
        controller_ptr__->enFramequeue(maze, MazeNode{ ny, nx, MazeElement::EXPLORED });

        f_map[ny][nx] = target_node.f_score;
        come_from[ny][nx] = { current.y, current.x };
        open_list.push(target_node);
      }
      else if (maze[ny][nx] == MazeElement::EXPLORED) {
        if (f_map[ny][nx] > target_node.f_score) {
          f_map[ny][nx] = target_node.f_score;
          come_from[ny][nx] = { current.y, current.x };
        }
      }
    }
  }

  // If we get here, no path was found, it should not happen
  controller_ptr__->setModelComplete();
}    // end solveMazeAStar()

/* -------------------- private utility function --------------------   */

/**
 * @brief Clean the explored and answer status in the maze.
 */
void MazeModel::cleanExplored__()
{
  for (auto &row : maze) {
    std::replace(row.begin(), row.end(), MazeElement::EXPLORED, MazeElement::GROUND);
    std::replace(row.begin(), row.end(), MazeElement::ANSWER, MazeElement::GROUND);
  }

  controller_ptr__->enFramequeue(maze);
  setFlag__();
}

/**
 * @brief Initialize the maze, set the wall and ground into grid-shaped.
 */
void MazeModel::initMaze__()
{
  for (int32_t y{}; y < MAZE_HEIGHT; ++y) {
    for (int32_t x{}; x < MAZE_WIDTH; ++x) {
      if (y == 0 || y == MAZE_HEIGHT - 1 || x == 0 || x == MAZE_WIDTH - 1)
        maze[y][x] = MazeElement::WALL;
      else if (x % 2 == 1 && y % 2 == 1)    // odd for ground
        maze[y][x] = MazeElement::GROUND;
      else
        maze[y][x] = MazeElement::WALL;
    }
  }

  controller_ptr__->enFramequeue(maze);
}

/**
 * @brief Reset the wall around the maze.
 */
void MazeModel::resetWallAroundMaze__()
{
  for (int32_t y = 0; y < MAZE_HEIGHT; ++y) {
    for (int32_t x = 0; x < MAZE_WIDTH; ++x) {
      if (x == 0 || x == MAZE_WIDTH - 1 || y == 0 || y == MAZE_HEIGHT - 1)
        maze[y][x] = MazeElement::WALL;    // Wall
      else
        maze[y][x] = MazeElement::GROUND;    // Ground
    }
  }

  controller_ptr__->enFramequeue(maze);
}

/**
 * @brief Set the begin and end flag in the maze.
 */
void MazeModel::setFlag__()
{
  maze[BEGIN_Y][BEGIN_X] = MazeElement::BEGIN;
  maze[END_Y][END_X] = MazeElement::END;

  controller_ptr__->enFramequeue(maze);
}

/**
 * @brief Check if the node is in the wall, which means (y < MAZE_HEIGHT - 1) && (x < MAZE_WIDTH - 1) && (y > 0) && (x > 0)
 * @param y The y-coordinate of the node.
 * @param x The x-coordinate of the node.
 * @return Whether the node is in the wall.
 */
bool MazeModel::inWall__(const int32_t y, const int32_t x)
{
  return (y < MAZE_HEIGHT - 1) && (x < MAZE_WIDTH - 1) && (y > 0) && (x > 0);
}

/**
 * @brief Check if the node is in the maze, which means (y < MAZE_HEIGHT) && (x < MAZE_WIDTH) && (y > -1) && (x > -1)
 * @param y The y-coordinate of the node.
 * @param x The x-coordinate of the node.
 * @return Whether the node is in the maze.
 */
bool MazeModel::inMaze__(const int32_t y, const int32_t x)
{
  return (y < MAZE_HEIGHT) && (x < MAZE_WIDTH) && (y > -1) && (x > -1);
}

/**
 * @brief generate a random begin point for maze generation algorithm
 *
 * @param seed_y
 * @param seed_x
 */
void MazeModel::setBeginPoint__(MazeNode &node)
{
  std::mt19937 gen(std::chrono::high_resolution_clock::now().time_since_epoch().count());
  std::uniform_int_distribution<> y_dis(0, (MAZE_HEIGHT - 3) / 2);
  std::uniform_int_distribution<> x_dis(0, (MAZE_WIDTH - 3) / 2);

  node.y = 2 * y_dis(gen) + 1;
  node.x = 2 * x_dis(gen) + 1;
  node.element = MazeElement::EXPLORED;
  maze[node.y][node.x] = MazeElement::EXPLORED;    // set the randomly chosen point as the generation start point

  controller_ptr__->enFramequeue(maze);
}