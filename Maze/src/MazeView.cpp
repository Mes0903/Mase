#include "imgui.h"

#include "MazeController.h"
#include "MazeModel.h"
#include "MazeView.h"
#include "MazeNode.h"

#include <memory>
#include <cstdint>

MazeView::MazeView(uint32_t height, uint32_t width)
    : render_maze{ height, std::vector<MazeElement>{ width, MazeElement::GROUND } }, update_node{ MazeNode{ -1, -1, MazeElement::INVALID } }, stop_flag{ false } {}

void MazeView::setController(MazeController *controller_ptr)
{
  this->controller_ptr = std::unique_ptr<MazeController>(controller_ptr);
}

void MazeView::setFrameMaze(const std::vector<std::vector<MazeElement>> &maze)
{
  std::lock_guard<std::mutex> lock(controller_ptr->g_mutex);
  render_maze = maze;
}

void MazeView::enFramequeue(const MazeNode &node)
{
  std::lock_guard<std::mutex> lock(controller_ptr->g_mutex);
  frame_queue.emplace(node);
}

void MazeView::deFramequeue()
{
  std::lock_guard<std::mutex> lock(controller_ptr->g_mutex);
  if (frame_queue.empty())
    return;

  update_node = frame_queue.front();
  frame_queue.pop();
  if (update_node.y != -1 && update_node.x != -1)
    render_maze[update_node.y][update_node.x] = update_node.element;
}

void MazeView::renderMaze()
{
  ImDrawList *draw_list = ImGui::GetWindowDrawList();
  const ImVec2 p = ImGui::GetCursorScreenPos();
  const float cell_size = 15.0f;

  for (int32_t y = 0; y < MAZE_HEIGHT; ++y) {
    for (int32_t x = 0; x < MAZE_WIDTH; ++x) {
      MazeElement cell = render_maze[y][x];
      ImVec2 cell_min = ImVec2(p.x + x * cell_size, p.y + y * cell_size);
      ImVec2 cell_max = ImVec2(cell_min.x + cell_size, cell_min.y + cell_size);

      if (update_node.y == y && update_node.x == x)
        draw_list->AddRectFilled(cell_min, cell_max, IM_COL32(50, 215, 250, 255));
      else if (cell == MazeElement::BEGIN)
        draw_list->AddRectFilled(cell_min, cell_max, IM_COL32(35, 220, 130, 255));
      else if (cell == MazeElement::END)
        draw_list->AddRectFilled(cell_min, cell_max, IM_COL32(250, 50, 150, 255));
      else if (cell == MazeElement::WALL)
        draw_list->AddRectFilled(cell_min, cell_max, IM_COL32(115, 64, 70, 255));
      else if (cell == MazeElement::GROUND)
        draw_list->AddRectFilled(cell_min, cell_max, IM_COL32(255, 255, 255, 255));
      else if (cell == MazeElement::EXPLORED)
        draw_list->AddRectFilled(cell_min, cell_max, IM_COL32(231, 158, 79, 255));

      draw_list->AddRect(cell_min, cell_max, IM_COL32(100, 100, 100, 255));
    }
  }
}

void MazeView::renderGUI()
{
  if (!stop_flag)
    deFramequeue();

  ImGui::Begin("Maze Generator and Solver");

  ImGui::BeginGroup();
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
              1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  ImGui::Checkbox("Stop", &stop_flag);
  if (ImGui::Button("Generate Maze (Prim's)")) controller_ptr->handleInput(MazeAction::G_PRIMS);
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
  // std::this_thread::sleep_for(std::chrono::nanoseconds(1));
  renderMaze();
  ImGui::EndChild();

  ImGui::End();
}