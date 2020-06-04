#ifndef LEVEL_H
#define LEVEL_H
#include "tile.h"
#include <QVector>
#include <QMap>
#include <QPixmap>


class LivingEntity;
class Player;
class Checkpoint;
class LevelTimer;

class Level
{
public:
    Level();
    ~Level();
    Level(QString levelFileName);
    Level(QString levelFileName, int livesLeft);
    Level(QString levelFileName, int livesLeft, LevelTimer * timer, bool check = false);
    void addAnimationFromJson(const QJsonObject & object);
    void addAnimation(const QJsonArray & images, QString name);
    void createEntityFromJson(QString name, int x, int y, const QJsonObject & param);
    inline QMap<QString, Animation*> getAnimationMap() const {return animationsMap;};
    inline int getNbRows() const {return nbRows;};
    inline int getNbCols() const {return nbCols;};
    inline Tile * getTile(int x, int y) const {return map[x][y];};
    inline int getNbEntities() const {return livingEntities.size();}
    inline LivingEntity * getEntity(int pos) const {return livingEntities[pos];}
    inline void removeEntity(int pos) {livingEntities.remove(pos);};
    inline bool getKey(int pos) const {return keys[pos];}
    inline void setKey(int pos, bool value) {keys[pos] = value;}
    inline Player * getPlayer() const {return player;};
    inline void setXWindow(int value) {xWindow = value;};
    inline void setYWindow(int value) {yWindow = value;};
    inline int getXWindow() const {return xWindow;};
    inline int getYWindow() const {return yWindow;};
    inline bool getWin() const {return win;};
    inline void setWin(bool value) {win = value;};
    inline void addLivingEntity(LivingEntity * le) {livingEntities.push_back(le);};
    inline bool getTimeElapsed() {return timeElapsed;};
    void setTimeElapsed(bool value);
    inline int getDuration() { return duration;};
    inline void setDuration(int value) {duration = value;};
    inline bool getTerminate() {return terminate;};
    inline void setTerminate(bool value) {terminate = value;};
    inline LevelTimer * getTimer() {return lvlTimer;};
    inline void setLevelId(int value) {levelId = value;};
    inline int getLevelId() {return levelId;};
    inline Checkpoint * getCheckpoint() const {return checkpoint;};

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
    bool timeElapsed = false;
    int duration;
    bool win = false;
    LevelTimer * lvlTimer;
    bool terminate = false;
    int levelId;
    Checkpoint * checkpoint;
};

#endif // LEVEL_H
