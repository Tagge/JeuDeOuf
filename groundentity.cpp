#include "groundentity.h"
#include "math.h"
#include "header.h"
#include <QDebug>

GroundEntity::GroundEntity(double jumpHeight, double jumpTime) : LivingEntity()
{
    fallingTime = 0;
    g = (-2 * jumpHeight)/pow(jumpTime, 2);
    gFast = (-2 * jumpHeight)/pow(jumpTime*0.9, 2);
    v0 = (2 * jumpHeight)/jumpTime;
}

bool GroundEntity::onGround(Level * const level)
{
    QRectF pos = getHitbox();
    setPosTmp(pos.left(), pos.top()+1);
    if(detectCollisionMap(level)){
        fallingTime = 0;
        return true;
    }
    else{
        fallingTime++;
        qDebug() << fallingTime;
        return false;
    }
}

void GroundEntity::fall()
{
    double y = getVectorY();
    double t_1 = ((double)fallingTime-1) / constants::FPS_CALCULATION;
    double t = (double) fallingTime / constants::FPS_CALCULATION;
    double posT = 0.5*gFast*pow(t, 2);
    double posT_1 = 0.5*gFast*pow(t_1, 2);
    setVectorY(y-(posT-posT_1));
}
