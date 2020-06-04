#ifndef BRICK_H
#define BRICK_H

#include "livingentity.h"

class Brick: public LivingEntity
{
private:
    int movingFor = 0;
public:
    Brick();
    Brick(int x, int y, const QMap<QString, Animation*> & animations);
    virtual void update(Level * const level);
    virtual void collide(LivingEntity * e, Level * const l);
    virtual void collide(Roomba * r, Level * const l);
    virtual void collide(Player * p, Level * const l);
    virtual void collide(Brick * b, Level * const l);
    void endTurn();
    void move(Level * const level, QRect limit);
    void jump();
    virtual void deathTimer();
};

#endif // BRICK_H
