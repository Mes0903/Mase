#include "animator.h"
#include "mainwindow.h"

#include <QPainter>
#include <QThread>
#include <iostream>

Animator::Animator( MainWindow *__Window ) : Window( __Window ) {
}

Animator::~Animator() {
    Window = nullptr;
}

void Animator::update( const int m_sec ) {
    QThread::msleep( m_sec );
    Window->repaint();
}
