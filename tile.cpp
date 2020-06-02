#include "tile.h"
#include <QDebug>
#include "header.h"

Tile::Tile()
{

}

Tile::Tile(bool wall, int x, int y, Animation * const anim)
{
    setWall(wall);
    tileRect.setLeft(x*constants::TILE_WIDTH);
    tileRect.setTop(y*constants::TILE_HEIGHT);
    tileRect.setWidth(constants::TILE_WIDTH);
    tileRect.setHeight(constants::TILE_HEIGHT);
    setAnimation(anim);
}
