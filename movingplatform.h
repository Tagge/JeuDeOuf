#ifndef MOVINGPLATFORM_H
#define MOVINGPLATFORM_H

#include "groundentity.h"
#include "livingentity.h"


class MovingPlatform: public LivingEntity
{
private:
    double speed = 0.025*constants::TILE_WIDTH;
    int startx;
    int starty;
    int endx;
    int endy;
    bool started = false;
    bool turneds = false;
    bool turnede = false;
    QVector<GroundEntity *> steppedOnBy;
public:
    MovingPlatform();
    MovingPlatform(int startx, int starty, int endx, int endy, int size, const QMap<QString, Animation*> & animations);
    virtual void update(Level * const level);
    virtual void collide(LivingEntity * e, Level * const l);
    virtual void collide(Roomba * r, Level * const l);
    virtual void collide(Player * p, Level * const l);
    void endTurn();
    void move(Level * const level, QRect limit);
    inline void stepOn(GroundEntity * e) {steppedOnBy.push_back(e);};
    QVector<GroundEntity *> getSteppedOnBy() const;
    void setSteppedOnBy(const QVector<GroundEntity *> &value);
};

#endif // MOVINGPLATFORM_H
