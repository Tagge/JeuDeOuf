#ifndef GROUNDENTITY_H
#define GROUNDENTITY_H

#include "livingentity.h"

class GroundEntity: public LivingEntity
{
private:
    int fallingTime;
    double g;
    double gFast;
    double v0;
public:
    GroundEntity();
    GroundEntity(double jumpHeight, double jumpTime);
    virtual void update(Level * const level) = 0;
    bool onGround(Level * const level);
    void fall();
    inline int getFallingTime() const {return fallingTime;};
    inline void setFallingTime(int value) {fallingTime = value;};
    inline double getG() const {return g;};
    inline void setG(double value) {g = value;};
    inline double getGFast() const {return gFast;};
    inline void setGFast(double value) {gFast = value;};
    inline double getV0() const {return v0;};
    inline void setV0(double value) {v0 = value;};
};

#endif // GROUNDENTITY_H
