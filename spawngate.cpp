#include "spawngate.h"

SpawnGate::SpawnGate()
{

}

SpawnGate::SpawnGate(int x, int y, const QMap<QString, Animation *> &animations)
{
    QRectF hitbox((x-0.75)*constants::TILE_WIDTH, (y-1)*constants::TILE_HEIGHT, constants::TILE_WIDTH*2, constants::TILE_HEIGHT*2);
    setImagePos(hitbox);
    setHitbox(hitbox);
    setHealth(-1);
    addAnimation(animations["spawn_gate"]);
}

void SpawnGate::deathTimer()
{
    if(getHealth() < 0){
        setHealth(getHealth()-1);
    }
    if(getHealth() == -4*constants::FPS_DRAW/12){
        setIsDead(true);
    }
}
