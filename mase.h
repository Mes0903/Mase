#ifndef MASE_H
#define MASE_H

#include "mainwindow.h"

inline constexpr int MAZE_HEIGHT = 35;
inline constexpr int MAZE_WIDTH = 35;
inline constexpr int END_Y = MAZE_HEIGHT - 2;
inline constexpr int END_X = MAZE_WIDTH - 1;
inline constexpr int GRID_SIZE = 25;
inline constexpr std::pair<int, int> directions[4]{ { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };


class Mase {

  private:
    static Mase* M;

  protected:
    ~Mase();
    Mase();

  public:
    void reset();
    static Mase* getInstance();
    static inline int maze[MAZE_HEIGHT][MAZE_WIDTH]{ { 0 } };

};

#endif // MASE_H
