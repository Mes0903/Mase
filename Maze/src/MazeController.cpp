#include "MazeController.h"
#include "MazeModel.h"
#include "MazeView.h"

#include <thread>
#include <iostream>

void MazeController::setModelView(MazeModel *model_ptr, MazeView *view_ptr)
{
  this->model_ptr = model_ptr;
  this->view_ptr = view_ptr;

  this->model_ptr->setController(this);
  this->view_ptr->setController(this);
}

void MazeController::handleInput(const MazeAction actions)
{
  std::thread t1;

  switch (actions) {
  case MazeAction::G_CLEANALL:
    model_ptr->emptyMap();
    break;
  case MazeAction::G_CLEAN_EXPLORER:
    model_ptr->cleanExplorer();
    break;
  case MazeAction::G_INIT:
    model_ptr->initMaze();
    break;
  case MazeAction::G_PRIMS:
    t1 = std::thread(&MazeModel::generateMazePrim, model_ptr);
    t1.detach();
    break;
  case MazeAction::G_RECURSION_BACKTRACKER:
    t1 = std::thread(&MazeModel::generateMazeRecursionBacktracker, model_ptr);
    t1.detach();
    break;
  case MazeAction::G_RECURSION_DIVISION:
    model_ptr->generateMazeRecursionDivision(1, 1, MAZE_HEIGHT - 2, MAZE_WIDTH - 2);
    break;
  case MazeAction::S_DFS:
    model_ptr->solveMazeDFS(1, 0);
    break;
  case MazeAction::S_BFS:
    model_ptr->solveMazeBFS();
    break;
  case MazeAction::S_UCS_MANHATTAN:
    model_ptr->solveMazeUCS(actions);
    break;
  case MazeAction::S_UCS_TWO_NORM:
    model_ptr->solveMazeUCS(actions);
    break;
  case MazeAction::S_UCS_INTERVAL:
    model_ptr->solveMazeUCS(actions);
    break;
  case MazeAction::S_GREEDY:
    model_ptr->solveMazeGreedy();
    break;
  case MazeAction::S_ASTAR:
    model_ptr->solveMazeAStar(actions);
    break;
  case MazeAction::S_ASTAR_INTERVAL:
    model_ptr->solveMazeAStar(actions);
    break;
  default:
    std::clog << "invalid action" << std::endl;
    break;
  }

  model_complete_flag.store(false);
}

void MazeController::setFrameMaze(const std::vector<std::vector<MazeElement>> &maze)
{
  view_ptr->setFrameMaze(maze);
}

void MazeController::enFramequeue(const MazeNode &node)
{
  view_ptr->enFramequeue(node);
}

void MazeController::setModelComplete()
{
  model_complete_flag.store(true);
}
bool MazeController::isModelComplete() const
{
  return model_complete_flag.load();
}