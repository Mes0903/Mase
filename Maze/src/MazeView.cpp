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
#include "MazeView.h"

MazeView::MazeView(uint32_t height, uint32_t width)
    : render_maze__{ height, std::vector<MazeElement>{ width, MazeElement::GROUND } },
      update_node__{ MazeNode{ -1, -1, MazeElement::INVALID } },
      stop_flag__{ false },
      grid_flag__{ true },
      limit_fps__{ false },
      fps__{ 240 },
      frame__{ std::chrono::steady_clock::now() },
      cell_size__{ 15 },
      renderer__{}
{
  render_maze__[BEGIN_Y][BEGIN_X] = MazeElement::BEGIN;
  render_maze__[END_Y][END_X] = MazeElement::END;
}

void MazeView::setController(MazeController *controller_ptr__)
{
  this->controller_ptr__ = controller_ptr__;
}

void MazeView::resetUpdateNode()
{
  update_node__ = MazeNode{ -1, -1, MazeElement::INVALID };
}

/**
 * @brief Enqueue the maze and the node to the view.
 */
void MazeView::enFramequeue(const std::vector<std::vector<MazeElement>> &maze, const MazeNode &node)
{
  maze_queue__.enqueue(maze);
  update_node_queue__.enqueue(node);
}

/**
 * @brief Dequeue the maze and the node from the view.
 */
void MazeView::deFramequeue__()
{
  std::optional<decltype(render_maze__)> opt_maze = maze_queue__.dequeue();
  std::optional<MazeNode> opt_node = update_node_queue__.dequeue();
  if (opt_maze.has_value()) {
    std::lock_guard<std::mutex> lock(maze_mutex__);    // lock the maze
    render_maze__ = std::move(*opt_maze);

    if (opt_node.has_value())
      update_node__ = std::move(*opt_node);
  }
  else if (controller_ptr__->isModelComplete()) {
    controller_ptr__->setViewComplete();    // if the model is complete, and the queue is empty, set the view complete
  }
}

/**
 * @brief The main logic to render the maze.
 */
void MazeView::renderMaze__()
{
  ImDrawList *draw_list = ImGui::GetWindowDrawList();
  const ImVec2 p = ImGui::GetCursorScreenPos();

  std::lock_guard<std::mutex> lock(maze_mutex__);    // lock the maze
  for (int32_t y = 0; y < MAZE_HEIGHT; ++y) {
    for (int32_t x = 0; x < MAZE_WIDTH; ++x) {
      MazeElement cell = render_maze__[y][x];
      ImVec2 cell_min = ImVec2(p.x + x * cell_size__, p.y + y * cell_size__);
      ImVec2 cell_max = ImVec2(cell_min.x + cell_size__, cell_min.y + cell_size__);

      // draw the cell of the maze in corresponding color
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

      // draw the grid line
      if (grid_flag__)
        draw_list->AddRect(cell_min, cell_max, IM_COL32(100, 100, 100, 255));
    }
  }
}

/**
 * @brief Draw the main GUI, including the maze and the controls panel, the maze part is hosted by the renderMaze__() function.
 */
