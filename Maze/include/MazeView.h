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
#include "MazeRenderer.h"
#include "ThreadSafeQueue.h"

#include <mutex>

class MazeController;
struct MazeNode;

class MazeView {
public:
  MazeView(uint32_t height, uint32_t width);

  void setController(MazeController *controller_ptr__);

  void render();
  void resetUpdateNode();
  void enFramequeue(const std::vector<std::vector<MazeElement>> &node);

private:
  MazeRenderer renderer__;

  std::vector<std::vector<MazeElement>> render_maze__;
  MazeController *controller_ptr__;
  ThreadSafeQueue<decltype(render_maze__)> maze_queue__;
  MazeNode update_node__;
  bool stop_flag__;
  bool grid_flag__;
  std::mutex maze_mutex__;

private:
  void deFramequeue__();
  void renderMaze__();
  void drawGUI__();
};

#endif