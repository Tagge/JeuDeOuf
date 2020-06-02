#include "player.h"
#include <QDebug>
#include "math.h"
#include "header.h"
#include "roomba.h"

Player::Player():GroundEntity(0, 0)
{
    setJumpTime(0);
}

Player::Player(int x, int y, const QMap<QString, Animation *> &animations):GroundEntity(constants::TILE_HEIGHT*3.33, 0.3333)
{
    QRectF hitbox(x*constants::TILE_WIDTH, y*constants::TILE_HEIGHT, constants::TILE_WIDTH*8/16, constants::TILE_HEIGHT);
    double xTmp = (x-4.0/16.0)*constants::TILE_WIDTH;
    QRectF posImage(xTmp, y*constants::TILE_HEIGHT, constants::TILE_WIDTH, constants::TILE_HEIGHT);
    setImagePos(posImage);
    setHitbox(hitbox);
    addAnimation(animations["character"]);
    addAnimation(animations["character_move"]);
    addAnimation(animations["character_jump"]);
    addAnimation(animations["character_death"]);
    setAccel(getAccel()*constants::TILE_WIDTH/constants::FPS_CALCULATION);
    setMaxSpeed(getMaxSpeed()*constants::TILE_WIDTH/constants::FPS_CALCULATION);
    setJumpTime(0);
}

void Player::update(Level * const level)
{
    if(onGround(level)){
        setJumpTime(0);
    }
    if(underCeiling(level)){
        setJumpTime(getJumpTop() * constants::FPS_CALCULATION + 1);
    }
    setAnimPos(0);
    if(level->getKey(0)){
        double x = getVectorX();
        setVectorX(x-getAccel());
    }
    if(level->getKey(1)){
        double x = getVectorX();
        setVectorX(x+getAccel());
    }
    if(level->getKey(2) && getJumpTime() == 0 && getFallingTime() == 0){
        setJumpTime(1);
    }
    if(getVectorX() > getMaxSpeed()){
        setVectorX(getMaxSpeed());
    }
    if(getVectorX() < -getMaxSpeed()){
        setVectorX(-getMaxSpeed());
    }
    if(getVectorX() < -0.9){
        setAnimPos(1);
        setFacingBack(true);
    }
    else if(getVectorX() > 0.9){
        setAnimPos(1);
        setFacingBack(false);
    }
    if(getJumpTime() > 0){
        setAnimPos(2);
        jump();
    }
    else if(getFallingTime() >  0){
        fall();
    }
    QRect limit(level->getXWindow(), 0, level->getNbCols()*constants::TILE_WIDTH-level->getXWindow(), level->getNbRows()*constants::TILE_HEIGHT);
    move(level, limit);
}

void Player::collide(LivingEntity *e)
{
    e->collide(this);
}

void Player::collide(Roomba *r)
{

    //type collision
    QRectF roomba = r->getHitbox();
    QRectF player = getHitbox();
    if(r->getHealth()>0) {
        if(player.bottom() < roomba.bottom()) {
            r->setHealth(r->getHealth()-1);
            setJumpTime(getJumpTop() * constants::FPS_CALCULATION/2);
        }
        else {
            setHealth(getHealth()-1);
            setAnimPos(3);
        }
    } else {
        if(player.left() < roomba.left()) {
            r->setVectorX(0.05*constants::TILE_WIDTH);
        } else if(player.right() > roomba.right()) {
            r->setVectorX(-0.05*constants::TILE_WIDTH);
        }
    }
}



void Player::jump()
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

void Player::endTurn()
{
    setVectorX(getVectorX()*0.9);
    if(getVectorX() < 0.1 && getVectorX() > -0.1){
        setVectorX(0);
    }
    setVectorY(0);
}