void MazeView::drawGUI__()
{
  if (!stop_flag__)
    deFramequeue__();

  ImGui::Begin("Maze Generator and Solver");
  {
    // Calculate the width of the FPS text
    const char *fps_text = "Application average 000.000 ms/frame (000.0 FPS)";
    ImVec2 text_size = ImGui::CalcTextSize(fps_text);
    float column_width = text_size.x + ImGui::GetStyle().FramePadding.x * 2;

    // Left column for controls
    ImGui::BeginChild("ControlsColumn", ImVec2(column_width, cell_size__ * (MAZE_HEIGHT + 1)), true);
    {
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                  1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

      static uint32_t cell_buffer = cell_size__;
      ImGui::SetNextItemWidth(40);
      if (ImGui::InputScalar("Cell Size", ImGuiDataType_U32, &cell_buffer, nullptr, nullptr, nullptr, ImGuiInputTextFlags_EnterReturnsTrue))
        cell_size__ = cell_buffer;
      if (ImGui::Checkbox("Limit FPS", &limit_fps__)) frame__ = std::chrono::steady_clock::now();

      static uint32_t fps_buffer = fps__;
      ImGui::SameLine();
      ImGui::SetNextItemWidth(40);
      if (ImGui::InputScalar("FPS", ImGuiDataType_U32, &fps_buffer, nullptr, nullptr, nullptr, ImGuiInputTextFlags_EnterReturnsTrue)) {
        fps__ = fps_buffer;
        frame__ = std::chrono::steady_clock::now();
        limit_fps__ = true;
      }

      ImGui::Checkbox("Grid", &grid_flag__);
      ImGui::Checkbox("Stop", &stop_flag__);

      ImGui::Dummy(ImVec2(0.0f, 3.0f));
      ImGui::Separator();
      ImGui::Dummy(ImVec2(0.0f, 3.0f));

      ImGui::Text("Spend Cost: %d", controller_ptr__->getSolveCost());
      ImGui::Text("Spend Cell: %d", controller_ptr__->getSolveCell());

      ImGui::Dummy(ImVec2(0.0f, 3.0f));
      ImGui::Separator();
      ImGui::Dummy(ImVec2(0.0f, 3.0f));

      ImGui::Text("Generate Maze");
      if (ImGui::Button("Clean All")) controller_ptr__->handleAction(MazeAction::G_CLEANALL);
      if (ImGui::Button("Randomized Prim's")) controller_ptr__->handleAction(MazeAction::G_PRIM);
      if (ImGui::Button("Randomized Prim's and Break some walls")) controller_ptr__->handleAction(MazeAction::G_PRIM_BREAK);
      if (ImGui::Button("Recursion Backtracker")) controller_ptr__->handleAction(MazeAction::G_RECURSION_BACKTRACKER);
      if (ImGui::Button("Recursion Division")) controller_ptr__->handleAction(MazeAction::G_RECURSION_DIVISION);

      ImGui::Dummy(ImVec2(0.0f, 3.0f));
      ImGui::Separator();
      ImGui::Dummy(ImVec2(0.0f, 3.0f));

      ImGui::Text("Solve Maze");
      if (ImGui::Button("DFS")) controller_ptr__->handleAction(MazeAction::S_DFS);
      if (ImGui::Button("BFS")) controller_ptr__->handleAction(MazeAction::S_BFS);
      if (ImGui::Button("UCS")) controller_ptr__->handleAction(MazeAction::S_UCS);
      if (ImGui::Button("Manhattan Greedy")) controller_ptr__->handleAction(MazeAction::S_GREEDY_MANHATTAN);
      if (ImGui::Button("Two Norm Greedy")) controller_ptr__->handleAction(MazeAction::S_GREEDY_TWO_NORM);
      if (ImGui::Button("Manhattan A*")) controller_ptr__->handleAction(MazeAction::S_ASTAR_MANHATTAN);
      if (ImGui::Button("Two Norm A*")) controller_ptr__->handleAction(MazeAction::S_ASTAR_TWO_NORM);
    }
    ImGui::EndChild();

    ImGui::SameLine();
    ImGui::BeginChild("MazeView", ImVec2(cell_size__ * (MAZE_WIDTH + 1), cell_size__ * (MAZE_HEIGHT + 1)), true);
    {
      renderMaze__();
    }
    ImGui::EndChild();
  }
  ImGui::End();
}

/**
 * @brief The main loop to render the maze.
 */
void MazeView::render()
{
  while (!glfwWindowShouldClose(renderer__.window__)) {
    // use the time_point to limit the fps, come from https://stackoverflow.com/questions/38730273/how-to-limit-fps-in-a-loop-with-c
    if (limit_fps__)
      frame__ += std::chrono::milliseconds(1000 / fps__);

    renderer__.setNewFrame__();
    drawGUI__();
    renderer__.render__();

    if (limit_fps__)
      std::this_thread::sleep_until(frame__);
  }
}