#ifndef TEXT_H
#define TEXT_H
#include<QString>
#include<QPixmap>
#include<QFont>
#include<QFontMetrics>
#include<QPainter>

class Text
{
private:
    QString sentence;
    int x;
    int y;
    int size;
    QString police;
    QPixmap image;
public:
    Text();
    Text(QString sentence);
    inline QString getSentence() {return sentence;};
    void setSentence(QString sentence);
    inline int getX() {return x;};
    inline void setX(int value) { x = value;};
    inline int getY() { return y;};
    inline void setY(int value) { y = value;};
    inline QPixmap getImage() {return image;};
    inline int getSize() {return size;};
    inline void setSize(int value) {size=value;};
    inline QString getPolice() {return police;};
    inline void setPolice(QString value) {police=value;};
};

#endif // TEXT_H
