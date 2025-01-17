#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "calculatethread.h"
#include "drawthread.h"
#include "level.h"
#include "sprite.h"

#include <QMainWindow>
#include <QTimer>
#include <QSound>
#include "overlay.h"

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
    Overlay overlay;
    int overCount = 0;
    QSound * soundManager;
public:
    GameWindow(QWidget *parent = nullptr);
    ~GameWindow();
    //Events
    void paintEvent(QPaintEvent* e);
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent *event);
    //Getters and setters
    inline Level *getLvl() const {return lvl;};
    inline void setLevel(Level * value) {lvl = value;};
    inline double getWidthOrigin() const {return widthOrigin;};
    inline double getHeightOrigin() const {return heightOrigin;};
    inline QString getLevelPath() const {return levelPath;};
    inline Overlay getOverlay() {return overlay;};
    inline bool getIngame() {return inGame;};

private slots:
    void createGame();
    void createGame1();
    void createGame2();
    void createGame3();
};
#endif // GAMEWINDOW_H
