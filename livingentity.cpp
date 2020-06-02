#include "livingentity.h"
#include "header.h"
#include <QDebug>



double LivingEntity::getHealth() const
{
    return health;
}

void LivingEntity::setHealth(double value)
{
    health = value;
}

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

QVector<LivingEntity *> LivingEntity::getCollidingEntities(double id, Level * const level)
{
    QVector<LivingEntity *> colliding;
    for(int i = 0; i<level->getNbEntities(); i++)
        if(i != id) {
            LivingEntity * entity = level->getEntity(i);
            if(entity->getHitbox().intersects(getHitbox())) {
                colliding.push_back(entity);
            }
        }
    return colliding;
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
    pos.setLeft(posTmp.left()-(4.0/16.0)*constants::TILE_WIDTH);
    pos.setWidth(constants::TILE_WIDTH);
    setImagePos(pos);
}
