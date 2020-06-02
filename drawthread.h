#ifndef DRAWTHREAD_H
#define DRAWTHREAD_H

#include "gamewindow.h"

#include <QThread>

class GameWindow;

class DrawThread: public QThread
{
    Q_OBJECT
private:
    GameWindow * game;
public:
    DrawThread();
    void run();
    inline void setGame(GameWindow * window) {game = window;};
private slots:
    void doDraw();
};

#endif // DRAWTHREAD_H
