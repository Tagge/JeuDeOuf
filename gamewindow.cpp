#include "gamewindow.h"
#include "player.h"
#include "ui_gamewindow.h"
#include <QDebug>
#include <QMutex>
#include <QPainter>
#include <QPixmap>
#include <QKeyEvent>

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GameWindow)
{
    ui->setupUi(this);
    inGame = 0;
    widthOrigin = width();
    heightOrigin = height();
    drawThread = new DrawThread();
    drawThread->setGame(this);
    calculateThread = new CalculateThread();
    calculateThread->setGame(this);
    connect(ui->levelTest, SIGNAL(clicked()), this, SLOT(createGame()));
    /*timer.setTimerType(Qt::PreciseTimer);
    connect(&timer, SIGNAL(timeout()), this, SLOT(gameLoop()));
    timer.start(16);*/
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::createGame()
{
    ui->levelTest->hide();
    lvl = new Level(":/levels/level_test.json");
    inGame = 1;
    drawThread->start();
    QThread::msleep(5);
    //calculateThread->start();
    setFocus();
}

//En cas de non Thread
void GameWindow::gameLoop()
{
    if(inGame){
        calculate();
        repaint();
    }
}

void GameWindow::paintEvent(QPaintEvent *e)
{
    if(inGame){
        QMutex mutex;
        double ratioWidth = width()/widthOrigin;
        double ratioHeight = height()/heightOrigin;
        QPainter painter(this);
        //Paint the map
        int nbRows = lvl->getNbRows();
        int nbCols = lvl->getNbCols();
        for(int row = 0; row < nbRows; row++){
            for(int col = 0; col < nbCols; col++){
                mutex.lock();
                Tile * tile = lvl->getTile(row, col);
                QRect rect = tile->getPos();
                int x  = (rect.left()-lvl->getXWindow())*ratioWidth;
                int y = (rect.top()-lvl->getYWindow())*ratioHeight;
                painter.drawPixmap(x, y, rect.width()*ratioWidth+1, rect.height()*ratioHeight+1, *tile->getSprite());
                mutex.unlock();
            }
        }
        //Paint the entities
        mutex.lock();
        int size = lvl->getNbEntities();
        for(int idEntity = 0; idEntity < size; idEntity++){
            LivingEntity * entity = lvl->getEntity(idEntity);
            QRectF rect = entity->getImagePos();
            int x = (rect.left()-lvl->getXWindow())*ratioWidth;
            int y = (rect.top()-lvl->getYWindow())*ratioHeight;
            painter.drawPixmap(x, y, rect.width()*ratioWidth+1, rect.height()*ratioHeight+1, *entity->getSprite());
        }
        mutex.unlock();
        for(Animation * animation : lvl->getAnimationMap()){
            animation->iterate();
        }
    }
}

void GameWindow::keyPressEvent(QKeyEvent *event)
{
    QMutex mutex;
    //mutex.lock();
    if(inGame){
        if(event->key() == Qt::Key_Left){
            lvl->setKey(0, true);
        }
        if(event->key() == Qt::Key_Right){
            lvl->setKey(1, true);
        }
        if(event->key() == Qt::Key_Space){
            lvl->setKey(2, true);
        }
    }

    //mutex.unlock();
}

void GameWindow::keyReleaseEvent(QKeyEvent * event)
{
    if(inGame){
        if(event->key() == Qt::Key_Left){
            lvl->setKey(0, false);
        }
        if(event->key() == Qt::Key_Right){
            lvl->setKey(1, false);
        }
        if(event->key() == Qt::Key_Space){
            lvl->setKey(2, false);
        }
    }
}

//En cas de non threads
void GameWindow::calculate()
{
    int size = lvl->getNbEntities();
    for(int idEntity = 0; idEntity < size; idEntity++){
        lvl->getEntity(idEntity)->update(lvl);
        lvl->setKey(0, false);
        lvl->setKey(1, false);
        lvl->setKey(2, false);
    }
}

