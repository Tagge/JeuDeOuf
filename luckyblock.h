#ifndef LUCKYBLOCK_H
#define LUCKYBLOCK_H

#include "livingentity.h"



class LuckyBlock: public LivingEntity
{
private:
    int objContained;
public:
    LuckyBlock();
    LuckyBlock(int x, int y, const QMap<QString, Animation*> & animations, int obj);
    virtual void update(Level * const level);
    virtual void collide(LivingEntity * e);
    virtual void collide(Roomba * r);
    virtual void collide(Player * p);
    void endTurn();
    void move(Level * const level, QRect limit);
};

#endif // LUCKYBLOCK_H
