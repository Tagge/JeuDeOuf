#include "luckyblock.h"
#include "header.h"
#include "roomba.h"
#include "player.h"
#include <QDebug>


LuckyBlock::LuckyBlock()
{

}

LuckyBlock::LuckyBlock(int x, int y, const QMap<QString, Animation *> &animations, int obj)
{
    QRectF hitbox(x*constants::TILE_WIDTH, y*constants::TILE_HEIGHT, constants::TILE_WIDTH, constants::TILE_HEIGHT);
    QRectF posImage(x*constants::TILE_WIDTH, y*constants::TILE_HEIGHT, constants::TILE_WIDTH, constants::TILE_HEIGHT);
    setImagePos(posImage);
    setHitbox(hitbox);
    addAnimation(animations["lucky"]);
    setHealth(1);
    objContained = obj;
}

void LuckyBlock::update(Level * const level)
{
    if(activatedFor == 5) {
        setVectorY(-getVectorY());
    }
    if(activatedFor == 10) {
        setVectorY(0);
    }
    if(activatedFor>0 && activatedFor<10) {
        activatedFor++;
    }
    QRect limit(0, 0, level->getNbCols()*constants::TILE_WIDTH, level->getNbRows()*constants::TILE_HEIGHT);
    move(level, limit);
}

void LuckyBlock::collide(LivingEntity *e)
{
    e->collide(this);
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
    QRectF pos = getHitbox();
    setPosTmp(pos.left(), pos.top()+getVectorY());
    validatePos();
}

void LuckyBlock::dropItem()
{
    if(getHealth()>0) {
        setVectorY(-0.05*constants::TILE_HEIGHT);
        activatedFor=1;
        setHealth(0);
    }
}
