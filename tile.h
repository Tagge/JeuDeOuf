#ifndef TILE_H
#define TILE_H

#include "animation.h"
#include "sprite.h"



class Tile
{
private:
    bool wall;
    QRect tileRect;
    Animation * animation;
public:
    Tile();
    Tile(bool wall, int x, int y, Animation * const anim);
    //Getters and setters
    inline bool getWall() const {return wall;};
    inline void setWall(bool value) {wall = value;};
    inline QRect getPos() const {return tileRect;};
    inline void setPos(QRect value) {tileRect = value;};
    inline QPixmap * getSprite() const {return animation->getSprite();};
    inline void setAnimation(Animation * const value) {animation = value;};
};

#endif // TILE_H
