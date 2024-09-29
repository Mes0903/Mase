#ifndef MAZEVIEW_H
#define MAZEVIEW_H

/**
 * @file MazeView.h
 * @author Mes (mes900903@gmail.com)
 * @brief Display the maze
 * @version 0.1
 * @date 2024-09-22
 */

#include <memory>
#include <queue>
#include <utility>

class MazeController;

class MazeView {
public:
  MazeView(uint32_t height, uint32_t width);
  void setController(MazeController *controller_ptr);

  void renderGUI();
  void setFrameMaze(const std::vector<std::vector<MazeElement>> &maze);
  void enFramequeue(const int y, const int x, const MazeElement element);

private:
  std::vector<std::vector<MazeElement>> render_maze;
  std::unique_ptr<MazeController> controller_ptr;
  std::queue<std::tuple<int, int, MazeElement>> frame_queue;
  std::tuple<int, int, MazeElement> update_pos;
  bool stop_flag;

private:
  void deFramequeue();
  void renderMaze();
};

#endif