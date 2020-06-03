#ifndef ENTITY_H
#define ENTITY_H

#include "animation.h"
#include "sprite.h"
#include "header.h"

class Player;
class Roomba;

class Entity
{
private:
    QVector<Animation *> animations;
    unsigned int animPos;
    QRectF imagePos;
    QRectF hitbox;
    bool facingBack;
    int relativePosImage = 0;
public:
    Entity();
    inline QRectF getHitbox() {return hitbox;};
    inline void setHitbox(QRectF value) {hitbox = value;};
    QPixmap * getSprite() const;
    void addAnimation(Animation * value);
    inline QRectF getImagePos() const {return imagePos;};
    void setImagePos(QRectF value);
    inline unsigned int getAnimPos() const {return animPos;};
    inline void setAnimPos(unsigned int value) {animPos = value;};
    inline void setFacingBack(bool value) {facingBack = value;};
    inline virtual void collide() {};
    bool getFacingBack() const;
    inline int getRelativePosImage() const {return relativePosImage;};
    inline void setRelativePosImage(double value) {relativePosImage = value*constants::TILE_WIDTH;};
};

#endif // ENTITY_H
