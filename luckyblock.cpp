#include "luckyblock.h"
#include "header.h"

LuckyBlock::LuckyBlock()
{

}

LuckyBlock::LuckyBlock(int x, int y, const QMap<QString, Animation *> &animations, int obj)
{
    QRectF hitbox(x*constants::TILE_WIDTH, y*constants::TILE_HEIGHT, constants::TILE_WIDTH*8/16, constants::TILE_HEIGHT);
    QRectF posImage(x*constants::TILE_WIDTH, y*constants::TILE_HEIGHT, constants::TILE_WIDTH, constants::TILE_HEIGHT);
    setImagePos(posImage);
    setHitbox(hitbox);
    addAnimation(animations["lucky"]);
    objContained = obj;
}

void LuckyBlock::update(Level * const level)
{

}

void LuckyBlock::collide(LivingEntity *e)
{

}

void LuckyBlock::collide(Roomba *r)
{

}

void LuckyBlock::collide(Player *p)
{

}

void LuckyBlock::endTurn()
{

}

void LuckyBlock::move(Level * const level, QRect limit)
{

}
