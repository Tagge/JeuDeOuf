#ifndef LUCKYBLOCK_H
#define LUCKYBLOCK_H

#include "level.h"
#include "livingentity.h"

class Level;

class LuckyBlock: public LivingEntity
{
private:
    int objContained;
    int activatedFor = 0;
public:
    LuckyBlock();
    LuckyBlock(int x, int y, const QMap<QString, Animation*> & animations, int obj);
    virtual void update(Level * const level);
    virtual void collide(LivingEntity * e, Level * const l);
    virtual void collide(Roomba * r, Level * const l);
    virtual void collide(Player * p, Level * const l);
    void endTurn();
    void move(Level * const level, QRect limit);
    void dropItem(Level * l);
};

#endif // LUCKYBLOCK_H
