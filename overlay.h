#ifndef OVERLAY_H
#define OVERLAY_H
#include<QVector>
#include<QMap>
#include<QFontDatabase>
#include "text.h"
#include "leveltimer.h"

class Level;

class Overlay
{
private:
    QMap<QString, Text> allTexts;
public:
    Overlay();
    void update(Level * lvl);
    inline QMap<QString, Text> getAllTexts() {return allTexts;};
};

#endif // OVERLAY_H
