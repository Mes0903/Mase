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

#include <memory>
#include <queue>
#include <utility>

class MazeController;
struct MazeNode;

class MazeView {
public:
  MazeView(uint32_t height, uint32_t width);
  void setController(MazeController *controller_ptr);

  void renderGUI();
  void setFrameMaze(const std::vector<std::vector<MazeElement>> &maze);
  void enFramequeue(const MazeNode &node);

private:
  std::vector<std::vector<MazeElement>> render_maze;
  std::unique_ptr<MazeController> controller_ptr;
  std::queue<MazeNode> frame_queue;
  MazeNode update_node;
  bool stop_flag;

private:
  void deFramequeue();
  void renderMaze();
};

#endif