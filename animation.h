#ifndef ANIMATION_H
#define ANIMATION_H

#include <QRect>
#include <QPixmap>
#include <QVector>



class Animation
{
private:
    QVector<QPixmap*> images;
    QVector<QPixmap*> * imagesReverse;
    unsigned int frameDuration;
    unsigned int iteration;
    unsigned int nbIteration;
    bool reversed = false;
public:
    Animation();
    inline QPixmap* getSprite() const {return images[iteration/frameDuration];};
    inline QPixmap* getReversedSprite() const {return (*imagesReverse)[iteration/frameDuration];};
    void addImage(QPixmap* pixmap);
    void iterate();
    void reverse();
    inline int getNbSprites() const {imagesReverse->size();};
};

#endif // ANIMATION_H
