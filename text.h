#ifndef TEXT_H
#define TEXT_H
#include<QString>
#include<QPixmap>

class Text
{
private:
    QString sentence;
    int x;
    int y;
    QPixmap image;
public:
    Text();
    inline QString getSentence() {return sentence;};
    void setSentence();
    inline int getX() {return x;};
    inline void setX(int value) { x = value;};
    inline int getY() { return y;};
    inline void setY(int value) { y = value;};
    inline QPixmap getImage() {return image;};
};

#endif // TEXT_H
