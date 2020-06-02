#include "entity.h"

#include <QMutex>
#include <QDebug>

Entity::Entity()
{
    animPos = 0;
    facingBack = false;
}

QPixmap *Entity::getSprite() const
{
    if(facingBack){
        return animations[animPos]->getReversedSprite();
    }
    else{
        return animations[animPos]->getSprite();
    }

}

void Entity::addAnimation(Animation *value)
{
    QMutex mutex;
    mutex.lock();
    value->reverse();
    mutex.unlock();

    animations.push_back(value);
}


