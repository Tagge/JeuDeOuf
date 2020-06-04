#include "animation.h"
#include "header.h"

Animation::Animation()
{
    iteration = 0;
    nbIteration = 0;
    frameDuration = constants::FPS_DRAW/12;
    imagesReverse = &images;
}

Animation::~Animation() {
    for(int i = 0; i<images.size(); i++) {
        delete(images[i]);
    }
    /*if(imagesReverse != nullptr) {
        for(int i = 0; i<imagesReverse->size(); i++) {
            delete(imagesReverse->at(i));
        }
        delete(imagesReverse);
    }*/
}

void Animation::addImage(QPixmap *pixmap)
{
    images.push_back(pixmap);
    nbIteration++;
}

void Animation::iterate()
{
    iteration++;
    if(iteration == nbIteration*frameDuration){
        iteration = 0;
    }
}

void Animation::reverse()
{
    if(!reversed){
        imagesReverse = new QVector<QPixmap*>();
        for(unsigned int pic = 0; pic < nbIteration; pic++){
            QPixmap * pixmap = new QPixmap();
            *pixmap = images[pic]->transformed(QTransform().scale(-1, 1));
            imagesReverse->push_back(pixmap);
        }
        reversed = true;
    }
}
