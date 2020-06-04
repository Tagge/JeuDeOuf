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

int LivingEntity::getIntangible() const
{
    return intangible;
}

void LivingEntity::setIntangible(int value)
{
    intangible = value;
}

void LivingEntity::deathTimer()
{
    if(health < 0){
        health--;
    }
    if(health == -constants::FPS_CALCULATION/2){
        isDead = true;
    }
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

void LivingEntity::validatePos()
{
    setHitbox(posTmp);
    setImagePos(posTmp);
}
