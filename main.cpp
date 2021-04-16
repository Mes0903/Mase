#include "mainwindow.h"

#include <QApplication>

int main( int argc, char *argv[] ) {
    QApplication a( argc, argv );

    MainWindow *W = new MainWindow();
    W->show();

    return a.exec();
}
