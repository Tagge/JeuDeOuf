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
    double jumpTime;
    double jumpTop = 0.3333;
    bool onSolid = false;
public:
    GroundEntity();
    GroundEntity(double jumpHeight, double jumpTime);
    virtual void update(Level * const level) = 0;
    bool onGround(Level * const level);
    bool underCeiling(Level * const level);
    void fall();
    inline int getFallingTime() const {return fallingTime;};
    inline void setFallingTime(int value) {fallingTime = value;};
    inline double getG() const {return g;};
    inline void setG(double value) {g = value;};
    inline double getGFast() const {return gFast;};
    inline void setGFast(double value) {gFast = value;};
    inline double getV0() const {return v0;};
    inline void setV0(double value) {v0 = value;};
    inline double getJumpTime() {return jumpTime;};
    inline double getJumpTop() {return jumpTop;};
    inline void setJumpTime(double value) {jumpTime = value;};
    inline void setJumpTop(double value) {jumpTop = value;};
    bool getOnSolid() const;
    void setOnSolid(bool value);
};

#endif // GROUNDENTITY_H
