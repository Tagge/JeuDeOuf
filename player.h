#ifndef PLAYER_H
#define PLAYER_H

#include "level.h"
#include "groundentity.h"

class Level;

class Player: public GroundEntity
{
private:
    double jumpTime;
    double maxSpeed = 6;
    double acceleration = 2;
    double jumpTop = 0.3333;
public:
    Player();
    Player(int x, int y, const QMap<QString, Animation*> & animations);
    virtual void update(Level * const level);
    void jump();
};

#endif // PLAYER_H
