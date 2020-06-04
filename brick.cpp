#include "brick.h"
#include "header.h"
#include "player.h"
#include "roomba.h"
#include <QDebug>

Brick::Brick()
{

}

Brick::Brick(int x, int y, const QMap<QString, Animation *> &animations)
{
    QRectF hitbox(x*constants::TILE_WIDTH, y*constants::TILE_HEIGHT, constants::TILE_WIDTH, constants::TILE_HEIGHT);
    setImagePos(hitbox);
    setHitbox(hitbox);
    addAnimation(animations["brick"]);
    setAnimPos(0);
}

void Brick::update(Level * const level)
{
    if(movingFor == 6) {
        setVectorY(-getVectorY());
    }
    if(movingFor == 11) {
        setVectorY(0);
        movingFor = 0;
    }
    if(movingFor>0 && movingFor<11) {
        QRect limit(0, 0, level->getNbCols()*constants::TILE_WIDTH, level->getNbRows()*constants::TILE_HEIGHT);
        move(level, limit);
        movingFor++;
    }
}

void Brick::collide(LivingEntity *e, Level * const l)
{
    e->collide(this, l);
}

void Brick::collide(Roomba *r, Level * const l)
{
    if(r->getHealth() == 1 && r->getHitbox().bottom() == getHitbox().bottom()) {
        setHealth(-1);
        r->setFacingBack(!getFacingBack());
        r->setVectorX(-r->getVectorX());
    }
}

void Brick::collide(Player *p, Level * const l)
{

}

void Brick::collide(Brick *b, Level * const l)
{

}

void Brick::endTurn()
{

}

void Brick::move(Level * const level, QRect limit)
{
    QRectF pos = getHitbox();
    setPosTmp(pos.left(), pos.top()+getVectorY());
    validatePos();
}

void Brick::jump()
{
    if(movingFor == 0) {
        setVectorY(-0.05*constants::TILE_HEIGHT);
        movingFor=1;
    }
}
