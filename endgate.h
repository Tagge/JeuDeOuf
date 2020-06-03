#ifndef ENDGATE_H
#define ENDGATE_H

#include "level.h"
#include "livingentity.h"

class Level;

class EndGate: public LivingEntity
{
public:
    EndGate();
    EndGate(int x, int y, const QMap<QString, Animation*> & animations);
    virtual void update(Level * const level);
    virtual void collide(LivingEntity * e, Level * const l);
    virtual void collide(Player * p, Level * l);
    void move(Level * const level, QRect limit);
    void endTurn();
};

#endif // ENDGATE_H
