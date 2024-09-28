#include "imgui.h"

#include "MazeController.h"
#include "MazeModel.h"
#include "MazeView.h"

#include <memory>

void MazeView::render()
{
  renderGUI();
}

void MazeView::setController(MazeController *controller_ptr)
{
  this->controller_ptr = std::unique_ptr<MazeController>(controller_ptr);
}

void MazeView::renderMaze()
{
  ImDrawList *draw_list = ImGui::GetWindowDrawList();
  const ImVec2 p = ImGui::GetCursorScreenPos();
  const float cell_size = 15.0f;
  const ImU32 colors[] = {
    IM_COL32(255, 255, 255, 255),    // Ground
    IM_COL32(115, 64, 70, 255),    // Wall
    IM_COL32(231, 158, 79, 255),    // Explored
    IM_COL32(35, 220, 130, 255),    // Begin
    IM_COL32(250, 50, 150, 255)    // End
  };

  for (int y = 0; y < MAZE_HEIGHT; ++y) {
    for (int x = 0; x < MAZE_WIDTH; ++x) {
      int cell = controller_ptr->getMazeCell(y, x);
      ImVec2 cell_min = ImVec2(p.x + x * cell_size, p.y + y * cell_size);
      ImVec2 cell_max = ImVec2(cell_min.x + cell_size, cell_min.y + cell_size);
      draw_list->AddRectFilled(cell_min, cell_max, colors[cell]);
      draw_list->AddRect(cell_min, cell_max, IM_COL32(100, 100, 100, 255));
    }
  }

  auto [buffer_y, buffer_x] = controller_ptr->getBufferNode();
  if (buffer_y != -1 && buffer_x != -1) {
    ImVec2 buffer_min = ImVec2(p.x + buffer_x * cell_size, p.y + buffer_y * cell_size);
    ImVec2 buffer_max = ImVec2(buffer_min.x + cell_size, buffer_min.y + cell_size);
    draw_list->AddRectFilled(buffer_min, buffer_max, IM_COL32(50, 215, 250, 255));
  }
}

void MazeView::renderGUI()
{
  ImGui::Begin("Maze Generator and Solver");

  ImGui::BeginGroup();
  if (ImGui::Button("Generate Maze (Prim's)")) controller_ptr->handleInput(MazeAction::G_PRIMS);
  if (ImGui::Button("Generate Maze (Prim's Break Wall)")) controller_ptr->handleInput(MazeAction::G_PRIMS_BREAK_WALL);
  if (ImGui::Button("Generate Maze (Recursion Backtracker)")) controller_ptr->handleInput(MazeAction::G_RECURSION_BACKTRACKER);
  if (ImGui::Button("Generate Maze (Recursion Division)")) controller_ptr->handleInput(MazeAction::G_RECURSION_DIVISION);
  if (ImGui::Button("Solve Maze (DFS)")) controller_ptr->handleInput(MazeAction::S_DFS);
  if (ImGui::Button("Solve Maze (BFS)")) controller_ptr->handleInput(MazeAction::S_BFS);
  if (ImGui::Button("Solve Maze (UCS Manhattan)")) controller_ptr->handleInput(MazeAction::S_UCS_MANHATTAN);
  if (ImGui::Button("Solve Maze (UCS Two Norm)")) controller_ptr->handleInput(MazeAction::S_UCS_TWO_NORM);
  if (ImGui::Button("Solve Maze (UCS Interval)")) controller_ptr->handleInput(MazeAction::S_UCS_INTERVAL);
  if (ImGui::Button("Solve Maze (Greedy)")) controller_ptr->handleInput(MazeAction::S_GREEDY);
  if (ImGui::Button("Solve Maze (A*)")) controller_ptr->handleInput(MazeAction::S_ASTAR);
  if (ImGui::Button("Solve Maze (A* Interval)")) controller_ptr->handleInput(MazeAction::S_ASTAR_INTERVAL);
  ImGui::EndGroup();

  ImGui::SameLine();
  ImGui::BeginChild("MazeView", ImVec2(0, 0), true);
  renderMaze();
  ImGui::EndChild();

  ImGui::End();
}