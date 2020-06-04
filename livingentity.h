#ifndef LIVINGENTITY_H
#define LIVINGENTITY_H

#include "entity.h"
#include "level.h"

class Player;
class Roomba;
class LuckyBlock;
class PowerUp;
class EndGate;
class MovingPlatform;
class Level;

class LivingEntity: public Entity
{
private:
    double vectorX;
    double vectorY;
    QRectF posTmp;
    double maxSpeed = 6;
    double acceleration = 2;
    double health = 0;
    int intangible = 0;
    bool isDead = false;

public:
    LivingEntity();
    virtual void update(Level * const level) = 0;
    virtual void move(Level * const level, QRect limit) = 0;
    bool detectCollisionMap(Level * const level);
    QVector<LivingEntity *> getCollidingEntities(double id, Level * const level);
    void validatePos();
    inline double getVectorX() const {return vectorX;};
    inline void setVectorX(double value) {vectorX = value;};
    inline double getVectorY() const {return vectorY;};
    inline void setVectorY(double value) {vectorY = value;};
    inline QRectF getPosTmp() const {return posTmp;};
    inline void setPosTmp(double x, double y) {posTmp = QRectF(x, y, getHitbox().width(), getHitbox().height());};
    virtual void endTurn() = 0;
    inline double getMaxSpeed() {return maxSpeed;};
    inline double getAccel() {return acceleration;};
    inline void setMaxSpeed(double value) {maxSpeed = value;};
    inline void setAccel(double value) {acceleration = value;};
    inline virtual void collide(LivingEntity * e, Level * const l) {};
    inline virtual void collide(Player * p, Level * const l) {};
    inline virtual void collide(Roomba * r, Level * const l) {};
    inline virtual void collide(LuckyBlock * lb, Level * const l) {};
    inline virtual void collide(PowerUp * pu, Level * const l) {};
    inline virtual void collide(MovingPlatform * mp, Level * const l) {};
    inline virtual void collide(EndGate * eg, Level * const l) {};
    double getHealth() const;
    void setHealth(double value);
    inline bool getIsDead() const {return isDead;};
    inline void setIsDead(bool value) {isDead = value;};
    int getIntangible() const;
    void setIntangible(int value);
    virtual void deathTimer();
};

#endif // LIVINGENTITY_H
