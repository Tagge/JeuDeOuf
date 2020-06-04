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
    exec();
}

void LevelTimer::triggerEnd() {
    qDebug()  << "timer1";
    ourLvl->setTimeElapsed(true);
    qDebug()  << "timer2";
}
