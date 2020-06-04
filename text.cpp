#include "text.h"

Text::Text()
{
    police = "Arial";
    size = 12;
}

Text::Text(QString sentence) {
    setSentence(sentence);
    police = "Arial";
    size = 12;
}

Text::Text(QString sentence, int x, int y, int size, QString police, QString color) {
    this->police = police;
    this->color = color;
    this->x = x;
    this->y = y;
    this->size = size;
    setSentence(sentence);
}

void Text::setSentence(QString sentence) {
    this->sentence = sentence;
    QFont ourFont(police, size);
    QFontMetrics measuresString(ourFont);
    int width = measuresString.width(sentence);
    int height = measuresString.height();
    this->height = height;
    this->width = width;
    QPixmap pictureText = QPixmap(width, height);
    pictureText.fill(Qt::transparent);
    QPainter paintText(&pictureText);
    paintText.setPen(QColor(color));
    paintText.setFont(ourFont);
    paintText.drawText(QPoint(0,height/1.35), sentence);
    image = pictureText;
    //generation image
}


