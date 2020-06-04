#ifndef OVERLAY_H
#define OVERLAY_H
#include<QVector>
#include "text.h"

class Overlay
{
private:
    QVector<Text> allTexts;
public:
    Overlay();
    void update();
    inline QVector<Text> getAllTexts() {return allTexts;};
};

#endif // OVERLAY_H
