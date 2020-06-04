#ifndef CHECKPOINT_H
#define CHECKPOINT_H

#include "livingentity.h"

class Checkpoint: public LivingEntity
{
private:
    int xCheckpoint;
    bool checked = false;
public:
    Checkpoint();
    Checkpoint(int x, int y, const QMap<QString, Animation*> & animations);
    virtual void update(Level * const level) {};
    void move(Level * const level, QRect limit) {};
    void endTurn(){};
    inline int getXCheckpoint() {return xCheckpoint;};
    inline bool isChecked() const {return checked;};
    inline void setChecked(bool value) {checked = true;};
};

#endif // CHECKPOINT_H
