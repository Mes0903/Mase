#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include "glad/glad.h"
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>

#include "MazeController.h"
#include "MazeModel.h"
#include "MazeView.h"


MazeView::MazeView(uint32_t height, uint32_t width)
    : render_maze__{ height, std::vector<MazeElement>{ width, MazeElement::GROUND } }, update_node__{ MazeNode{ -1, -1, MazeElement::INVALID } }, stop_flag__{ false }, grid_flag__{ true }, renderer__{} {}

void MazeView::setController(MazeController *controller_ptr__)
{
  this->controller_ptr__ = controller_ptr__;
}

void MazeView::resetUpdateNode()
{
  update_node__ = MazeNode{ -1, -1, MazeElement::INVALID };
}

void MazeView::enFramequeue(const std::vector<std::vector<MazeElement>> &maze, const MazeNode &node)
{
  maze_queue__.enqueue(maze);
  update_node_queue__.enqueue(node);
}

void MazeView::deFramequeue__()
{
  std::optional<decltype(render_maze__)> opt_maze = maze_queue__.dequeue();
  std::optional<MazeNode> opt_node = update_node_queue__.dequeue();
  if (opt_maze.has_value()) {
    std::lock_guard<std::mutex> lock(maze_mutex__);
    render_maze__ = std::move(*opt_maze);

    if (opt_node.has_value())
      update_node__ = std::move(*opt_node);
  }
  else if (controller_ptr__->isModelComplete()) {
    controller_ptr__->setViewComplete();
  }
}

void MazeView::renderMaze__()
{
  ImDrawList *draw_list = ImGui::GetWindowDrawList();
  const ImVec2 p = ImGui::GetCursorScreenPos();
  const float cell_size = GRID_SIZE;

  std::lock_guard<std::mutex> lock(maze_mutex__);
  for (int32_t y = 0; y < MAZE_HEIGHT; ++y) {
    for (int32_t x = 0; x < MAZE_WIDTH; ++x) {
      MazeElement cell = render_maze__[y][x];
      ImVec2 cell_min = ImVec2(p.x + x * cell_size, p.y + y * cell_size);
      ImVec2 cell_max = ImVec2(cell_min.x + cell_size, cell_min.y + cell_size);

      if (update_node__.y == y && update_node__.x == x)
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
      else if (cell == MazeElement::ANSWER)
        draw_list->AddRectFilled(cell_min, cell_max, IM_COL32(170, 100, 255, 255));

      if (grid_flag__)
        draw_list->AddRect(cell_min, cell_max, IM_COL32(100, 100, 100, 255));
    }
  }
}

void MazeView::drawGUI__()
{
  if (!stop_flag__)
    deFramequeue__();

  ImGui::Begin("Maze Generator and Solver");

  ImGui::BeginGroup();
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
              1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

  ImGui::Checkbox("Grid", &grid_flag__);
  ImGui::Checkbox("Stop", &stop_flag__);
  if (ImGui::Button("Clean All")) controller_ptr__->handleInput(MazeAction::G_CLEANALL);
  if (ImGui::Button("Clean Explored")) controller_ptr__->handleInput(MazeAction::G_CLEAN_EXPLORED);
  if (ImGui::Button("Generate Maze (Prim's)")) controller_ptr__->handleInput(MazeAction::G_PRIM);
  if (ImGui::Button("Generate Maze (Prim's) and Break some walls")) controller_ptr__->handleInput(MazeAction::G_PRIM_BREAK);
  if (ImGui::Button("Generate Maze (Recursion Backtracker)")) controller_ptr__->handleInput(MazeAction::G_RECURSION_BACKTRACKER);
  if (ImGui::Button("Generate Maze (Recursion Division)")) controller_ptr__->handleInput(MazeAction::G_RECURSION_DIVISION);
  if (ImGui::Button("Solve Maze (DFS)")) controller_ptr__->handleInput(MazeAction::S_DFS);
  if (ImGui::Button("Solve Maze (BFS)")) controller_ptr__->handleInput(MazeAction::S_BFS);
  if (ImGui::Button("Solve Maze (Manhattan UCS)")) controller_ptr__->handleInput(MazeAction::S_UCS_MANHATTAN);
  if (ImGui::Button("Solve Maze (Two Norm UCS)")) controller_ptr__->handleInput(MazeAction::S_UCS_TWO_NORM);
  if (ImGui::Button("Solve Maze (Manhattan Greedy)")) controller_ptr__->handleInput(MazeAction::S_GREEDY_MANHATTAN);
  if (ImGui::Button("Solve Maze (Two Norm Greedy)")) controller_ptr__->handleInput(MazeAction::S_GREEDY_TWO_NORM);
  if (ImGui::Button("Solve Maze (A*)")) controller_ptr__->handleInput(MazeAction::S_ASTAR_MANHATTAN);
  if (ImGui::Button("Solve Maze (A* Interval)")) controller_ptr__->handleInput(MazeAction::S_ASTAR_INTERVAL);
  ImGui::EndGroup();

  ImGui::SameLine();
  ImGui::BeginChild("MazeView", ImVec2(0, 0), true);
  // std::this_thread::sleep_for(std::chrono::nanoseconds(1));
  renderMaze__();
  ImGui::EndChild();

  ImGui::End();
}

void MazeView::render()
{
  while (!glfwWindowShouldClose(renderer__.window__)) {
    renderer__.setNewFrame__();
    drawGUI__();
    renderer__.render();
  }
}