#include "roomba.h"
#include <QDebug>
#include "math.h"
#include "header.h"
#include "player.h"
#include "luckyblock.h"


Roomba::Roomba():GroundEntity(0, 0)
{
    setJumpTime(0);
}

Roomba::Roomba(int x, int y, const QMap<QString, Animation *> &animations):GroundEntity(constants::TILE_HEIGHT*3.33, 0.3333)
{
    QRectF hitbox(x*constants::TILE_WIDTH, y*constants::TILE_HEIGHT, constants::TILE_WIDTH, constants::TILE_HEIGHT*11.0/16.0);
    setImagePos(hitbox);
    setHitbox(hitbox);
    addAnimation(animations["roomba"]);
    addAnimation(animations["roomba_death"]);
    setAccel(getAccel()*constants::TILE_WIDTH/constants::FPS_CALCULATION);
    setMaxSpeed(getMaxSpeed()*constants::TILE_WIDTH/constants::FPS_CALCULATION);
    setJumpTime(0);
    setFacingBack(true);
    setHealth(1);
    started = 0;
}

void Roomba::update(Level * const level)
{
    if(getHealth()<1) {
        setAnimPos(1);
    } else {
        if(getHitbox().left() - level->getPlayer()->getHitbox().right() < 10*constants::TILE_WIDTH && started == 0) {
            if(getFacingBack()) {
                setVectorX(-0.025*constants::TILE_WIDTH);
            }
            else {
                setVectorX(0.025*constants::TILE_WIDTH);
            }
        started = 1;
        }
    }
    if(onGround(level)){
        setJumpTime(0);
    }
    if(getFallingTime() >  0) {
        fall();
    }
    if(getIntangible() > 0) {
        setIntangible((getIntangible()-1));
    }
    QRect limit(0, 0, level->getNbCols()*constants::TILE_WIDTH, level->getNbRows()*constants::TILE_HEIGHT);
    move(level, limit);
}

void Roomba::collide(LivingEntity *e, Level * const l)
{
    e->collide(this, l);
}

void Roomba::collide(Player *p, Level * const l)
{
}

void Roomba::collide(LuckyBlock * lb, Level * const l)
{
    QRectF pos = getHitbox();
    QRectF lucky = lb->getHitbox();
    setPosTmp(pos.left(), pos.top());
    int direction;
    int leftdir = abs(pos.right() - lucky.left());
    int topdir = abs(pos.bottom() - lucky.top());
    int rightdir = abs(pos.left() - lucky.right());
    int botdir = abs(pos.top() - lucky.bottom());
    if(leftdir <= topdir && leftdir <= rightdir && leftdir <= botdir) {
        direction = 1;
    } else if (topdir <= leftdir && topdir <= rightdir && topdir <= botdir){
        direction = 2;
    } else if (rightdir <= leftdir && rightdir <= topdir && rightdir <= botdir) {
        direction = 3;
    } else {
        direction = 4;
    }

    //Gauche
    if(direction == 1) {
        setPosTmp(lucky.left()-getHitbox().width(),getPosTmp().top());
        setVectorX(-getVectorX());
        setFacingBack(!getFacingBack());
        if(getHealth() == 0) {
            lb->dropItem(l);
        }
    }
    //Haut
    else if (direction == 2) {
        setPosTmp(getPosTmp().left(), lucky.top()-getHitbox().height());
        setOnSolid(true);
    }
    //Droite
    else if (direction == 3) {
        setPosTmp(lucky.right(), getPosTmp().top());
        setVectorX(-getVectorX());
        setFacingBack(!getFacingBack());
        if(getHealth() == 0) {
            lb->dropItem(l);
        }
    }
    //Bas
    else {
        setPosTmp(getPosTmp().left(), lucky.bottom());
        setJumpTime(getJumpTop() * constants::FPS_CALCULATION + 1);
    }
    validatePos();
}

void Roomba::collide(Roomba *r, Level * const l)
{
    if(getIntangible() == 0 && r->getIntangible() == 0 && getHealth() >= 0 && r->getHealth() >= 0) {
        if(getHealth() == r->getHealth()) {
            setVectorX(-getVectorX());
            setIntangible(5);
            setFacingBack(!getFacingBack());
            r->setVectorX(-r->getVectorX());
            r->setIntangible(5);
            r->setFacingBack(!r->getFacingBack());
        } else if(getHealth() > r->getHealth()) {
            if(r->getVectorX() != 0) {
                setHealth(-1);
                setVectorX(0);
            } else {
                setVectorX(-getVectorX());
                setIntangible(5);
                setFacingBack(!getFacingBack());
            }
        } else {
            if(getVectorX() != 0) {
                r->setHealth(-1);
                r->setVectorX(0);
            }
            else {
                r->setVectorX(-r->getVectorX());
                r->setIntangible(5);
                r->setFacingBack(!r->getFacingBack());
            }
        }
    }
}

void Roomba::collide(Brick *b, Level * const l)
{

}

void Roomba::endTurn()
{

}

void Roomba::move(Level * const level, QRect limit)
{
    QRectF pos = getHitbox();
    setPosTmp(pos.left()+(int)getVectorX(), pos.top());
    if(getPosTmp().left() < limit.left()){
        setPosTmp(limit.left(), getPosTmp().top());
    }
    if(getPosTmp().right() > limit.right()){
        setPosTmp(limit.right()-getHitbox().width(), getPosTmp().top());
    }
    bool collision = detectCollisionMap(level);
    int modifier = 1;
    if(getVectorX() > 0){
        modifier = -1;
    }
    while(collision){
        setPosTmp(getPosTmp().left()+modifier, pos.top());
        collision = detectCollisionMap(level);
        if(!collision) {
            setVectorX(-getVectorX());
            setFacingBack(!getFacingBack());
        }
    }

    setPosTmp(getPosTmp().left(), pos.top()+(int)getVectorY());
    if(getPosTmp().top() < limit.top()){
        setPosTmp(getPosTmp().left(), limit.top());
    }
    if(getPosTmp().bottom() > limit.bottom()){
        setPosTmp(getPosTmp().left(), limit.bottom()-getHitbox().height());
    }
    collision = detectCollisionMap(level);
    modifier = 1;
    if(getVectorY() > 0){
        modifier = -1;
    }
    while(collision){
        setPosTmp(getPosTmp().left(), getPosTmp().top()+modifier);
        collision = detectCollisionMap(level);
    }
    validatePos();
}

void Roomba::deathTimer()
{
    if(getHealth() < 0){
        setAnimPos(1);
        setHealth(getHealth()-1);
    }
    if(getHealth() == -constants::FPS_CALCULATION/4){
        setIsDead(true);
    }
}
