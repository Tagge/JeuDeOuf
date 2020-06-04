#ifndef BUZZER_H
#define BUZZER_H

#include "livingentity.h"



class Buzzer: public LivingEntity
{
private:
    double speed = 0.025*constants::TILE_WIDTH;
    int startx;
    int starty;
    int endx;
    int endy;
    bool started = false;
    bool turneds = false;
    bool turnede = false;
public:
    Buzzer();
    Buzzer(int startx, int starty, int endx, int endy, const QMap<QString, Animation*> & animations);
    virtual void update(Level * const level);
    virtual void collide(LivingEntity * e, Level * const l);
    virtual void collide(Player * p, Level * const l);
    void endTurn();
    void move(Level * const level, QRect limit);
    virtual void deathTimer();
};

#endif // BUZZER_H
