#ifndef MAZECONTROLLER_H
#define MAZECONTROLLER_H

/**
 * @file MazeController.h
 * @author Mes
 * @brief The controller of the maze, accepts and processes the user's action.
 * @version 0.1
 * @date 2024-10-14
 */

#include "MazeModel.h"
#include "MazeView.h"
#include "MazeDefine.h"

#include <atomic>

class MazeModel;
class MazeView;

class MazeController {
public:
  void setModelView(MazeModel *model_ptr, MazeView *view_ptr);

  void handleAction(const MazeAction action);
  void enFramequeue(const std::vector<std::vector<MazeElement>> &maze, const MazeNode &node = MazeNode{ -1, -1, MazeElement::INVALID });

  void setModelComplete();
  bool isModelComplete() const;

  void setViewComplete();
  bool isViewComplete() const;

  int32_t getSolveCost() const;
  int32_t getSolveCell() const;

private:
  std::atomic<bool> model_complete_flag__ = true;
  std::atomic<bool> view_complete_flag__ = true;

  MazeModel *model_ptr__;
  MazeView *view_ptr__;
};

#endif
