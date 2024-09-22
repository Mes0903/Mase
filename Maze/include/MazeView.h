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

class MazeController;

class MazeView {
public:
  void render();

private:
  std::unique_ptr<MazeController> controller;

  void renderMaze();
  void renderGUI();

  // Add other private methods as needed
};

#endif