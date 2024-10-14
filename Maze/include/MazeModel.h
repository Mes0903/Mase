#ifndef MAZEMODEL_H
#define MAZEMODEL_H

/**
 * @file MazeModel.h
 * @author Mes (mes900903@gmail.com)
 * @brief The model of the maze, build and store the maze
 * @version 0.1
 * @date 2024-10-14
 */

#include "MazeController.h"
#include "MazeDefine.h"

#include <vector>
#include <utility>
#include <cstdint>

class MazeController;

class MazeModel {
public:
  MazeModel(uint32_t height, uint32_t width);

  void setController(MazeController *controller_ptr);
  int32_t getSolveCost() const;
  int32_t getSolveCell() const;

  void emptyMap();

  // maze generation and solving methods
  void generateMazePrim(const MazeAction actions);
  void generateMazeRecursionBacktracker();
  void generateMazeRecursionDivision(const int32_t uy, const int32_t lx, const int32_t dy, const int32_t rx, bool is_first_call = false);

  bool solveMazeDFS(const int32_t y, const int32_t x, bool is_first_call = false);
  void solveMazeBFS();
  void solveMazeAStar(const MazeAction actions);

public:
  std::vector<std::vector<MazeElement>> maze;

private:
  void cleanExplored__();
  void initMaze__();
  void resetWallAroundMaze__();
  void setFlag__();
  bool inWall__(const int32_t y, const int32_t x);
  bool inMaze__(const int32_t y, const int32_t x);
  void setBeginPoint__(MazeNode &node);

private:
  MazeController *controller_ptr__;
  int32_t solve_cost__;
  int32_t solve_cell__;
};

#endif