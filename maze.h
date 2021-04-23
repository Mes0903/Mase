#ifndef MAZE_H
#define MAZE_H

#include <utility>

inline constexpr int MAZE_HEIGHT = 37;
inline constexpr int MAZE_WIDTH = 37;
inline constexpr int BEGIN_Y = 1;
inline constexpr int BEGIN_X = 0;
inline constexpr int END_Y = MAZE_HEIGHT - 2;
inline constexpr int END_X = MAZE_WIDTH - 1;
inline constexpr int GRID_SIZE = 25;
inline constexpr std::pair<int, int> directions[4]{ { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
inline constexpr std::pair<int, int> GROUND_Direction[4]{ { 2, 0 }, { 0, 2 }, { -2, 0 }, { 0, -2 } };

class Animator;

class Maze {

  public:
    Maze(Animator *A);
    ~Maze();
    void reset();
    void reset_wall_around_maze();
    static inline int maze[MAZE_HEIGHT][MAZE_WIDTH]{ { 0 } };
    static inline std::pair<int, int> Buffer_Node{-1,-1};
    Animator* animator;

  protected:
    enum class Maze_Elements : int {
        GROUND = 0,
        WALL = 1,
        EXPLORED = 2,
        BEGIN = 9,
        END = 10,
    };
};

#endif    // MAZE_H
