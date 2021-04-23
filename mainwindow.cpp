#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "animator.h"
#include "maze.h"
#include "maze_making.h"
#include "maze_solving.h"

#include <QAction>
#include <QMainWindow>
#include <QPainter>

#include <iostream>

void MainWindow::paintEvent( QPaintEvent * ) {
    QPainter painter( this );
    for ( int y = 0; y < MAZE_HEIGHT; ++y ) {
        for ( int x = 0; x < MAZE_WIDTH; ++x ) {
            switch ( M->maze[y][x] ) {
                case 0:    //GROUND
                    painter.fillRect( x * GRID_SIZE, y * GRID_SIZE + 22.5, GRID_SIZE, GRID_SIZE, QColor( qRgb( 255, 255, 255 ) ) );
                    break;
                case 1:    //WALL
                    painter.fillRect( x * GRID_SIZE, y * GRID_SIZE + 22.5, GRID_SIZE, GRID_SIZE, QColor( qRgb( 115, 64, 70 ) ) );
                    break;
                case 2:    //EXPLORED
                    painter.fillRect( x * GRID_SIZE, y * GRID_SIZE + 22.5, GRID_SIZE, GRID_SIZE, QColor( qRgb( 231, 158, 79 ) ) );
                    break;
                case 9:    //BEGIN
                    painter.fillRect( x * GRID_SIZE, y * GRID_SIZE + 22.5, GRID_SIZE, GRID_SIZE, QColor( qRgb( 35, 220, 130 ) ) );
                    break;
                case 10:    //END
                    painter.fillRect( x * GRID_SIZE, y * GRID_SIZE + 22.5, GRID_SIZE, GRID_SIZE, QColor( qRgb( 250, 50, 150 ) ) );
                    break;
            }
            painter.fillRect( M->Buffer_Node.second * GRID_SIZE + 1, M->Buffer_Node.first * GRID_SIZE + 22.5 + 1, GRID_SIZE - 1, GRID_SIZE - 1, QColor( qRgb( 50, 215, 250 ) ) );
            painter.drawRect( x * GRID_SIZE, y * GRID_SIZE + 22.5, GRID_SIZE, GRID_SIZE );
        }
    }
}    // MainWindow::paintEvent

