#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class Maze;
class Maze_Making;
class Maze_Solving;
class Animator;

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow( QWidget *parent = nullptr );
    ~MainWindow();
    friend Animator;

  private:
    Ui::MainWindow *ui;
    static MainWindow *W;

    void paintEvent( QPaintEvent * ) override;

    Maze *M;
    Maze_Making *mk;
    Maze_Solving *slv;
    Animator *animator;

    QMenu *SLV_Menu;
    QAction *DFS;
    QAction *BFS;
    QAction *Manhattan_Distance_UCS;
    QAction *Two_Norm_UCS;
    QAction *Interval_UCS;
    QAction *GREEDY;
    QAction *Normal_A_STAR;
    QAction *Interval_A_STAR;

    QMenu *MK_Menu;
    QAction *Empty_Map;
    QAction *Rm_Prim_Map;
    QAction *Recursive_Map;
};

#endif    // MAINWINDOW_H
