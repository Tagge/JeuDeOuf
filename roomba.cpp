#include "roomba.h"
#include <QDebug>
#include "math.h"
#include "header.h"
#include "player.h"

Roomba::Roomba():GroundEntity(0, 0)
{
    setJumpTime(0);
}

Roomba::Roomba(int x, int y, const QMap<QString, Animation *> &animations):GroundEntity(constants::TILE_HEIGHT*3.33, 0.3333)
{
    QRectF hitbox(x*constants::TILE_WIDTH, y*constants::TILE_HEIGHT, constants::TILE_WIDTH*9/16, constants::TILE_HEIGHT);
    QRectF posImage((x-3.0/16.0)*constants::TILE_WIDTH, y*constants::TILE_HEIGHT, constants::TILE_WIDTH, constants::TILE_HEIGHT);
    setImagePos(posImage);
    setHitbox(hitbox);
    addAnimation(animations["roomba"]);
    addAnimation(animations["roomba_death"]);
    setAccel(getAccel()*constants::TILE_WIDTH/constants::FPS_CALCULATION);
    setMaxSpeed(getMaxSpeed()*constants::TILE_WIDTH/constants::FPS_CALCULATION);
    setJumpTime(0);
    setFacingBack(false);
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
                setVectorX(-0.05*constants::TILE_WIDTH);
            }
            else {
                setVectorX(0.05*constants::TILE_WIDTH);
            }
        started = 1;
        qDebug() << "started = " << started;
        }
    }
    if(onGround(level)){
        setJumpTime(0);
    }
    if(getFallingTime() >  0) {
        fall();
    }
    if(getIntangible() > 0) {
        setIntangible((getIntangible()+1)%constants::INTANGIBLE_FRAMES);
    }
    QRect limit(0, 0, level->getNbCols()*constants::TILE_WIDTH, level->getNbRows()*constants::TILE_HEIGHT);
    move(level, limit);
}

void Roomba::collide(LivingEntity *e)
{
    e->collide(this);
}

void Roomba::collide(Player *p)
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


/*
void Roomba::jump()
{
    double y = getVectorY();
    double t_1 = (getJumpTime()-1) / constants::FPS_CALCULATION;
    double t = getJumpTime() / constants::FPS_CALCULATION;
    double g = getG();
    if(t > getJumpTop()){
        g = getGFast();
    }
    double posT = 0.5*g*pow(t, 2)+getV0()*t;
    double posT_1 = 0.5*g*pow(t_1, 2)+getV0()*t_1;
    setVectorY(y-(posT-posT_1));
    setJumpTime(getJumpTime()+1);
}
*/
