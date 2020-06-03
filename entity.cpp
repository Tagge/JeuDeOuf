#include "entity.h"

#include <QMutex>
#include <QDebug>

bool Entity::getFacingBack() const
{
    return facingBack;
}

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

void Entity::setImagePos(QRectF value)
{
    //value.setLeft(value.left()-relativePosImage);
    imagePos = value;
}


