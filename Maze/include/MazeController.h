#ifndef MAZECONTROLLER_H
#define MAZECONTROLLER_H

/**
 * @file MazeController.h
 * @author Mes (mes900903@gmail.com)
 * @brief The controller of the maze, accept and process the user's action
 * @version 0.1
 * @date 2024-09-22
 */

#include "MazeModel.h"
#include "MazeView.h"

#include <memory>

class MazeModel;
class MazeView;

class MazeController {
public:
  void setModelView(MazeModel *model_ptr, MazeView *view_ptr);

  void handleInput(const MazeAction action);
  void setFrameMaze(const std::vector<std::vector<MazeElement>> &maze);
  void enFramequeue(const int32_t y, const int32_t x, const MazeElement element);

public:
  std::mutex g_mutex;

private:
  std::unique_ptr<MazeModel> model_ptr;
  std::unique_ptr<MazeView> view_ptr;
};

#endif
