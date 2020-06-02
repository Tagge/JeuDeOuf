#include "player.h"
#include <QDebug>
#include "math.h"
#include "header.h"

Player::Player():GroundEntity(0, 0)
{
    jumpTime = 0;
}

Player::Player(int x, int y, const QMap<QString, Animation *> &animations):GroundEntity(constants::TILE_HEIGHT*3.33, 0.3333)
{
    QRectF hitbox(x*constants::TILE_WIDTH, y*constants::TILE_HEIGHT, constants::TILE_WIDTH*8/16, constants::TILE_HEIGHT);
    double xTmp = (x-4.0/16.0)*constants::TILE_WIDTH;
    qDebug() << (x*constants::TILE_WIDTH);
    qDebug() << xTmp;
    QRectF posImage(xTmp, y*constants::TILE_HEIGHT, constants::TILE_WIDTH, constants::TILE_HEIGHT);
    setImagePos(posImage);
    setHitbox(hitbox);
    addAnimation(animations["character"]);
    addAnimation(animations["character_move"]);
    addAnimation(animations["character_jump"]);
    acceleration = acceleration*constants::TILE_WIDTH/constants::FPS_CALCULATION;
    maxSpeed = maxSpeed*constants::TILE_WIDTH/constants::FPS_CALCULATION;
    jumpTime = 0;
}

void Player::update(Level * const level)
{
    if(onGround(level)){
        jumpTime = 0;
    }
    if(underCeiling(level)){
        jumpTime = jumpTop * constants::FPS_CALCULATION + 1;
    }
    setAnimPos(0);
    if(level->getKey(0)){
        double x = getVectorX();
        setVectorX(x-acceleration);
    }
    if(level->getKey(1)){
        double x = getVectorX();
        setVectorX(x+acceleration);
    }
    if(level->getKey(2) && jumpTime == 0 && getFallingTime() == 0){
        jumpTime = 1;
    }
    if(getVectorX() > maxSpeed){
        setVectorX(maxSpeed);
    }
    if(getVectorX() < -maxSpeed){
        setVectorX(-maxSpeed);
    }
    if(getVectorX() < -0.9){
        setAnimPos(1);
        setFacingBack(true);
    }
    else if(getVectorX() > 0.9){
        setAnimPos(1);
        setFacingBack(false);
    }
    if(jumpTime > 0){
        setAnimPos(2);
        jump();
    }
    else if(getFallingTime() >  0){
        fall();
    }
    QRect limit(level->getXWindow(), 0, level->getNbCols()*constants::TILE_WIDTH-level->getXWindow(), level->getNbRows()*constants::TILE_HEIGHT);
    move(level, limit);
}

void Player::jump()
{
    double y = getVectorY();
    double t_1 = (jumpTime-1) / constants::FPS_CALCULATION;
    double t = jumpTime / constants::FPS_CALCULATION;
    double g = getG();
    if(t >= jumpTop){
        g = getGFast();
    }
    double posT = 0.5*g*pow(t, 2)+getV0()*t;
    double posT_1 = 0.5*g*pow(t_1, 2)+getV0()*t_1;
    setVectorY(y-(posT-posT_1));
    jumpTime++;
}

