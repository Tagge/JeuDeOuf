#include "overlay.h"
#include "level.h"
#include "player.h"

Overlay::Overlay()
{
    int id = QFontDatabase::addApplicationFont(":/fonts/SuperMario256.ttf");
    QString fontName = QFontDatabase::applicationFontFamilies(id).at(0);
    qDebug() << fontName;
    allTexts["health"] = Text("Vies : ♥♥♥", 40, 20, 30, fontName, "white");
    allTexts["level"] = Text("Niveau 1", 450, 20, 30, fontName, "white");
    allTexts["time"] = Text("60:00", 1000, 20, 30, fontName, "white");
}

void Overlay::update(Level * lvl) {
    int livesLeft = lvl->getPlayer()->getLivesLeft();
    int levelId = lvl->getLevelId();
    int timeLeft = lvl->getTimer()->getTimeLeft();
    QString livesLeftSentence = "Vies : ";
    for(int i = 0; i<livesLeft; i++) {
        livesLeftSentence.append("♥");
    }
    allTexts["health"].setSentence(livesLeftSentence);
    allTexts["level"].setSentence("Niveau "+QString::number(levelId));
    allTexts["time"].setSentence(QString::number(timeLeft));
}
