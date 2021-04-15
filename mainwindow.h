#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QColor>
#include <QComboBox>
#include <QDebug>
#include <QFile>
#include <QPainter>
#include <QPushButton>
#include <QTextStream>
#include <QThread>
#include <QTimer>
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <queue>
#include <random>
#include <stack>
#include <utility>
#include <vector>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

inline constexpr int MAZE_HEIGHT = 35;
inline constexpr int MAZE_WIDTH = 35;
inline constexpr int END_Y = MAZE_HEIGHT - 2;
inline constexpr int END_X = MAZE_WIDTH - 1;
inline constexpr int GRID_SIZE = 25;
inline constexpr std::pair<int, int> directions[4]{ { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };


class MainWindow : public QMainWindow {
    Q_OBJECT

  private:
    Ui::MainWindow *ui;
    static MainWindow* w;

    MainWindow(QWidget *parent = nullptr);
    void paintEvent( QPaintEvent * ) override;

  public:
    static inline int maze[MAZE_HEIGHT][MAZE_WIDTH]{ { 0 } };

    static MainWindow* getInstance();
    ~MainWindow();
    void reset();


};

#endif // MAINWINDOW_H
