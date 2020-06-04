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
    qDebug() << "calcStart";
    QMutex mutex;
    if(!game->getIngame()) {
        return;
    }
    if(game->getLvl()->getTerminate()) {
        return;
    }
    if(game->getLvl()->getPlayer()->getIsDead()){
        mutex.try_lock();
        game->getLvl()->setTerminate(true);
        mutex.unlock();
        return;
    }
    int size = game->getLvl()->getNbEntities();
    for(int idEntity = 0; idEntity < size; idEntity++){
        if(game->getLvl()->getEntity(idEntity)->getIsDead()){
            mutex.try_lock();
            delete game->getLvl()->getEntity(idEntity);
            game->getLvl()->removeEntity(idEntity);
            idEntity--;
            size--;
            mutex.unlock();
        }
    }
    int xPlayer = game->getLvl()->getPlayer()->getHitbox().left();
    for(int idEntity = 0; idEntity < size; idEntity++){
        int left = game->getLvl()->getEntity(idEntity)->getHitbox().left();
        if(left >= xPlayer-(constants::TILE_WIDTH*10) && left <= xPlayer+(constants::TILE_WIDTH*10) && !game->getLvl()->getWin()){
            mutex.try_lock();
            game->getLvl()->getEntity(idEntity)->update(game->getLvl());
            mutex.unlock();
        }
    }
    for(int idEntity = 0; idEntity < size; idEntity++){
        mutex.try_lock();
        QVector<LivingEntity *> colliding = game->getLvl()->getEntity(idEntity)->getCollidingEntities(idEntity, game->getLvl());
        for(int i=0; i<colliding.size(); i++) {
            game->getLvl()->getEntity(idEntity)->collide(colliding[i], game->getLvl());
        }
        game->getLvl()->getEntity(idEntity)->deathTimer();
        mutex.unlock();
    }
    for(int idEntity = 0; idEntity < size; idEntity++){
        mutex.try_lock();
        game->getLvl()->getEntity(idEntity)->endTurn();
        mutex.unlock();
    }
    int halfWidth = game->getWidthOrigin()/2;
    int center = game->getLvl()->getXWindow()+halfWidth;
    xPlayer = game->getLvl()->getPlayer()->getHitbox().left();
    if(xPlayer > center){
        mutex.try_lock();
        game->getLvl()->setXWindow(xPlayer-halfWidth);
        mutex.unlock();
    }
    qDebug() << "calcFin";
}
