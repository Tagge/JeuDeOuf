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
    setFacingBack(true);
}

void Roomba::update(Level * const level)
{
    if(getHealth()<1) {
        setAnimPos(1);
    } else {
        if(getHitbox().left() - level->getPlayer()->getHitbox().right() < 5*constants::TILE_WIDTH)
            if(getFacingBack()) {
                setVectorX(-getMaxSpeed());
            }
            else {
                setVectorX(getMaxSpeed());
            }
        }
    if(onGround(level)){
        setJumpTime(0);
    }
    if(getFallingTime() >  0) {
        fall();
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
