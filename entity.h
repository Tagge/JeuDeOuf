#ifndef ENTITY_H
#define ENTITY_H

#include "animation.h"
#include "sprite.h"



class Entity
{
private:
    QVector<Animation *> animations;
    unsigned int animPos;
    QRectF imagePos;
    QRectF hitbox;
    bool facingBack;
public:
    Entity();
    inline QRectF getHitbox() {return hitbox;};
    inline void setHitbox(QRectF value) {hitbox = value;};
    QPixmap * getSprite() const;
    void addAnimation(Animation * value);
    inline QRectF getImagePos() const {return imagePos;};
    inline void setImagePos(QRectF value) {imagePos = value;};
    inline unsigned int getAnimPos() const {return animPos;};
    inline void setAnimPos(unsigned int value) {animPos = value;};
    inline void setFacingBack(bool value) {facingBack = value;};
};

#endif // ENTITY_H
