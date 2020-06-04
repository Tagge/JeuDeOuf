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
    if(getIntangible() > 0) {
        setIntangible((getIntangible()-1));
    }
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
    if(getIntangible() == 0) {
        if(r->getHitbox().bottom() == getHitbox().bottom()) {
            if(r->getHealth() == 0) {
                setHealth(-1);
                jump();
            }
            r->setFacingBack(!r->getFacingBack());
            r->setVectorX(-r->getVectorX());
            setIntangible(10);
        }
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

void Brick::deathTimer()
{
    if(getHealth() < 0){
        setHealth(getHealth()-1);
    }
    if(getHealth() == -constants::FPS_CALCULATION/6){
        setIsDead(true);
    }
}
