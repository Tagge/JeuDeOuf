#include "sprite.h"
#include <QPixmap>

Sprite::Sprite()
{
    x = 20;
    y = 50;
    w = 100;
    h = 100;
    pix = new QPixmap("C:/Users/leole/Documents/cours/s8/qt/test_jeu/resources/elias-wwe-2.jpg");
}

Sprite::Sprite(const int x, const int y, const int w, const int h, QPixmap * const pixmap)
{
    pix = new QPixmap();
    setX(x);
    setY(y);
    setW(w);
    setH(h);
    setPix(pix);
}
