#ifndef POWERUP_H
#define POWERUP_H

#include "livingentity.h"

class PowerUp: public LivingEntity
{
public:
    PowerUp();
    PowerUp(int x, int y, Animation * anim);
    virtual void update(Level * const level);
    virtual void collide(LivingEntity * e, Level * const l);
    virtual void collide(Player * p, Level * const l);
    virtual void collide(LuckyBlock * lb, Level * const l);
    void endTurn();
    void move(Level * const level, QRect limit);
    virtual void deathTimer();
};

#endif // POWERUP_H
