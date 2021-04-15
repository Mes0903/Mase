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

#include "mase.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

  private:
    Ui::MainWindow *ui;
    static MainWindow* w;
    //Mase* M;

    MainWindow(QWidget *parent = nullptr);
    void paintEvent( QPaintEvent * ) override;

  public:
    static MainWindow* getInstance();
    ~MainWindow();
    void reset();


};

#endif // MAINWINDOW_H
