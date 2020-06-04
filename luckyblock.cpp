#include "luckyblock.h"
#include "header.h"
#include "roomba.h"
#include "player.h"
#include "powerup.h"
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
    addAnimation(animations["lucky_used"]);
    setHealth(1);
    objContained = obj;
}

void LuckyBlock::update(Level * const level)
{
    if(activatedFor == 6) {
        setVectorY(-getVectorY());
    }
    QRect limit(0, 0, level->getNbCols()*constants::TILE_WIDTH, level->getNbRows()*constants::TILE_HEIGHT);
    move(level, limit);
    if(activatedFor == 10) {
        setVectorY(0);
        setAnimPos(1);
    }
    if(activatedFor>0 && activatedFor<10) {
        activatedFor++;
    }
}

void LuckyBlock::collide(LivingEntity *e, Level * const l)
{
    e->collide(this, l);
}

void LuckyBlock::collide(Roomba *r, Level * const l)
{

}

void LuckyBlock::collide(Player *p, Level * const l)
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

void LuckyBlock::dropItem(Level * l)
{
    if(getHealth()>0) {
        setVectorY(-0.05*constants::TILE_HEIGHT);
        activatedFor=1;
        setHealth(0);
        if(objContained == 1) {
            PowerUp * pu = new PowerUp(getHitbox().left() + constants::TILE_WIDTH/4, getHitbox().top()-constants::TILE_HEIGHT/2, l->getAnimationMap()["bolt"]);
            l->addLivingEntity(pu);
        }
    }
}
