#ifndef CALCULATETHREAD_H
#define CALCULATETHREAD_H

#include "gamewindow.h"

#include <QKeyEvent>
#include <QThread>
#include <QTimer>

class GameWindow;

class CalculateThread: public QThread
{
    Q_OBJECT
private:
    GameWindow * game;
public:
    CalculateThread();
    void run();
    inline void setGame(GameWindow * window) {game = window;};
private slots:
    void doCalculations();
};

#endif // CALCULATETHREAD_H
