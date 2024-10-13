#ifndef MAZE_DEFINE_H
#define MAZE_DEFINE_H

#include <cstdint>
#include <utility>

inline constexpr int32_t MAZE_HEIGHT = 39;
inline constexpr int32_t MAZE_WIDTH = 75;
inline constexpr int32_t BEGIN_Y = 1;
inline constexpr int32_t BEGIN_X = 0;
inline constexpr int32_t END_Y = MAZE_HEIGHT - 2;
inline constexpr int32_t END_X = MAZE_WIDTH - 1;
inline constexpr int32_t GRID_SIZE = 15;
inline constexpr std::pair<int32_t, int32_t> dir_vec[4]{ { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };

enum class MazeAction : int32_t {
  G_CLEANALL,
  G_CLEAN_EXPLORED,
  G_PRIM,
  G_PRIM_BREAK,
  G_RECURSION_BACKTRACKER,
  G_RECURSION_DIVISION,
  S_DFS,
  S_BFS,
  S_UCS_MANHATTAN,    // Cost Function 為 Two_Norm，所以距離終點越遠 Cost 越大
  S_UCS_TWO_NORM,    // Cost Function 為 Two_Norm，所以距離終點越遠 Cost 越大
  S_GREEDY_MANHATTAN,
  S_GREEDY_TWO_NORM,
  S_ASTAR_MANHATTAN,
  S_ASTAR_TWO_NORM,
};

enum class MazeElement : int8_t {
  INVALID = -1,
  WALL = 0,
  GROUND,
  EXPLORED,
  ANSWER,
  BEGIN,
  END,
};

enum class MazeDistance : int32_t {
  D_MANHATTAN,
  D_TWO_NORM,
};

struct MazeNode {
  int32_t y, x;
  MazeElement element;
};
#endif
