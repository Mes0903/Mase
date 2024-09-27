#include "imgui.h"
#include "imgui_internal.h"

#include "MazeController.h"
#include "MazeModel.h"
#include "MazeView.h"

void MazeController::init(MazeModel *model_ptr, MazeView *view_ptr)
{
  this->model_ptr = std::unique_ptr<MazeModel>(model_ptr);
  this->view_ptr = std::unique_ptr<MazeView>(view_ptr);
}

void MazeController::handleInput()
{
  // This method will be called in the main loop to handle user input
  // You can add ImGui::IsItemClicked() checks here to handle button clicks
  if (ImGui::IsItemClicked(0)) {    // 0 for left mouse button
    if (ImGui::GetActiveID() == ImGui::GetID("Generate Maze (Prim's)")) {
      model_ptr->generateMazePrim(0);
    }
    else if (ImGui::GetActiveID() == ImGui::GetID("Generate Maze (Recursion)")) {
      model_ptr->generateMazeRecursion();
    }
    else if (ImGui::GetActiveID() == ImGui::GetID("Generate Maze (Recursion Division)")) {
      model_ptr->generateMazeRecursionDivision(1, 1, MAZE_HEIGHT - 2, MAZE_WIDTH - 2);
    }
    else if (ImGui::GetActiveID() == ImGui::GetID("Solve Maze (DFS)")) {
      model_ptr->solveMazeDFS(1, 0);
    }
    else if (ImGui::GetActiveID() == ImGui::GetID("Solve Maze (BFS)")) {
      model_ptr->solveMazeBFS();
    }
    else if (ImGui::GetActiveID() == ImGui::GetID("Solve Maze (UCS)")) {
      model_ptr->solveMazeUCS(0);
    }
    else if (ImGui::GetActiveID() == ImGui::GetID("Solve Maze (Greedy)")) {
      model_ptr->solveMazeGreedy();
    }
    else if (ImGui::GetActiveID() == ImGui::GetID("Solve Maze (A*)")) {
      model_ptr->solveMazeAStar(0);
    }
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
