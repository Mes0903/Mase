#ifndef MAZEMODEL_H
#define MAZEMODEL_H

/**
 * @file MazeModel.h
 * @author Mes (mes900903@gmail.com)
 * @brief The model of the maze, build and store the maze
 * @version 0.1
 * @date 2024-09-22
 */
#include <vector>
#include <deque>
#include <memory>
#include <utility>

inline constexpr int MAZE_HEIGHT = 39;
inline constexpr int MAZE_WIDTH = 75;
inline constexpr int BEGIN_Y = 1;
inline constexpr int BEGIN_X = 0;
inline constexpr int END_Y = MAZE_HEIGHT - 2;
inline constexpr int END_X = MAZE_WIDTH - 1;
inline constexpr int GRID_SIZE = 25;
inline constexpr std::pair<int, int> directions[4]{ { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
inline constexpr std::pair<int, int> GROUND_Direction[4]{ { 2, 0 }, { 0, 2 }, { -2, 0 }, { 0, -2 } };

enum class Maze_Elements : int {
  GROUND = 0,
  WALL = 1,
  EXPLORED = 2,
  BEGIN = 9,
  END = 10,
};

enum class MazeAction : int {
  G_RESET,
  G_PRIMS,
  G_PRIMS_BREAK_WALL,
  G_RECURSION_BACKTRACKER,
  G_RECURSION_DIVISION,
  S_DFS,
  S_BFS,
  S_UCS_MANHATTAN,    // Cost Function 為 Two_Norm，所以距離終點越遠 Cost 越大
  S_UCS_TWO_NORM,    // Cost Function 為 Two_Norm，所以距離終點越遠 Cost 越大
  S_UCS_INTERVAL,    // Cost Function 以區間來計算，每一個區間 Cost 差10，距離終點越遠 Cost 越大
  S_GREEDY,
  S_ASTAR,
  S_ASTAR_INTERVAL
};

class MazeController;

class MazeModel {
public:
  MazeModel(int height, int width);
  void setController(MazeController *controller_ptr);

public:
  void resetMaze();
  void resetWallAroundMaze();
  int getMazeCell(int y, int x) const;
  void setMazeCell(int y, int x, int value);
  std::pair<int, int> getBufferNode() const;
  void setBufferNode(int y, int x);

  // maze generation and solving methods
  void generateMazePrim(const MazeAction actions);
  void generateMazeRecursionBacktracker();
  void generateMazeRecursionDivision(const int uy, const int lx, const int dy, const int rx);

  bool solveMazeDFS(const int y, const int x);
  void solveMazeBFS();
  void solveMazeUCS(const MazeAction actions);
  void solveMazeGreedy();
  void solveMazeAStar(const MazeAction actions);

private:
  std::unique_ptr<MazeController> controller_ptr;

  std::vector<std::vector<int>> maze;
  std::pair<int, int> bufferNode;

  void set_Flag();
  bool if_in_wall(const int y, const int x, const int delta_y, const int delta_x);
  void empty_map();
  void initializing_maze();

  void set_begin_point(int &seed_y, int &seed_x, std::deque<std::pair<int, int>> &re_load);
  void set_end_point(int &seed_y, int &seed_x, std::deque<std::pair<int, int>> &re_load);
  bool is_in_maze(const int y, const int x);
  int pow_two_norm(const int y, const int x);
};

#endif