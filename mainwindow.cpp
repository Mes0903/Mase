#include "mainwindow.h"
#include "making_maze.h"
#include "maze_solving.h"
#include "maze.h"
#include "./ui_mainwindow.h"


#include <QMainWindow>
#include <QPainter>
#include <QAction>


void MainWindow::paintEvent( QPaintEvent * )  {
    QPainter painter( this );
    for ( int i = 0; i < MAZE_HEIGHT; ++i ) {
        for ( int j = 0; j < MAZE_WIDTH; ++j ) {
            switch ( M->maze[i][j] ) {
                case 1:
                    painter.fillRect( j * GRID_SIZE, i * GRID_SIZE, GRID_SIZE, GRID_SIZE, QColor( qRgb( 115, 64, 70 ) ) );
                    break;
                case 2:
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

    M = new Maze();
    mk = new Making_Maze();
    slv = new Maze_Solving();

    //search
    DFS = new QAction(QIcon(("C:/image/ina.gif")), "DFS", this);
    DFS->setStatusTip(tr("DFS Search"));
    QObject::connect(DFS, &QAction::triggered, [this](){M->reset();});
    QObject::connect(DFS, &QAction::triggered, [this](){slv->dfs({1,0}, this );});
    QObject::connect(DFS, &QAction::triggered, [this](){this->repaint();});

    BFS = new QAction(QIcon(("C:/image/ina.gif")), "BFS", this);
    BFS->setStatusTip(tr("BFS Search"));
    QObject::connect(BFS, &QAction::triggered, [this](){M->reset();});
    QObject::connect(BFS, &QAction::triggered, [this](){slv->bfs(1, 0, this );});
    QObject::connect(BFS, &QAction::triggered, [this](){this->repaint();});

    UCS = new QAction(QIcon(("C:/image/ina.gif")), "UCS", this);
    UCS->setStatusTip(tr("UCS Search"));
    QObject::connect(UCS, &QAction::triggered, [this](){M->reset();});
    QObject::connect(UCS, &QAction::triggered, [this](){slv->ucs( 1, 0, this );});
    QObject::connect(UCS, &QAction::triggered, [this](){this->repaint();});

    GREEDY = new QAction(QIcon(("C:/image/ina.gif")), "GREEDY", this);
    GREEDY->setStatusTip(tr("UCS Search"));
    QObject::connect(GREEDY, &QAction::triggered, [this](){M->reset();});
    QObject::connect(GREEDY, &QAction::triggered, [this](){slv->greedy( 1, 0, this );});
    QObject::connect(GREEDY, &QAction::triggered, [this](){this->repaint();});

    //making map
    Rm_Prim_Map = new QAction(QIcon(("C:/image/ina.gif")), "Rm_Prim_Map", this);
    DFS->setStatusTip(tr("Rm_Prim_Map Map Making"));
    QObject::connect(Rm_Prim_Map, &QAction::triggered, [this](){M->reset();});
    QObject::connect(Rm_Prim_Map, &QAction::triggered, [this](){mk->random_prim_make_maze( 1, 0, this );});
    QObject::connect(Rm_Prim_Map, &QAction::triggered, [this](){this->repaint();});

    //Search Menu
    SLV_Menu = this->menuBar()->addMenu("&Searching");
    SLV_Menu->addAction(DFS);
    SLV_Menu->addAction(BFS);
    SLV_Menu->addAction(UCS);
    SLV_Menu->addAction(GREEDY);


    //Making Map Menu
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
}


