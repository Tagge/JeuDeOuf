#include "spawngate.h"

SpawnGate::SpawnGate()
{

}

SpawnGate::SpawnGate(int x, int y, const QMap<QString, Animation *> &animations)
{
    QRectF hitbox((x-0.75)*constants::TILE_WIDTH, (y-1)*constants::TILE_HEIGHT, constants::TILE_WIDTH*2, constants::TILE_HEIGHT*2);
    setImagePos(hitbox);
    setHitbox(hitbox);
    addAnimation(animations["spawn_gate"]);
}
