#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QPainter>
#include <QTextStream>

const int MAZE_WIDTH = 10;
const int MAZE_HEIGHT = 5;

class PaintWindow : public MainWindow {
public:
    bool maze[MAZE_WIDTH][MAZE_HEIGHT] = { 0 };

    void paintEvent(QPaintEvent*) override
    {
        const int GRID_SIZE = 50;

        QPainter painter(this);
        for (int y = 0; y < MAZE_HEIGHT; ++y)
            for (int x = 0; x < MAZE_WIDTH; ++x)
                if (maze[y][x]) painter.fillRect(x * GRID_SIZE, y * GRID_SIZE, GRID_SIZE, GRID_SIZE, Qt::red);
                else painter.drawRect(x * GRID_SIZE, y * GRID_SIZE, GRID_SIZE, GRID_SIZE);
    }
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PaintWindow w;
    w.show();
    QFile file(QString(SOURCE_PATH) + "/test.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    for (int y = 0; y < MAZE_HEIGHT; ++y) {
        QString string = file.readLine();
        string = string.simplified().remove(' ');
        for (int x = 0; x < MAZE_WIDTH; ++x)
            w.maze[y][x] = string[x] == '0' ? false : true;
    }
    return a.exec();
}
