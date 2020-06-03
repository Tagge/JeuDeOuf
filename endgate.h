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
    virtual void collide(LivingEntity * e);
    virtual void collide(Roomba * r);
    virtual void collide(Player * p);
};

#endif // ENDGATE_H
