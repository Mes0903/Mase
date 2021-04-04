#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QPainter>
#include <QTextStream>
#include <QVector2D>

const int MAZE_WIDTH = 16;
const int MAZE_HEIGHT = 12;

class PaintWindow : public MainWindow {
private:
    QVector<int> directions[4] = { QVector<int>(1.0f, 0.0f), QVector<int>(0.0f, 1.0f), QVector<int>(-1.0f, 0.0f), QVector<int>(0.0f, -1.0f) };
public:
    int maze[MAZE_WIDTH][MAZE_HEIGHT] = { {0} };

    bool dfs(QVector<int> position)
    {
        maze[int(position[0])][int(position[1])] = 2;
        if (position == QVector<int>(15, 10)) return 1;
        for (const auto &dir : directions) {
            auto temp = position + dir;
            if (maze[int(temp[0])][int(temp[1])] == 0) {
                if (dfs(temp)) return 1;
            }
        }
        return 0;
    }

    void paintEvent(QPaintEvent*) override
    {
        const int GRID_SIZE = 50;

        QPainter painter(this);
        for (int y = 0; y < MAZE_HEIGHT; ++y)
            for (int x = 0; x < MAZE_WIDTH; ++x) {
                switch (maze[x][y]) {
                case 1: painter.fillRect(x * GRID_SIZE, y * GRID_SIZE, GRID_SIZE, GRID_SIZE, Qt::red); break;
                case 2: painter.fillRect(x * GRID_SIZE, y * GRID_SIZE, GRID_SIZE, GRID_SIZE, Qt::green); break;
                }

                painter.drawRect(x * GRID_SIZE, y * GRID_SIZE, GRID_SIZE, GRID_SIZE);
            }
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
            w.maze[x][y] = string[x].digitValue();
    }
    w.dfs({0, 1});
    return a.exec();
}
