#include "calculatethread.h"

#include <QMutex>
#include <QTimer>
#include <QDebug>
#include <QTime>
#include "player.h"
#include "header.h"

CalculateThread::CalculateThread()
{

}

void CalculateThread::run()
{
    QTimer timer;
    timer.setTimerType(Qt::PreciseTimer);
    connect(&timer, SIGNAL(timeout()), this, SLOT(doCalculations()));
    double frame = 1000/constants::FPS_CALCULATION;
    timer.start(frame);
    exec();
}

void CalculateThread::doCalculations()
{
    QMutex mutex;
    int size = game->getLvl()->getNbEntities();
    for(int idEntity = 0; idEntity < size; idEntity++){
        mutex.lock();
        game->getLvl()->getEntity(idEntity)->update(game->getLvl());
        mutex.unlock();
    }
    for(int idEntity = 0; idEntity < size; idEntity++){
        mutex.lock();
        game->getLvl()->getEntity(idEntity)->endTurn();
        mutex.unlock();
    }
    int xPlayer = game->getLvl()->getPlayer()->getHitbox().left();
    int halfWidth = game->getWidthOrigin()/2;
    int center = game->getLvl()->getXWindow()+halfWidth;
    if(xPlayer > center){
        mutex.lock();
        game->getLvl()->setXWindow(xPlayer-halfWidth);
        mutex.unlock();
    }
}
