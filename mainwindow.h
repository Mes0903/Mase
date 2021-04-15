#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "making_maze.h"
#include "maze_solving.h"
#include "maze.h"


#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Maze;
class Making_Maze;
class Maze_Solving;

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private:
    Ui::MainWindow *ui;
    void paintEvent( QPaintEvent * ) override;

    Maze *M;
    Making_Maze *mk;
    Maze_Solving *slv;
    QAction* DFS;
    QAction* BFS;
    QAction* UCS;
    QAction* GREEDY;
    QAction* Rm_Prim_Map;
    QMenu *SLV_Menu;
    QMenu *MK_Menu;
};

#endif // MAINWINDOW_H
