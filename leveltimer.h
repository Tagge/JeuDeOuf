#ifndef LEVELTIMER_H
#define LEVELTIMER_H

#include<QThread>
#include<QTimer>
#include<chrono>
#include<QDebug>
#include "level.h"
#include "header.h"

class Level;

class LevelTimer : public QThread
{
    Q_OBJECT
private:
    Level * ourLvl;
    std::chrono::time_point<std::chrono::steady_clock> timeBegin;
public:
    LevelTimer();
    void run();
    inline void setLvl(Level * lvl) { ourLvl = lvl;};
    int getTimeLeft();
    inline void setTimeBegin(std::chrono::time_point<std::chrono::steady_clock> value) {timeBegin=value;};
private slots:
    void triggerEnd();
};

#endif // LEVELTIMER_H
