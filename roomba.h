#ifndef ROOMBA_H
#define ROOMBA_H

#include "level.h"
#include "groundentity.h"

class Level;

class Roomba: public GroundEntity
{
private:
public:
    Roomba();
    Roomba(int x, int y, const QMap<QString, Animation *> &animations);
    virtual void update(Level * const level);
    virtual void collide(LivingEntity * e);
    virtual void collide(Player * p);
    virtual void endTurn();
    void move(Level * const level, QRect limit);
};

#endif // ROOMBA_H