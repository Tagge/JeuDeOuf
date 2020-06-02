#include "livingentity.h"
#include "header.h"
#include <QDebug>

LivingEntity::LivingEntity():Entity()
{
    vectorX = 0;
    vectorY = 0;
}

bool LivingEntity::detectCollisionMap(Level * const level)
{
    int firstTileY = posTmp.left()/ constants::TILE_WIDTH;
    int lastTileY = (posTmp.right()-1)/ constants::TILE_WIDTH;
    int firstTileX = posTmp.top()/ constants::TILE_HEIGHT;
    int lastTileX = (posTmp.bottom()-1)/ constants::TILE_HEIGHT;
    bool collision = false;
    for(int tileX = firstTileX; tileX <= lastTileX; tileX++){
        for(int tileY = firstTileY; tileY <= lastTileY; tileY++){
            if(level->getTile(tileX, tileY)->getWall()){
                collision = true;
            }
        }
    }
    return collision;
}

void LivingEntity::move(Level * const level, QRect limit)
{
    QRectF pos = getHitbox();
    setPosTmp(pos.left()+(int)vectorX, pos.top());
    if(getPosTmp().left() < limit.left()){
        setPosTmp(limit.left(), posTmp.top());
    }
    if(getPosTmp().right() > limit.right()){
        setPosTmp(limit.right()-getHitbox().width(), posTmp.top());
    }
    bool collision = detectCollisionMap(level);
    int modifier = 1;
    if(vectorX > 0){
        modifier = -1;
    }
    while(collision){
        setPosTmp(posTmp.left()+modifier, pos.top());
        collision = detectCollisionMap(level);
    }
    setPosTmp(posTmp.left(), pos.top()+(int)vectorY);
    if(getPosTmp().top() < limit.top()){
        setPosTmp(posTmp.left(), limit.top());
    }
    if(getPosTmp().bottom() > limit.bottom()){
        setPosTmp(posTmp.left(), limit.bottom()-getHitbox().height());
    }
    collision = detectCollisionMap(level);
    modifier = 1;
    if(vectorY > 0){
        modifier = -1;
    }
    while(collision){
        setPosTmp(posTmp.left(), posTmp.top()+modifier);
        collision = detectCollisionMap(level);
    }
    validatePos();
}

void LivingEntity::validatePos()
{
    setHitbox(posTmp);
    QRectF pos = posTmp;
    pos.setLeft(posTmp.left()-12);
    pos.setWidth(constants::TILE_WIDTH);
    setImagePos(pos);
}

void LivingEntity::endTurn()
{
    vectorX *= 0.9;
    if(vectorX < 0.1 && vectorX > -0.1){
        vectorX = 0;
    }
    vectorY = 0;
}
