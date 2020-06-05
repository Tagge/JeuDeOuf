#include "gamewindow.h"
#include "player.h"
#include "ui_gamewindow.h"
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
    connect(ui->buttonLevel1, SIGNAL(clicked()), this, SLOT(createGame1()));
    connect(ui->buttonLevel2, SIGNAL(clicked()), this, SLOT(createGame2()));
    connect(ui->buttonLevel3, SIGNAL(clicked()), this, SLOT(createGame3()));
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::createGame()
{
    ui->buttonLevel1->hide();
    ui->buttonLevel2->hide();
    ui->buttonLevel3->hide();
    lvl = new Level(":/levels/level2.json");
    levelPath = ":/levels/level2.json";
    int yWindow = lvl->getYWindow();
    lvl->setYWindow(yWindow-heightOrigin);
    inGame = 1;
    drawThread->start();
    QThread::msleep(5);
    calculateThread->start();
    setFocus();
}

void GameWindow::createGame1()
{
    ui->buttonLevel1->hide();
    ui->buttonLevel2->hide();
    ui->buttonLevel3->hide();
    lvl = new Level(":/levels/level1.json");
    levelPath = ":/levels/level1.json";
    int yWindow = lvl->getYWindow();
    lvl->setYWindow(yWindow-heightOrigin);
    inGame = 1;
    drawThread->start();
    QThread::msleep(5);
    calculateThread->start();
    setFocus();
}

void GameWindow::createGame2()
{
    ui->buttonLevel1->hide();
    ui->buttonLevel2->hide();
    ui->buttonLevel3->hide();
    lvl = new Level(":/levels/level2.json");
    levelPath = ":/levels/level2.json";
    int yWindow = lvl->getYWindow();
    lvl->setYWindow(yWindow-heightOrigin);
    inGame = 1;
    drawThread->start();
    QThread::msleep(5);
    calculateThread->start();
    setFocus();
}

void GameWindow::createGame3()
{
    ui->buttonLevel1->hide();
    ui->buttonLevel2->hide();
    ui->buttonLevel3->hide();
    lvl = new Level(":/levels/level3.json");
    levelPath = ":/levels/level3.json";
    int yWindow = lvl->getYWindow();
    lvl->setYWindow(yWindow-heightOrigin);
    inGame = 1;
    drawThread->start();
    QThread::msleep(5);
    calculateThread->start();
    setFocus();
}

void GameWindow::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    if(inGame){
        QMutex mutex;
        if(lvl->getTerminate() && lvl->getPlayer()->getLivesLeft() != 0) {
            mutex.try_lock();
            std::chrono::time_point<std::chrono::steady_clock> begin = lvl->getBeginDate();
            int livesLeft = lvl->getPlayer()->getLivesLeft();
            bool checkpoint = lvl->getCheckpoint()->isChecked();
            delete(lvl);
            lvl = new Level(getLevelPath(), livesLeft, begin, checkpoint);
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
        if(lvl->getWin()) {
            if(overCount < 300) {
                Text gg("GG", (widthOrigin/2)*ratioWidth, (heightOrigin/2)*ratioHeight, 40, "Super Mario 256", "red");
                painter.drawPixmap(gg.getX(), gg.getY(), gg.getWidth()*ratioWidth, gg.getHeight()*ratioHeight, gg.getImage());
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
        ui->buttonLevel1->show();
        ui->buttonLevel2->show();
        ui->buttonLevel3->show();
        QPixmap pix(":/sprites/bg_menu");
        painter.drawPixmap(0, 0, 1920, 1080, pix);
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

