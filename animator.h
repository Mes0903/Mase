#ifndef ANIMATOR_H
#define ANIMATOR_H

class MainWindow;

class Animator {

  public:
    ~Animator();
    void update( const int m_sec );
    Animator(MainWindow *window);

  private:
    MainWindow *Window;
};

#endif    // ANIMATOR_H
