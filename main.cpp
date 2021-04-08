#include "mainwindow.h"

#include <QDebug>
#include <queue>
#include <QApplication>
#include <QFile>
#include <QPainter>
#include <QTextStream>
#include <utility>
#include <iostream>
#include <QColor>
#include <QTimer>

constexpr int MAZE_HEIGHT = 12;
constexpr int MAZE_WIDTH = 16;

template <typename T1, typename T2>
std::pair<T1, T2> operator+( const std::pair<T1, T2> &p1, const std::pair<T1, T2> &p2 ) {
    return std::move( std::make_pair( p1.first + p2.first, p1.second + p2.second ) );
}

class PaintWindow : public MainWindow {
  private:
    const std::pair<int, int> directions[4]{ { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
  public:
    int maze[MAZE_HEIGHT][MAZE_WIDTH]{ { 0 } };

    bool dfs( std::pair<int, int> position ) {

        maze[position.first][position.second] = 2;
        if ( position.first == 10 && position.second == 15 )
            return true;
        for ( const auto &dir : directions ) {
            auto temp = position + dir;
            if (maze[temp.first][temp.second] == 0){
                if ( dfs( temp ) ) return true;
            }
        }
        return false;
    }

    void bfs(const int first_y, const int first_x){
        bool flag{false}; //判斷找到終點了沒
        int x{}, y{};
        std::queue<std::pair<int,int>> result;
        result.push(std::make_pair(first_y,first_x));
        maze[first_y][first_x] = 2;
        std::pair<int,int> temp;

        while(!flag){
            temp = result.front();
            result.pop();
            for(const auto &dir : directions){
                y = temp.first + dir.first;
                x = temp.second + dir.second;

                if( y < MAZE_HEIGHT && x < MAZE_WIDTH){
                    if (y == 10 && x == 15){
                        maze[y][x] = 2;
                        flag = true;
                        break;
                    }
                    else if(maze[y][x] == 0){
                        result.push(std::make_pair(y, x));
                        maze[y][x] = 2;
                    }
                }
            }
        }
    }

    void paintEvent(QPaintEvent*) override {
        const int GRID_SIZE = 50;
        QPainter painter(this);
        for (int i = 0; i < MAZE_HEIGHT; ++i) {
            for (int j = 0; j < MAZE_WIDTH; ++j) {
                switch (maze[i][j]) {
                    case 1:
                        painter.fillRect(j * GRID_SIZE, i * GRID_SIZE, GRID_SIZE, GRID_SIZE, QColor(qRgb(115,64,70)));
                        break;
                    case 2:
                        painter.fillRect(j * GRID_SIZE, i * GRID_SIZE, GRID_SIZE, GRID_SIZE, QColor(qRgb(231,158,79)));
                        break;
                }
                painter.drawRect(j * GRID_SIZE, i * GRID_SIZE, GRID_SIZE, GRID_SIZE);
            }
        }
    }
};

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    PaintWindow w;
    w.show();

    QFile file(QString(SOURCE_PATH) + "/test.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString buff_line{};

    for (int i = 0; i < MAZE_HEIGHT; ++i) {
        buff_line = file.readLine();
        buff_line = buff_line.simplified().remove(' ');
        for (int j = 0; j < MAZE_WIDTH; ++j){
            w.maze[i][j] = buff_line[j].digitValue();
        }
    }

    //w.dfs({1, 0});
    w.bfs(1,0);
    return a.exec();
}
