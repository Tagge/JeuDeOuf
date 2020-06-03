#ifndef LEVEL_H
#define LEVEL_H


#include "tile.h"
#include <QVector>
#include <QMap>
#include <QPixmap>

class LivingEntity;
class Player;

class Level
{
public:
    Level();
    Level(QString levelFileName);
    void addAnimationFromJson(const QJsonObject & object);
    void addAnimation(const QJsonArray & images, QString name);
    void createEntityFromJson(QString name, int x, int y);
    inline QMap<QString, Animation*> getAnimationMap() const {return animationsMap;};
    inline int getNbRows() const {return nbRows;};
    inline int getNbCols() const {return nbCols;};
    inline Tile * getTile(int x, int y) const {return map[x][y];};
    inline int getNbEntities() const {return livingEntities.size();}
    inline LivingEntity * getEntity(int pos) const {return livingEntities[pos];}
    inline bool getKey(int pos) const {return keys[pos];}
    inline void setKey(int pos, bool value) {keys[pos] = value;}
    inline Player * getPlayer() const {return player;};
    inline void setXWindow(int value) {xWindow = value;};
    inline void setYWindow(int value) {yWindow = value;};
    inline int getXWindow() const {return xWindow;};
    inline int getYWindow() const {return yWindow;};

private:
    QMap<QString, Animation*> animationsMap;
    QVector<QVector<Tile*>> map;
    QVector<LivingEntity*> livingEntities;
    int nbRows;
    int nbCols;
    QVector<bool> keys;
    Player * player;
    int xWindow;
    int yWindow;
};

#endif // LEVEL_H
