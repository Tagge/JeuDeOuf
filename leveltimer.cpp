#include "leveltimer.h"
#include <QTimer>
#include <QDebug>

LevelTimer::LevelTimer()
{

}

void LevelTimer::run() {
    QTimer timer;
    timer.setTimerType(Qt::PreciseTimer);
    connect(&timer, SIGNAL(timeout()), this, SLOT(triggerEnd()));
    timer.start(ourLvl->getDuration());
    timeBegin = std::chrono::steady_clock::now();
    exec();
}

void LevelTimer::triggerEnd() {
    qDebug()  << "timer1";
    ourLvl->setTimeElapsed(true);
    qDebug()  << "timer2";
}

int LevelTimer::getTimeLeft() {
    auto timeNow = std::chrono::steady_clock::now();
    auto duration = timeNow - timeBegin;
    int timeElapsed = duration.count()/1000000000;
    return (ourLvl->getDuration()/1000)-timeElapsed;
}

