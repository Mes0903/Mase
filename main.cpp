#include "mainwindow.h"
#include "making_maze.h"
#include "maze_solving.h"

int main( int argc, char *argv[] ) {
    QApplication a( argc, argv );

    MainWindow* w = MainWindow::getInstance();
    w->show();

    return a.exec();
}
