#ifndef PLAYER_H
#define PLAYER_H

#include "level.h"
#include "groundentity.h"

class Level;

class Player: public GroundEntity
{
private:
public:
    Player();
    Player(int x, int y, const QMap<QString, Animation*> & animations);
    virtual void update(Level * const level);
    virtual void collide(LivingEntity * e, Level * const l);
    virtual void collide(Roomba * r, Level * const l);
    virtual void collide(LuckyBlock * lb, Level * const l);
    void jump();
    void endTurn();
    void move(Level * const level, QRect limit);
};

#endif // PLAYER_H
