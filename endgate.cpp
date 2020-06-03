#include "endgate.h"
#include "player.h"
#include <QDebug>

EndGate::EndGate()
{

}

EndGate::EndGate(int x, int y, const QMap<QString, Animation *> &animations)
{
    QRectF hitbox(x*constants::TILE_WIDTH, (y-2)*constants::TILE_HEIGHT, constants::TILE_WIDTH, constants::TILE_HEIGHT*3);
    setRelativePosImage(1);
    setImagePos(hitbox);
    setHitbox(hitbox);
    addAnimation(animations["end_gate"]);
}

void EndGate::update(Level * const level){}

void EndGate::collide(LivingEntity *e, Level * const l)
{

}

void EndGate::collide(Player *p, Level *l)
{
    l->setWin(true);
    p->setAnimPos(getHealth()*3+2);
}

void EndGate::move(Level * const level, QRect limit){}

void EndGate::endTurn(){}
