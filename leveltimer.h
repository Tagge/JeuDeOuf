#ifndef LEVELTIMER_H
#define LEVELTIMER_H

#include<QThread>
#include<QTimer>
#include "level.h"
#include "header.h"

class Level;

class LevelTimer : public QThread
{
    Q_OBJECT
private:
    Level * ourLvl;
public:
    LevelTimer();
    void run();
    inline void setLvl(Level * lvl) { ourLvl = lvl;};
private slots:
    void triggerEnd();
};

#endif // LEVELTIMER_H
