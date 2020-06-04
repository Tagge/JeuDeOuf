#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "calculatethread.h"
#include "drawthread.h"
#include "level.h"
#include "sprite.h"

#include <QMainWindow>
#include <QTimer>

class DrawThread;
class CalculateThread;

QT_BEGIN_NAMESPACE
namespace Ui { class GameWindow; }
QT_END_NAMESPACE

class GameWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::GameWindow *ui;
    Level *lvl;
    bool inGame = 0;
    double widthOrigin;
    double heightOrigin;
    DrawThread * drawThread;
    CalculateThread * calculateThread;
    QTimer timer;
    QString levelPath;

public:
    GameWindow(QWidget *parent = nullptr);
    ~GameWindow();
    //Events
    void paintEvent(QPaintEvent* e);
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent *event);
    //Getters and setters
    inline Level *getLvl() const {return lvl;};
    inline double getWidthOrigin() const {return widthOrigin;};
    void calculate();

private slots:
    void createGame();
    void gameLoop();
};
#endif // GAMEWINDOW_H
