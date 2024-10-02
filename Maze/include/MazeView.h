#ifndef MAZEVIEW_H
#define MAZEVIEW_H

/**
 * @file MazeView.h
 * @author Mes (mes900903@gmail.com)
 * @brief Display the maze
 * @version 0.1
 * @date 2024-09-22
 */

#include "MazeController.h"
#include "MazeNode.h"
#include "ThreadSafeQueue.h"

#include <memory>
#include <queue>
#include <utility>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include <stdio.h>
#include "glad/glad.h"
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>


class MazeController;
struct MazeNode;

class MazeView {
public:
  MazeView(uint32_t height, uint32_t width);
  void setController(MazeController *controller_ptr);

  void render(GLFWwindow*);
  void renderGUI();
  void setFrameMaze(const std::vector<std::vector<MazeElement>> &maze);
  void enFramequeue(const MazeNode &node);



private:
  std::vector<std::vector<MazeElement>> render_maze;
  MazeController* controller_ptr;
  ThreadSafeQueue<MazeNode> MazeDiffQueue;
  MazeNode update_node;
  bool stop_flag;

private:
  void deFramequeue();
  void renderMaze();
};

#endif