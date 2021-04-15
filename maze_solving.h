#ifndef MAZE_SOLVING_H
#define MAZE_SOLVING_H

#include "mainwindow.h"



class Maze_Solving : public QObject {

    Q_OBJECT

  private:
    static Maze_Solving* solve;
    Maze_Solving();
    ~Maze_Solving();

  public:
    static Maze_Solving* getInstance();
    static inline bool is_in_maze( const int &y, const int &x );
    bool dfs( std::pair<int, int> position , MainWindow* w);
    void bfs( const int &first_y, const int &first_x , MainWindow* w);
    void ucs( const int &first_y, const int &first_x , MainWindow* w);
};

#endif // MAZE_SOLVING_H
