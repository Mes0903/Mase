#ifndef MAZEMODEL_H
#define MAZEMODEL_H

/**
 * @file MazeModel.h
 * @author Mes (mes900903@gmail.com)
 * @brief The model of the maze, build and store the maze
 * @version 0.1
 * @date 2024-09-22
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

  void emptyMap();
  void cleanExplored();
  void resetWallAroundMaze();

  // maze generation and solving methods
  void generateMazePrim(const MazeAction actions);
  void generateMazeRecursionBacktracker();
  void generateMazeRecursionDivision(const int32_t uy, const int32_t lx, const int32_t dy, const int32_t rx, bool is_first_call = false);

  bool solveMazeDFS(const int32_t y, const int32_t x, bool is_first_call = false);
  void solveMazeBFS();
  void solveMazeUCS(const MazeAction actions);
  void solveMazeGreedy(const MazeAction actions);
  void solveMazeAStar(const MazeAction actions);

public:
  std::vector<std::vector<MazeElement>> maze;

private:
  MazeController *controller_ptr__;

private:
  bool inWall__(const int32_t y, const int32_t x);
  bool inMaze__(const int32_t y, const int32_t x);
  void initMaze__();
  void setFlag__();
  void setBeginPoint__(MazeNode &node);

  int32_t twoNorm__(const int32_t y, const int32_t x);
};

#endif