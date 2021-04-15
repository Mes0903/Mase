#ifndef MAKING_MAZE_H
#define MAKING_MAZE_H

#include "mainwindow.h"

class Making_Maze : public QObject {
    Q_OBJECT

  private:
    static Making_Maze* mk;
    Making_Maze();
    ~Making_Maze();

  public:
    static Making_Maze* getInstance();
    static inline bool if_in_wall( const int &y, const int &x, const int &delta_y, const int &delta_x );
    void random_prim_make_maze( const int &first_y, const int &first_x , MainWindow* w);
};

#endif // MAKING_MAZE_H
