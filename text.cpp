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

void Text::setSentence(QString sentence) {
    this->sentence = sentence;
    QFont ourFont(police, size);
    QFontMetrics measuresString(ourFont);
    int width = measuresString.width(sentence);
    int height = measuresString.height();
    QPixmap pictureText = QPixmap(width, height);
    QPainter paintText(&pictureText);
    paintText.setFont(ourFont);
    paintText.drawText(QPoint(0,0), sentence);
    image = pictureText;
    //generation image
}


