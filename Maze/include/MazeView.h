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

#include "MazeRenderer.h"
#include "ThreadSafeQueue.h"
#include "MazeDefine.h"

#include <mutex>

class MazeController;

class MazeView {
public:
  MazeView(uint32_t height, uint32_t width);

  void setController(MazeController *controller_ptr__);

  void render();
  void resetUpdateNode();
  void enFramequeue(const std::vector<std::vector<MazeElement>> &maze, const MazeNode &node);

private:
  MazeRenderer renderer__;

  std::vector<std::vector<MazeElement>> render_maze__;
  MazeController *controller_ptr__;
  ThreadSafeQueue<decltype(render_maze__)> maze_queue__;
  ThreadSafeQueue<MazeNode> update_node_queue__;
  MazeNode update_node__;
  bool stop_flag__;
  bool grid_flag__;
  bool limit_fps__;
  uint32_t fps__;
  uint32_t cell_size__;
  std::chrono::steady_clock::time_point frame__;
  std::mutex maze_mutex__;

private:
  void deFramequeue__();
  void renderMaze__();
  void drawGUI__();
};

#endif