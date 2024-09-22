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

class MazeModel {
public:
  MazeModel(int height, int width);
  void resetMaze();
  void resetWallAroundMaze();
  int getMazeCell(int y, int x) const;
  void setMazeCell(int y, int x, int value);
  std::pair<int, int> getBufferNode() const;
  void setBufferNode(int y, int x);

  // maze generation and solving methods
  void generateMazePrim(const int &types);
  void generateMazeRecursion();
  void generateMazeRecursionDivision(const int uy, const int lx, const int dy, const int rx);

  bool solveMazeDFS(const int &y, const int &x);
  void solveMazeBFS();
  void solveMazeUCS(const int &types);
  void solveMazeGreedy();
  void solveMazeAStar(const int &types);

private:
  std::vector<std::vector<int>> maze;
  std::pair<int, int> bufferNode;

  void set_Flag();
  inline bool if_in_wall(const int &y, const int &x, const int &delta_y, const int &delta_x);
  void empty_map();
  inline void initializing_maze();

  void set_begin_point(int &seed_y, int &seed_x, std::deque<std::pair<int, int>> &re_load);
  void set_end_point(int &seed_y, int &seed_x, std::deque<std::pair<int, int>> &re_load);
  bool is_in_maze(const int &y, const int &x);
  int pow_two_norm(const int &y, const int &x);
};

#endif