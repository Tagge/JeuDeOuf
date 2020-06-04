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
    value->reverse();
    animations.push_back(value);
}

void Entity::setImagePos(QRectF value)
{
    int width = value.width();
    int newLeft = value.left()-relativePosImage;
    value.setLeft(newLeft);
    value.setWidth(width+relativePosImage*2);
    imagePos = value;
}


