#include "player.h"
#include <QDebug>
#include "math.h"
#include "header.h"
#include "roomba.h"
#include "luckyblock.h"
#include <stdlib.h>

Player::Player():GroundEntity(0, 0)
{
    setJumpTime(0);
}

Player::Player(int x, int y, const QMap<QString, Animation *> &animations):GroundEntity(constants::TILE_HEIGHT*3.33, 0.3333)
{
    QRectF hitbox(x*constants::TILE_WIDTH, y*constants::TILE_HEIGHT, constants::TILE_WIDTH*8/16, constants::TILE_HEIGHT);
    setRelativePosImage(0.25);
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
    if(getIntangible() > 0) {
        setIntangible((getIntangible()+1)%constants::INTANGIBLE_FRAMES);
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
    if(getIntangible() == 0) {
        QRectF roomba = r->getHitbox();
        QRectF player = getHitbox();
        if(r->getHealth()>0) {
            if(player.bottom() < roomba.bottom()) {
                r->setHealth(r->getHealth()-1);
                r->setVectorX(0);
                setJumpTime(getJumpTop() * constants::FPS_CALCULATION/4);
            }
            else {
                qDebug() << "touché";
                setHealth(getHealth()-1);
                setAnimPos(3);
                setIntangible(1);
            }
        } else {
            if(r->getIntangible() == 0) {
                if(r->getVectorX() == 0) {
                    if(player.bottom() < roomba.bottom()) {
                        setJumpTime(getJumpTop() * constants::FPS_CALCULATION/4);
                    }
                    if((player.left()+player.right())/2 < (roomba.left()+roomba.right())/2) {
                        r->setVectorX(0.15*constants::TILE_WIDTH);
                    } else {
                        r->setVectorX(-0.15*constants::TILE_WIDTH);
                    }
                } else {
                    if(player.bottom() < roomba.bottom()) {
                        r->setVectorX(0);
                        setJumpTime(getJumpTop() * constants::FPS_CALCULATION/4);
                    }
                    else {
                        qDebug() << "touché";
                        setHealth(getHealth()-1);
                        setAnimPos(3);
                        setIntangible(1);
                    }
                }
            }
        }
    }
}

void Player::collide(LuckyBlock *lb)
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
        qDebug() << "touched from left";
        qDebug() << leftdir << topdir << rightdir << botdir;
    } else if (topdir <= leftdir && topdir <= rightdir && topdir <= botdir){
        direction = 2;
        qDebug() << "touched from top";
        qDebug() << leftdir << topdir << rightdir << botdir;
    } else if (rightdir <= leftdir && rightdir <= topdir && rightdir <= botdir) {
        direction = 3;
        qDebug() << "touched from right";
        qDebug() << leftdir << topdir << rightdir << botdir;
    } else {
        direction = 4;
        qDebug() << "touched from bot";
        qDebug() << leftdir << topdir << rightdir << botdir;
    }

    //Gauche
    if(direction == 1) {
        setPosTmp(lucky.left()-getHitbox().width(),getPosTmp().top());
    }
    //Haut
    else if (direction == 2) {
        setPosTmp(getPosTmp().left(), lucky.top()-getHitbox().height());
        setOnSolid(true);
    }
    //Droite
    else if (direction == 3) {
        setPosTmp(lucky.right(), getPosTmp().top());
    }
    //Bas
    else {
        setPosTmp(getPosTmp().left(), lucky.bottom());
        setJumpTime(getJumpTop() * constants::FPS_CALCULATION + 1);
        lb->dropItem();
    }
    validatePos();

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

void Player::move(Level * const level, QRect limit)
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

