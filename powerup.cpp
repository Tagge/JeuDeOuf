#include "powerup.h"

PowerUp::PowerUp()
{

}

PowerUp::PowerUp(int x, int y, Animation * anim)
{
    QRectF hitbox(x, y, constants::TILE_WIDTH/2, constants::TILE_HEIGHT/2);
    QRectF posImage(x, y, constants::TILE_WIDTH/2, constants::TILE_HEIGHT/2);
    setImagePos(posImage);
    setHitbox(hitbox);
    addAnimation(anim);
}

void PowerUp::update(Level * const level)
{
    if(getIntangible() > 0) {
        setIntangible((getIntangible()-1));
    }
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
