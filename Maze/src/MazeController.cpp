#include "imgui.h"
#include "imgui_internal.h"

#include "MazeController.h"
#include "MazeModel.h"
#include "MazeView.h"

#include <iostream>
#include <thread>


void MazeController::setModelView(MazeModel *model_ptr, MazeView *view_ptr)
{
  this->model_ptr = std::unique_ptr<MazeModel>(model_ptr);
  this->view_ptr = std::unique_ptr<MazeView>(view_ptr);
}

void MazeController::handleInput(const MazeAction actions)
{
  std::thread t1;

  switch (actions) {
  case MazeAction::G_RESET:
    model_ptr->resetMaze();
    break;
  case MazeAction::G_PRIMS:
    t1 = std::thread(&MazeModel::generateMazePrim, std::ref(*model_ptr));
    t1.join();
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

void MazeController::setFrameMaze(const std::vector<std::vector<MazeElement>> &maze)
{
  view_ptr->setFrameMaze(maze);
}

void MazeController::enFramequeue(const MazeNode &node)
{
  view_ptr->enFramequeue(node);
}