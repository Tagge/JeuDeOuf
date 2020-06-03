#include "level.h"
#include "player.h"
#include "roomba.h"
#include "luckyblock.h"
#include "header.h"
#include <QDebug>


//Mock level
Level::Level()
{   
    int nbPix = 10;
    nbRows = 10;
    nbCols = 100;

    xWindow = 0;
    yWindow = nbRows*constants::TILE_HEIGHT-8*constants::TILE_HEIGHT;
    //yWindow = 0;

    QVector<QVector<int>> mapInt(nbRows);
    for(int row = 0; row < nbRows; row++){
        map.push_back(QVector<Tile*>(nbCols));
    }
    //Design de la map
    QString animationName[] = {"air", "ground", "block", "character", "character_move", "character_jump", "character_death", "roomba", "roomba_death", "lucky"};
    QVector<QVector<QString>> fileName  = {{":/sprites/air.png"}, {":/sprites/Ground.png"}, {":/sprites/Block.png"}, {":/sprites/character/Character_0004.png"}, {":/sprites/character/Character_0001.png", ":/sprites/character/Character_0002.png", ":/sprites/character/Character_0003.png", ":/sprites/character/Character_0002.png"}, {":/sprites/character/Character_0000.png"}, {":/sprites/character/Character_0005.png"}, {":/sprites/enemies/Roomba_0", ":/sprites/enemies/Roomba_1"}, {":/sprites/enemies/DeathRoomba"}, {":/sprites/PowerBrick"}};
    bool pixIsWall[] = {0, 1, 1, 0, 0};
    for(int row = 0; row < 7; row++){
        mapInt[row] = QVector<int>(nbCols, 0);
    }
    for(int row = 7; row < 9; row++){
        mapInt[row] = QVector<int>(nbCols, 1);
    }
    mapInt[9] = QVector<int>(nbCols, 2);
    mapInt[4][13] = 2;
    mapInt[4][14] = 2;
    mapInt[4][15] = 2;
    mapInt[4][16] = 2;
    mapInt[4][17] = 2;
    mapInt[7][15] = 0;
    mapInt[8][15] = 0;
    mapInt[9][15] = 0;
    mapInt[5][26] = 2;
    mapInt[4][26] = 2;
    mapInt[6][26] = 2;

    //Fill imagesArray
    for(int anim = 0; anim < nbPix; anim++){
        Animation * animation = new Animation();
        int fileNumber = fileName[anim].size();
        for(int pic = 0; pic < fileNumber; pic++){
            QPixmap * pixmap = new QPixmap(fileName[anim][pic]);
            animation->addImage(pixmap);
        }
        animationsMap.insert(animationName[anim], animation);
    }
    //Fill map
    for(int row = 0; row < nbRows; row++){
        for(int col = 0; col < nbCols; col++){
            int idTile = mapInt[row][col];
            map[row][col] = new Tile(pixIsWall[idTile], col, row, animationsMap[animationName[idTile]]);
        }
    }
    player = new Player(1, 6, animationsMap);
    Roomba * roomba = new Roomba(14, 6, animationsMap);
    LuckyBlock * lb = new LuckyBlock(6,6, animationsMap, 1);
    livingEntities.push_back(player);
    livingEntities.push_back(roomba);
    livingEntities.push_back(lb);

    keys.push_back(false);
    keys.push_back(false);
    keys.push_back(false);
}
