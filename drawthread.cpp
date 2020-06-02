#include "drawthread.h"
#include "header.h"
#include <QDebug>

DrawThread::DrawThread()
{

}

void DrawThread::run()
{
    QTimer timer;
    timer.setTimerType(Qt::PreciseTimer);
    connect(&timer, SIGNAL(timeout()), this, SLOT(doDraw()));
    double frame = 1000/constants::FPS_DRAW;
    timer.start(frame);
    exec();
}

void DrawThread::doDraw()
{
    game->repaint();
}