MainWindow::MainWindow( QWidget *parent ) : QMainWindow( parent ), ui( new Ui::MainWindow ) {
    ui->setupUi( this );
    this->setWindowTitle( "Mase" );
    this->resize( QSize( GRID_SIZE * MAZE_WIDTH, GRID_SIZE * MAZE_HEIGHT + 40 ) );

    animator = new Animator( this );
    M = new Maze( animator );
    mk = new Maze_Making( animator );
    slv = new Maze_Solving( animator );

    //search
    DFS = new QAction( QIcon( ( "C:/image/ina.gif" ) ), "DFS", this );
    DFS->setStatusTip( "DFS Search" );
    QObject::connect( DFS, &QAction::triggered, [this]() { M->reset(); } );
    QObject::connect( DFS, &QAction::triggered, [this]() { slv->dfs( 1, 0 ); } );

    BFS = new QAction( QIcon( ( "C:/image/ina.gif" ) ), "BFS", this );
    BFS->setStatusTip( "BFS Search" );
    QObject::connect( BFS, &QAction::triggered, [this]() { M->reset(); } );
    QObject::connect( BFS, &QAction::triggered, [this]() { slv->bfs(); } );

    Manhattan_Distance_UCS = new QAction( QIcon( ( "C:/image/ina.gif" ) ), "Manhattan_Distance_UCS", this );
    Manhattan_Distance_UCS->setStatusTip( "UCS with Manhattan Distance cost function." );
    QObject::connect( Manhattan_Distance_UCS, &QAction::triggered, [this]() { M->reset(); } );
    QObject::connect( Manhattan_Distance_UCS, &QAction::triggered, [this]() { slv->ucs( 0 ); } );

    Pow_Two_Norm_UCS = new QAction( QIcon( ( "C:/image/ina.gif" ) ), "Pow_Two_Norm_UCS", this );
    Pow_Two_Norm_UCS->setStatusTip( "UCS with Two Norm of second power cost function." );
    QObject::connect( Pow_Two_Norm_UCS, &QAction::triggered, [this]() { M->reset(); } );
    QObject::connect( Pow_Two_Norm_UCS, &QAction::triggered, [this]() { slv->ucs( 1 ); } );

    Interval_UCS = new QAction( QIcon( ( "C:/image/ina.gif" ) ), "Interval_UCS", this );
    Interval_UCS->setStatusTip( "UCS with Interval cost function." );
    QObject::connect( Interval_UCS, &QAction::triggered, [this]() { M->reset(); } );
    QObject::connect( Interval_UCS, &QAction::triggered, [this]() { slv->ucs( 2 ); } );

    GREEDY = new QAction( QIcon( ( "C:/image/ina.gif" ) ), "GREEDY", this );
    GREEDY->setStatusTip( "GREEDY Search" );
    QObject::connect( GREEDY, &QAction::triggered, [this]() { M->reset(); } );
    QObject::connect( GREEDY, &QAction::triggered, [this]() { slv->greedy(); } );

    Normal_A_STAR = new QAction( QIcon( ( "C:/image/ina.gif" ) ), "Normal_A_STAR", this );
    Normal_A_STAR->setStatusTip( "A* with constant cost function 50 and heuristic function Manhattan Distance." );
    QObject::connect( Normal_A_STAR, &QAction::triggered, [this]() { M->reset(); } );
    QObject::connect( Normal_A_STAR, &QAction::triggered, [this]() { slv->a_star( 0 ); } );

    Interval_A_STAR = new QAction( QIcon( ( "C:/image/ina.gif" ) ), "Interval_A_STAR", this );
    Interval_A_STAR->setStatusTip( "A* with Interval cost function and Two Norm of second power heuristic function." );
    QObject::connect( Interval_A_STAR, &QAction::triggered, [this]() { M->reset(); } );
    QObject::connect( Interval_A_STAR, &QAction::triggered, [this]() { slv->a_star( 1 ); } );

    //making map
    Empty_Map = new QAction( QIcon( ( "C:/image/ina.gif" ) ), "Empty_Map", this );
    Empty_Map->setStatusTip( "Empty_Map Map Making" );
    QObject::connect( Empty_Map, &QAction::triggered, [this]() { M->reset(); } );
    QObject::connect( Empty_Map, &QAction::triggered, [this]() { mk->empty_map(); } );
    QObject::connect( Empty_Map, &QAction::triggered, [this]() { mk->set_Flag(); } );

    Rm_Prim_Map = new QAction( QIcon( ( "C:/image/ina.gif" ) ), "Random_Prim_Making", this );
    Rm_Prim_Map->setStatusTip( "Random Prim Algorithm Map Making." );
    QObject::connect( Rm_Prim_Map, &QAction::triggered, [this]() { M->reset(); } );
    QObject::connect( Rm_Prim_Map, &QAction::triggered, [this]() { mk->random_prim_make_maze( 0 ); } );
    QObject::connect( Rm_Prim_Map, &QAction::triggered, [this]() { mk->set_Flag(); } );

    Multi_Rm_Prim_Map = new QAction( QIcon( ( "C:/image/ina.gif" ) ), "Multi_Random_Prim_Making", this );
    Multi_Rm_Prim_Map->setStatusTip( "Random Prim Algorithm Map Making with randomly breaking some wall." );
    QObject::connect( Multi_Rm_Prim_Map, &QAction::triggered, [this]() { M->reset(); } );
    QObject::connect( Multi_Rm_Prim_Map, &QAction::triggered, [this]() { mk->random_prim_make_maze( 1 ); } );
    QObject::connect( Multi_Rm_Prim_Map, &QAction::triggered, [this]() { mk->set_Flag(); } );

    Recursive_Map = new QAction( QIcon( ( "C:/image/ina.gif" ) ), "Recursive_Map", this );
    Recursive_Map->setStatusTip( "Recursive Backtracker Algorithm Map Making." );
    QObject::connect( Recursive_Map, &QAction::triggered, [this]() { M->reset(); } );
    QObject::connect( Recursive_Map, &QAction::triggered, [this]() { mk->recursion_make_maze(); } );
    QObject::connect( Recursive_Map, &QAction::triggered, [this]() { mk->set_Flag(); } );

    Recur_div_Map = new QAction( QIcon( ( "C:/image/ina.gif" ) ), "Recur_div_Map", this );
    Recur_div_Map->setStatusTip( "Recursive Division Algorithm Map Making." );
    QObject::connect( Recur_div_Map, &QAction::triggered, [this]() { M->reset(); } );
    QObject::connect( Recur_div_Map, &QAction::triggered, [this]() { M->reset_wall_around_maze(); } );
    QObject::connect( Recur_div_Map, &QAction::triggered, [this]() { mk->recursive_division( 1, 1, MAZE_HEIGHT - 2, MAZE_WIDTH - 2 ); } );
    QObject::connect( Recur_div_Map, &QAction::triggered, [this]() { mk->set_Flag(); } );

    //Search Menu
    SLV_Menu = this->menuBar()->addMenu( "&Searching" );
    SLV_Menu->addAction( DFS );
    SLV_Menu->addAction( BFS );
    SLV_Menu->addAction( Manhattan_Distance_UCS );
    SLV_Menu->addAction( Pow_Two_Norm_UCS );
    SLV_Menu->addAction( Interval_UCS );
    SLV_Menu->addAction( GREEDY );
    SLV_Menu->addAction( Normal_A_STAR );
    SLV_Menu->addAction( Interval_A_STAR );

    //Making Map Menu
    MK_Menu = this->menuBar()->addMenu( "&Map Making" );
    MK_Menu->addAction( Empty_Map );
    MK_Menu->addAction( Rm_Prim_Map );
    MK_Menu->addAction( Multi_Rm_Prim_Map );
    MK_Menu->addAction( Recursive_Map );
    MK_Menu->addAction( Recur_div_Map );
}    //end MainWindow::MainWindow

MainWindow::~MainWindow() {
    delete ui;
    delete M;
    delete mk;
    delete slv;
}
