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
    timer.stop();
    ourLvl = nullptr;
}

void LevelTimer::triggerEnd() {
    if(ourLvl == nullptr){
        return;
    }
    ourLvl->setTimeElapsed(true);
}

int LevelTimer::getTimeLeft() {
    auto timeNow = std::chrono::steady_clock::now();
    auto duration = timeNow - timeBegin;
    int timeElapsed = duration.count()/1000000000;
    return (ourLvl->getDuration()/1000)-timeElapsed;
}

