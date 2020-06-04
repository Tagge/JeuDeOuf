#include "checkpoint.h"

Checkpoint::Checkpoint()
{

}

Checkpoint::Checkpoint(int x, int y, const QMap<QString, Animation *> &animations)
{
    xCheckpoint = x*constants::TILE_WIDTH;
    QRectF hitbox(xCheckpoint, y*constants::TILE_HEIGHT, constants::TILE_WIDTH, constants::TILE_HEIGHT);
    setImagePos(hitbox);
    setHitbox(hitbox);
    setHealth(0);
    addAnimation(animations["checkpoint_off"]);
    addAnimation(animations["checkpoint_on"]);
}
