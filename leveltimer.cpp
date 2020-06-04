#include "leveltimer.h"

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
    ourLvl->setTimeElapsed(true);
}
