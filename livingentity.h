#ifndef LIVINGENTITY_H
#define LIVINGENTITY_H

#include "entity.h"
#include "level.h"

class Level;

class LivingEntity: public Entity
{
private:
    double vectorX;
    double vectorY;
    QRectF posTmp;
    double maxSpeed = 6;
    double acceleration = 2;
    double health = 1;

public:
    LivingEntity();
    virtual void update(Level * const level) = 0;
    void move(Level * const level, QRect limit);
    bool detectCollisionMap(Level * const level);
    QVector<LivingEntity *> getCollidingEntities(double id, Level * const level);
    void validatePos();
    inline double getVectorX() const {return vectorX;};
    inline void setVectorX(double value) {vectorX = value;};
    inline double getVectorY() const {return vectorY;};
    inline void setVectorY(double value) {vectorY = value;};
    inline QRectF getPosTmp() const {return posTmp;};
    inline void setPosTmp(int x, int y) {posTmp = QRectF(x, y, getHitbox().width(), getHitbox().height());};
    virtual void endTurn() = 0;
    inline double getMaxSpeed() {return maxSpeed;};
    inline double getAccel() {return acceleration;};
    inline void setMaxSpeed(double value) {maxSpeed = value;};
    inline void setAccel(double value) {acceleration = value;};
    inline virtual void collide(LivingEntity * e) {};
    inline virtual void collide(Player * p) {};
    inline virtual void collide(Roomba * r) {};
    double getHealth() const;
    void setHealth(double value);
};

#endif // LIVINGENTITY_H
