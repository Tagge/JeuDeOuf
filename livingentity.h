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

public:
    LivingEntity();
    virtual void update(Level * const level) = 0;
    void move(Level * const level, QRect limit);
    bool detectCollisionMap(Level * const level);
    void validatePos();
    inline double getVectorX() const {return vectorX;};
    inline void setVectorX(double value) {vectorX = value;};
    inline double getVectorY() const {return vectorY;};
    inline void setVectorY(double value) {vectorY = value;};
    inline QRectF getPosTmp() const {return posTmp;};
    inline void setPosTmp(int x, int y) {posTmp = QRectF(x, y, getHitbox().width(), getHitbox().height());};
    void endTurn();
};

#endif // LIVINGENTITY_H
