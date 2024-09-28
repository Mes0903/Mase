#include "imgui.h"
#include "imgui_internal.h"

#include "MazeController.h"
#include "MazeModel.h"
#include "MazeView.h"

#include <iostream>

void MazeController::setModelView(MazeModel *model_ptr, MazeView *view_ptr)
{
  this->model_ptr = std::unique_ptr<MazeModel>(model_ptr);
  this->view_ptr = std::unique_ptr<MazeView>(view_ptr);
}

void MazeController::handleInput(const MazeAction actions)
{
  switch (actions) {
  case MazeAction::G_RESET:
    model_ptr->resetMaze();
    break;
  case MazeAction::G_PRIMS:
    model_ptr->generateMazePrim(actions);
    break;
  case MazeAction::G_PRIMS_BREAK_WALL:
    model_ptr->generateMazePrim(actions);
    break;
  case MazeAction::G_RECURSION_BACKTRACKER:
    model_ptr->generateMazeRecursionBacktracker();
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
}

// api to conatact model_ptr
int MazeController::getMazeCell(int y, int x) const
{
  return model_ptr->getMazeCell(y, x);
}

std::pair<int, int> MazeController::getBufferNode() const
{
  return model_ptr->getBufferNode();
}

void MazeController::initializing_maze()
{
  model_ptr->resetMaze();
}
