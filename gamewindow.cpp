#include "gamewindow.h"
#include "player.h"
#include "ui_gamewindow.h"
#include "leveltimer.h"
#include <QDebug>
#include <QMutex>
#include <QPainter>
#include <QPixmap>
#include <QKeyEvent>
#include "checkpoint.h"

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
    overlay = Overlay();
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
    levelPath = ":/levels/level_test.json";
    int yWindow = lvl->getYWindow();
    lvl->setYWindow(yWindow-heightOrigin);
    inGame = 1;
    drawThread->start();
    QThread::msleep(5);
    calculateThread->start();
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
    QPainter painter(this);
    if(inGame){
        QMutex mutex;
        if(lvl->getTerminate() && lvl->getPlayer()->getLivesLeft() != 0) {
            mutex.try_lock();
            LevelTimer * timer = lvl->getTimer();
            int livesLeft = lvl->getPlayer()->getLivesLeft();
            bool checkpoint = lvl->getCheckpoint()->isChecked();
            delete(lvl);
            lvl = new Level(getLevelPath(), livesLeft, timer, checkpoint);
            timer->setLvl(lvl);
            int yWindow = lvl->getYWindow();
            lvl->setYWindow(yWindow-getHeightOrigin());
            mutex.unlock();
        }
        double ratioWidth = width()/widthOrigin;
        double ratioHeight = height()/heightOrigin;
        //Paint the map
        int nbRows = lvl->getNbRows();
        int nbCols = lvl->getNbCols();
        for(int row = 0; row < nbRows; row++){
            for(int col = 0; col < nbCols; col++){
                mutex.try_lock();
                Tile * tile = lvl->getTile(row, col);
                QRect rect = tile->getPos();
                int x  = (rect.left()-lvl->getXWindow())*ratioWidth;
                int y = (rect.top()-lvl->getYWindow())*ratioHeight;
                painter.drawPixmap(x, y, rect.width()*ratioWidth+1, rect.height()*ratioHeight+1, *tile->getSprite());
                mutex.unlock();
            }
        }
        if(lvl->getTerminate() && lvl->getPlayer()->getLivesLeft() == 0) {
            if(overCount < 300) {
                Text gameOver("Game Over", (widthOrigin/3)*ratioWidth, (heightOrigin/2)*ratioHeight, 40, "Super Mario 256", "red");
                painter.drawPixmap(gameOver.getX(), gameOver.getY(), gameOver.getWidth()*ratioWidth, gameOver.getHeight()*ratioHeight, gameOver.getImage());
                overCount++;
                return;
            } else {
                delete(lvl);
                inGame = false;
                overCount = 0;
                return;
            }
        }
        //Paint the entities
        mutex.try_lock();
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
        //Paint the overlay
        mutex.try_lock();
        overlay.update(lvl);
        for(auto & x : overlay.getAllTexts()) {
            int topLeftX = x.getX() * ratioWidth;
            int topLeftY = x.getY() * ratioHeight;
            int height = x.getHeight() * ratioHeight;
            int width = x.getWidth() * ratioWidth;
            painter.drawPixmap(topLeftX, topLeftY, width, height, x.getImage());
        }
        mutex.unlock();

    }
    else{
        ui->levelTest->show();
        QPixmap pix(":/sprites/bg_menu");
        int y = heightOrigin-135*constants::TILE_HEIGHT/16.0;
        painter.drawPixmap(0, y, 240*constants::TILE_WIDTH/16.0, 135*constants::TILE_HEIGHT/16.0, pix);
    }
}

void GameWindow::keyPressEvent(QKeyEvent *event)
{
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

