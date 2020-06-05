#include "movingplatform.h"
#include <QDebug>

QVector<GroundEntity *> MovingPlatform::getSteppedOnBy() const
{
    return steppedOnBy;
}

void MovingPlatform::setSteppedOnBy(const QVector<GroundEntity *> &value)
{
    steppedOnBy = value;
}

MovingPlatform::MovingPlatform()
{

}

MovingPlatform::MovingPlatform(int startx, int starty, int endx, int endy, int size, const QMap<QString, Animation *> &animations)
{
    this->startx = startx * constants::TILE_WIDTH;
    this->starty = starty * constants::TILE_HEIGHT;
    this->endx = endx * constants::TILE_WIDTH;
    this->endy = endy * constants::TILE_HEIGHT;
    QRectF hitbox(this->startx, this->starty, constants::TILE_WIDTH*size, constants::TILE_HEIGHT*0.5);
    setImagePos(hitbox);
    setHitbox(hitbox);
    if(size == 2) {
        addAnimation(animations["platform2"]);
        setAnimPos(0);
    } else if (size == 3) {
        addAnimation(animations["platform3"]);
        setAnimPos(0);
    }
}

void MovingPlatform::update(Level * const level)
{
    for(int i = 0; i<steppedOnBy.size(); i++) {
        if(steppedOnBy[i]->getHitbox().bottom() - getHitbox().top() > 5 || steppedOnBy[i]->getHitbox().bottom() - getHitbox().top() < -5) {
            steppedOnBy.remove(i);
        }
    }

    if(started == false) {
        double xdif = startx - endx;
        double ydif = starty - endy;
        double ratiospeed = speed/(abs(xdif) + abs(ydif));
        setVectorX((-ratiospeed * xdif));
        setVectorY((-ratiospeed * ydif));
        started = true;
        turneds = true;
    } else {
        if (abs(getHitbox().left() - endx) < 5 && abs(getHitbox().top() - endy) < 5 && turnede == false){
            setVectorX(-getVectorX());
            setVectorY(-getVectorY());
            turnede = true;
            turneds = false;
        }
        else if (abs(getHitbox().left() - startx) < 5 && abs(getHitbox().top() - starty) < 5 && turneds == false) {
            setVectorX(-getVectorX());
            setVectorY(-getVectorY());
            turneds = true;
            turnede = false;
        }
    }
    QRect limit(level->getXWindow(), 0, level->getNbCols()*constants::TILE_WIDTH-level->getXWindow(), level->getNbRows()*constants::TILE_HEIGHT);
    move(level, limit);
}

void MovingPlatform::collide(LivingEntity *e, Level * const l)
{
    e->collide(this, l);
}

void MovingPlatform::collide(Roomba *r, Level * const l)
{

}

void MovingPlatform::collide(Player *p, Level * const l)
{

}

void MovingPlatform::endTurn()
{

}

void MovingPlatform::move(Level * const level, QRect limit)
{
    QRectF pos = getHitbox();
    setPosTmp(pos.left()+getVectorX(), pos.top()+getVectorY());
    validatePos();

    //Move entities stepping on.
    for(int i = 0; i<steppedOnBy.size(); i++) {
        steppedOnBy[i]->setPosTmp(steppedOnBy[i]->getHitbox().left()+getVectorX(),steppedOnBy[i]->getHitbox().top()+getVectorY());
        steppedOnBy[i]->validatePos();
    }
}
