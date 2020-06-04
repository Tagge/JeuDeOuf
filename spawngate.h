#ifndef SPAWNGATE_H
#define SPAWNGATE_H

#include "livingentity.h"

class SpawnGate: public LivingEntity
{
public:
    SpawnGate();
    SpawnGate(int x, int y, const QMap<QString, Animation*> & animations);
    virtual void update(Level * const level) {};
    void move(Level * const level, QRect limit) {};
    void endTurn(){};
    virtual void deathTimer();
};

#endif // SPAWNGATE_H
