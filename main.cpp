#include "mainwindow.h"
#include "making_maze.h"
#include "maze_solving.h"

#include <QApplication>


int main( int argc, char *argv[] ) {
    QApplication a( argc, argv );

    MainWindow* w = new MainWindow();
    w->show();

    return a.exec();
}
