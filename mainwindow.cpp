#include "mainwindow.h"
#include "making_maze.h"
#include "maze_solving.h"
#include "mase.h"
#include "./ui_mainwindow.h"

MainWindow* MainWindow::w = nullptr;

MainWindow* MainWindow::getInstance(){
    if(w == nullptr)
        w = new MainWindow();

    return w;
}

void MainWindow::paintEvent( QPaintEvent * )  {
    QPainter painter( this );
    Mase* M = Mase::getInstance();
    for ( int i = 0; i < MAZE_HEIGHT; ++i ) {
        for ( int j = 0; j < MAZE_WIDTH; ++j ) {
            QThread::msleep(30);
            switch ( M->maze[i][j] ) {
                case 1:
                    this->repaint();
                    QThread::msleep(30);
                    painter.fillRect( j * GRID_SIZE, i * GRID_SIZE, GRID_SIZE, GRID_SIZE, QColor( qRgb( 115, 64, 70 ) ) );
                    break;
                case 2:
                    this->repaint();
                    QThread::msleep(30);
                    painter.fillRect( j * GRID_SIZE, i * GRID_SIZE, GRID_SIZE, GRID_SIZE, QColor( qRgb( 231, 158, 79 ) ) );
                    break;
            }
            painter.drawRect( j * GRID_SIZE, i * GRID_SIZE, GRID_SIZE, GRID_SIZE );
        }
    }
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->setWindowTitle("Maze");
    this->resize(QSize(GRID_SIZE*MAZE_WIDTH, GRID_SIZE*MAZE_HEIGHT));
    Mase *M = Mase::getInstance();
    Making_Maze *mk = Making_Maze::getInstance();
    Maze_Solving *slv = Maze_Solving::getInstance();

    //search
    QAction* DFS = new QAction(QIcon(("C:/image/ina.gif")), "DFS", this);
    DFS->setStatusTip(tr("DFS Search"));
    QObject::connect(DFS, &QAction::triggered, [&M](){M->reset();});
    QObject::connect(DFS, &QAction::triggered, [&slv](){slv->dfs({1,0});});
    QObject::connect(DFS, &QAction::triggered, [this](){this->repaint();});

    QAction* BFS = new QAction(QIcon(("C:/image/ina.gif")), "BFS", this);
    DFS->setStatusTip(tr("BFS Search"));
    QObject::connect(BFS, &QAction::triggered, [&M](){M->reset();});
    QObject::connect(BFS, &QAction::triggered, [&slv](){slv->bfs(1, 0);});
    QObject::connect(BFS, &QAction::triggered, [this](){this->repaint();});

    QAction* UCS = new QAction(QIcon(("C:/image/ina.gif")), "UCS", this);
    DFS->setStatusTip(tr("UCS Search"));
    QObject::connect(UCS, &QAction::triggered, [&M](){M->reset();});
    QObject::connect(UCS, &QAction::triggered, [&slv](){slv->ucs( 1, 0);});
    QObject::connect(UCS, &QAction::triggered, [this](){this->repaint();});

    //making map
    QAction* Rm_Prim_Map = new QAction(QIcon(("C:/image/ina.gif")), "Rm_Prim_Map", this);
    DFS->setStatusTip(tr("Rm_Prim_Map Map Making"));
    QObject::connect(Rm_Prim_Map, &QAction::triggered, [&M](){M->reset();});
    QObject::connect(Rm_Prim_Map, &QAction::triggered, [&mk](){mk->random_prim_make_maze(1, 0);});
    QObject::connect(Rm_Prim_Map, &QAction::triggered, [this](){this->repaint();});

    QMenu *SLV_Menu;
    SLV_Menu = this->menuBar()->addMenu("&Searching");
    SLV_Menu->addAction(DFS);
    SLV_Menu->addAction(BFS);
    SLV_Menu->addAction(UCS);

    QMenu *MK_Menu;
    MK_Menu = this->menuBar()->addMenu("&Map Making");
    MK_Menu->addAction(Rm_Prim_Map);

    //QComboBox box( this );
    //box.show();
    //box.addItem( "DFS" );
    //box.addItem( "BFS" );
    //box.addItem( "UCS" );
    //box.addItem( "Random Prim Making" );
    //
    //QPushButton *runButton = this->findChild<QPushButton *>( "runButton" );
    //QObject::connect( runButton, &QPushButton::clicked, [&box, this, &mk, &slv]() {
    //    this->reset();
    //    if ( box.currentText() == "BFS" )
    //        slv->bfs( 1, 0 , this);
    //    else if ( box.currentText() == "UCS" )
    //        slv->ucs( 1, 0 , this);
    //    else if ( box.currentText() == "DFS" )
    //        slv->dfs( { 1, 0 } , this);
    //    else if (box.currentText() == "Random Prim Making")
    //        mk->random_prim_make_maze(1, 0, this);
    //    this->repaint();
    //} );
}

MainWindow::~MainWindow() {
    delete ui;
    delete w;
    w = nullptr;
}


