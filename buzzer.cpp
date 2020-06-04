#include "buzzer.h"

Buzzer::Buzzer()
{

}

Buzzer::Buzzer(int startx, int starty, int endx, int endy, const QMap<QString, Animation *> &animations)
{
    this->startx = startx * constants::TILE_WIDTH;
    this->starty = starty * constants::TILE_HEIGHT;
    this->endx = endx * constants::TILE_WIDTH;
    this->endy = endy * constants::TILE_HEIGHT;
    QRectF hitbox(this->startx, this->starty, constants::TILE_WIDTH, constants::TILE_HEIGHT);
    setImagePos(hitbox);
    setHitbox(hitbox);
    addAnimation(animations["buzzer"]);
    addAnimation(animations["buzzer_death"]);
    setAnimPos(0);
    setFacingBack(true);
}

void Buzzer::update(Level * const level)
{
    if(started == false) {
        double xdif = startx - endx;
        double ydif = starty - endy;
        double ratiospeed = speed/(abs(xdif) + abs(ydif));
        setVectorX((-ratiospeed * xdif));
        setVectorY((-ratiospeed * ydif));
        started = true;
        turneds = true;
    } else {
        if (abs(getHitbox().left() - endx) < 5 && abs(getHitbox().top() - endy) < 5 && turnede == false){
            setVectorX(-getVectorX());
            setVectorY(-getVectorY());
            turnede = true;
            turneds = false;
        }
        else if (abs(getHitbox().left() - startx) < 5 && abs(getHitbox().top() - starty) < 5 && turneds == false) {
            setVectorX(-getVectorX());
            setVectorY(-getVectorY());
            turneds = true;
            turnede = false;
        }
    }
    QRect limit(level->getXWindow(), 0, level->getNbCols()*constants::TILE_WIDTH-level->getXWindow(), level->getNbRows()*constants::TILE_HEIGHT);
    move(level, limit);
}

void Buzzer::collide(LivingEntity *e, Level * const l)
{
    e->collide(this, l);
}

void Buzzer::collide(Player *p, Level * const l)
{

}

void Buzzer::endTurn()
{

}

void Buzzer::move(Level * const level, QRect limit)
{
    QRectF pos = getHitbox();
    setPosTmp(pos.left()+getVectorX(), pos.top()+getVectorY());
    validatePos();
}

void Buzzer::deathTimer()
{
    if(getHealth() < 0){
        setAnimPos(1);
        setHealth(getHealth()-1);
    }
    if(getHealth() == -constants::FPS_CALCULATION/2){
        setIsDead(true);
    }
}
