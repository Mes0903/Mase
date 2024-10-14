#include "MazeController.h"
#include "MazeModel.h"
#include "MazeView.h"

#include <thread>
#include <iostream>

/**
 * @brief Set the model and view pointer of the maze.
 * @param model_ptr The pointer to the model.
 * @param view_ptr The pointer to the view.
 */
void MazeController::setModelView(MazeModel *model_ptr, MazeView *view_ptr)
{
  this->model_ptr__ = model_ptr;
  this->view_ptr__ = view_ptr;

  this->model_ptr__->setController(this);
  this->view_ptr__->setController(this);
}

/**
 * @brief Handle the action come from the view.
 * @param actions The action to be handled.
 */
void MazeController::handleAction(const MazeAction actions)
{
  if (!isModelComplete() || !isViewComplete())
    return;

  model_complete_flag__.store(false);
  view_complete_flag__.store(false);

  std::thread t1;
  switch (actions) {
  case MazeAction::G_CLEANALL:
    model_ptr__->emptyMap();
    setModelComplete();
    break;
  case MazeAction::G_PRIM:
  case MazeAction::G_PRIM_BREAK:
    t1 = std::thread(&MazeModel::generateMazePrim, model_ptr__, actions);
    t1.detach();
    break;
  case MazeAction::G_RECURSION_BACKTRACKER:
    t1 = std::thread(&MazeModel::generateMazeRecursionBacktracker, model_ptr__);
    t1.detach();
    break;
  case MazeAction::G_RECURSION_DIVISION:
    t1 = std::thread(&MazeModel::generateMazeRecursionDivision, model_ptr__, 1, 1, MAZE_HEIGHT - 2, MAZE_WIDTH - 2, true);
    t1.detach();
    break;
  case MazeAction::S_DFS:
    t1 = std::thread(&MazeModel::solveMazeDFS, model_ptr__, BEGIN_Y, BEGIN_X, true);
    t1.detach();
    break;
  case MazeAction::S_BFS:
    t1 = std::thread(&MazeModel::solveMazeBFS, model_ptr__);
    t1.detach();
    break;
  case MazeAction::S_UCS:
  case MazeAction::S_GREEDY_MANHATTAN:
  case MazeAction::S_GREEDY_TWO_NORM:
  case MazeAction::S_ASTAR_MANHATTAN:
  case MazeAction::S_ASTAR_TWO_NORM:
    t1 = std::thread(&MazeModel::solveMazeAStar, model_ptr__, actions);
    t1.detach();
    break;
  default:;    // should not reach here, do nothing
  }
}

void MazeController::enFramequeue(const std::vector<std::vector<MazeElement>> &maze, const MazeNode &node)
{
  view_ptr__->enFramequeue(maze, node);
}

/**
 * @brief Set the model complete flag to true. Also it will reset the update node in the view.
 */
void MazeController::setModelComplete()
{
  view_ptr__->resetUpdateNode();
  model_complete_flag__.store(true);
}

bool MazeController::isModelComplete() const
{
  return model_complete_flag__.load();
}

void MazeController::setViewComplete()
{
  view_complete_flag__.store(true);
}

bool MazeController::isViewComplete() const
{
  return view_complete_flag__.load();
}

int32_t MazeController::getSolveCost() const
{
  return model_ptr__->getSolveCost();
}

int32_t MazeController::getSolveCell() const
{
  return model_ptr__->getSolveCell();
}