#ifndef MAZE_H
#define MAZE_H


#include <utility>


inline constexpr int MAZE_HEIGHT = 35;
inline constexpr int MAZE_WIDTH = 35;
inline constexpr int END_Y = MAZE_HEIGHT - 2;
inline constexpr int END_X = MAZE_WIDTH - 1;
inline constexpr int GRID_SIZE = 25;
inline constexpr std::pair<int, int> directions[4]{ { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };


class Maze {

  public:
    void reset();
    static inline int maze[MAZE_HEIGHT][MAZE_WIDTH]{ { 0 } };
    Maze();
    ~Maze();
};

#endif // MAZE_H
