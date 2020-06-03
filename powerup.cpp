#include "powerup.h"

PowerUp::PowerUp():GroundEntity(0,0)
{

}

PowerUp::PowerUp(int x, int y, Animation * anim):GroundEntity(0,0)
{
    QRectF hitbox(x, y, constants::TILE_WIDTH/2, constants::TILE_HEIGHT/2);
    QRectF posImage(x, y, constants::TILE_WIDTH/2, constants::TILE_HEIGHT/2);
    setImagePos(posImage);
    setHitbox(hitbox);
    addAnimation(anim);
}

void PowerUp::update(Level * const level)
{

}

void PowerUp::collide(LivingEntity *e, Level * const l)
{
    e->collide(this, l);
}

void PowerUp::collide(Player *p, Level * const l)
{

}

void PowerUp::collide(LuckyBlock * lb, Level * const l)
{

}

void PowerUp::endTurn()
{

}

void PowerUp::move(Level * const level, QRect limit)
{

}
