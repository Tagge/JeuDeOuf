#include "drawthread.h"
#include "header.h"
#include "player.h"
#include "leveltimer.h"
#include <QDebug>
#include <QMutex>

DrawThread::DrawThread()
{

}

void DrawThread::run()
{
    QTimer timer;
    timer.setTimerType(Qt::PreciseTimer);
    connect(&timer, SIGNAL(timeout()), this, SLOT(doDraw()));
    double frame = 1000/constants::FPS_DRAW;
    timer.start(frame);
    exec();
}

void DrawThread::doDraw()
{
    QMutex mutex;
    if(game->getLvl()->getTerminate()) {
        mutex.lock();
        LevelTimer * timer = game->getLvl()->getTimer();
        int livesLeft = game->getLvl()->getPlayer()->getLivesLeft();
        delete(game->getLvl());
        game->setLevel(new Level(game->getLevelPath(), livesLeft, timer));
        timer->setLvl(game->getLvl());
        int yWindow = game->getLvl()->getYWindow();
        game->getLvl()->setYWindow(yWindow-game->getHeightOrigin());
        mutex.unlock();
        return;
    }
    game->repaint();
}
