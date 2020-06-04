#include "player.h"
#include <QDebug>
#include "math.h"
#include "header.h"
#include "roomba.h"
#include "luckyblock.h"
#include "powerup.h"
#include <stdlib.h>

Player::Player():GroundEntity(0, 0)
{
    setJumpTime(0);
}

Player::Player(int x, int y, const QMap<QString, Animation *> &animations):GroundEntity(constants::TILE_HEIGHT*3.33, 0.3333)
{
    QRectF hitbox(x*constants::TILE_WIDTH, (y-1)*constants::TILE_HEIGHT, constants::TILE_WIDTH*0.5, constants::TILE_HEIGHT);
    setRelativePosImage(0.25);
    setImagePos(hitbox);
    setHitbox(hitbox);
    addAnimation(animations["character"]);
    addAnimation(animations["character_move"]);
    addAnimation(animations["character_jump"]);
    addAnimation(animations["character2"]);
    addAnimation(animations["character_move2"]);
    addAnimation(animations["character_jump2"]);
    addAnimation(animations["character_death"]);
    setAccel(getAccel()*constants::TILE_WIDTH/constants::FPS_CALCULATION);
    setMaxSpeed(getMaxSpeed()*constants::TILE_WIDTH/constants::FPS_CALCULATION);
    setJumpTime(0);
    livesLeft = 3;
}

void Player::update(Level * const level)
{
    if(getHealth() == -1) {
        setLivesLeft(getLivesLeft()-1);
        if(getLivesLeft() == 0) {
            qDebug("Game over");
            //doSomethingToEndTheGame
        }
        level->setXWindow(0);
        setPosTmp(96, 480);
        setHealth(0);
        validatePos();
        level->setTerminate(true);
        return;

    }
    if(getFallingTime() > 100 && getHealth() == 0) {
        level->setXWindow(0);
        setLivesLeft(getLivesLeft()-1);
        if(getLivesLeft() == 0) {
            qDebug("Game over");
            //doSomethingToEndTheGame
        }
        setPosTmp(96, 480);
        setHealth(0);
        validatePos();
        level->setTerminate(true);
        return;
    }
    if(underCeiling(level)){
        setJumpTime(getJumpTop() * constants::FPS_CALCULATION + 1);
    }
    if(onGround(level)){
        setJumpTime(0);
    }
    setAnimPos(getHealth()*3);
    if(getIntangible() > 0) {
        setIntangible((getIntangible()-1));
    }
    setAnimPos(getHealth()*3);
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
        setAnimPos(getHealth()*3+1);
        setFacingBack(true);
    }
    else if(getVectorX() > 0.9){
        setAnimPos(getHealth()*3+1);
        setFacingBack(false);
    }
    if(getJumpTime() > 0){
        setAnimPos(getHealth()*3+2);
        jump();
    }
    else if(getFallingTime() > 0){
        fall();
    }

    QRect limit(level->getXWindow(), 0, level->getNbCols()*constants::TILE_WIDTH-level->getXWindow(), level->getNbRows()*constants::TILE_HEIGHT);
    move(level, limit);
}

void Player::collide(LivingEntity *e, Level * const l)
{
    e->collide(this, l);
}

void Player::collide(Roomba *r, Level * const l)
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
                setHealth(getHealth()-1);
                setIntangible(constants::FPS_CALCULATION/6);
                healthChanged();
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
                    r->setIntangible(constants::FPS_CALCULATION/6);
                } else {
                    if(player.bottom() < roomba.bottom()) {
                        r->setVectorX(0);
                        setJumpTime(getJumpTop() * constants::FPS_CALCULATION/4);
                    }
                    else {
                        setHealth(getHealth()-1);
                        setIntangible(constants::FPS_CALCULATION/4);
                        healthChanged();
                    }
                }
            }
        }
    }
}

void Player::collide(LuckyBlock *lb, Level * const l)
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
        lb->dropItem(l);
    }
    validatePos();

}

void Player::collide(PowerUp * pu, Level * const l)
{
    if(pu->getIntangible()==0) {
        setHealth(1);
        healthChanged();
        pu->setHealth(-1);
        pu->setIntangible(constants::FPS_CALCULATION*3);
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

void Player::healthChanged()
{
    if(getHealth() == 0) {
        QRectF hitbox = getHitbox();
        hitbox.setTop(hitbox.top()+hitbox.height()/2);
        setHitbox(hitbox);
    }
    else if(getHealth() > 0) {
        QRectF hitbox = getHitbox();
        hitbox.setTop(hitbox.top()-hitbox.height());
        setHitbox(hitbox);
    }

}

