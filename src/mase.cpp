#include "MazeModel.h"
#include "MazeView.h"
#include "MazeController.h"

int main(int, char **)
{
  MazeModel model(MAZE_HEIGHT, MAZE_WIDTH);
  MazeView view(MAZE_HEIGHT, MAZE_WIDTH);
  MazeController controller;

  controller.setModelView(&model, &view);

  view.render();
  return 0;
}
