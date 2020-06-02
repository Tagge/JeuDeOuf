#ifndef SPRITE_H
#define SPRITE_H

#include <QPixmap>

class Sprite
{
private:
    int x;
    int y;
    int w;
    int h;
    QPixmap * pix;
public:
    Sprite();
    Sprite(int x, int y, int w, int h, QPixmap * const pix);
    inline int getX() const {return x;};
    inline void setX(int value) {x = value;};
    inline int getY() const {return y;};
    inline void setY(int value) {y = value;};
    inline int getW() const {return w;};
    inline void setW(int value) {w = value;};
    inline int getH() const {return h;};
    inline void setH(int value) {h = value;};
    inline QPixmap *getPix() const {return pix;};
    inline void setPix(QPixmap * const value) {pix = value;};
};

#endif // SPRITE_H
