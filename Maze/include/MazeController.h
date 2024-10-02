#ifndef MAZECONTROLLER_H
#define MAZECONTROLLER_H

/**
 * @file MazeController.h
 * @author Mes
 * @brief The controller of the maze, accepts and processes the user's action.
 * @version 0.1
 * @date 2024-09-22
 */

#include "MazeModel.h"
#include "MazeView.h"
#include "MazeNode.h"

#include <memory>
#include <atomic>

class MazeModel;
class MazeView;
struct MazeNode;
enum class MazeAction;

class MazeController{
public:
  void setModelView(MazeModel* model_ptr, MazeView* view_ptr);

  void handleInput(const MazeAction action);
  void setFrameMaze(const std::vector<std::vector<MazeElement>>& maze);
  void enFramequeue(const MazeNode& node);

  void setModelComplete(); 
  bool isModelComplete() const; 

  void InitMaze();

public:
  std::atomic<bool> model_complete_flag{false};


private:
  MazeModel* model_ptr;  
  MazeView* view_ptr;
};

#endif
