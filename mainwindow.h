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
class Making_Maze;
class Maze_Solving;
class Animator;

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    static MainWindow *getInstance();
    ~MainWindow();
    friend Animator;

  private:
    MainWindow( QWidget *parent = nullptr );
    Ui::MainWindow *ui;
    static MainWindow *W;

    void paintEvent( QPaintEvent * ) override;

    Maze *M;
    Making_Maze *mk;
    Maze_Solving *slv;
    Animator *animator;
    QAction *DFS;
    QAction *BFS;
    QAction *UCS;
    QAction *GREEDY;
    QAction *Rm_Prim_Map;
    QMenu *SLV_Menu;
    QMenu *MK_Menu;
};

#endif    // MAINWINDOW_H
