#ifndef ROOMBA_H
#define ROOMBA_H

#include "level.h"
#include "groundentity.h"

class Level;

class Roomba: public GroundEntity
{
private:
    bool started;
public:
    Roomba();
    Roomba(int x, int y, const QMap<QString, Animation *> &animations);
    virtual void update(Level * const level);
    virtual void collide(LivingEntity * e, Level * const l);
    virtual void collide(Player * p, Level * const l);
    virtual void collide(LuckyBlock * lb, Level * const l);
    virtual void collide(Roomba * r, Level * const l);
    virtual void collide(Brick * b, Level * const l);
    virtual void endTurn();
    void move(Level * const level, QRect limit);
    virtual void deathTimer();
};

#endif // ROOMBA_H
