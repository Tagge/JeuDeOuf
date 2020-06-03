#include "level.h"
#include "player.h"
#include "roomba.h"
#include "header.h"
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>


//Mock level
Level::Level()
{   
    int nbPix = 9;
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
    QString animationName[] = {"air", "ground", "block", "character", "character_move", "character_jump", "character_death", "roomba", "roomba_death"};
    QVector<QVector<QString>> fileName  = {{":/sprites/air.png"}, {":/sprites/Ground.png"}, {":/sprites/Block.png"}, {":/sprites/character/Character_0004.png"}, {":/sprites/character/Character_0001.png", ":/sprites/character/Character_0002.png", ":/sprites/character/Character_0003.png", ":/sprites/character/Character_0002.png"}, {":/sprites/character/Character_0000.png"}, {":/sprites/character/Character_0005.png"}, {":/sprites/enemies/Roomba_0", ":/sprites/enemies/Roomba_1"}, {":/sprites/enemies/DeathRoomba"}};
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
    Roomba * roomba = new Roomba(22, 6, animationsMap);
    livingEntities.push_back(player);
    livingEntities.push_back(roomba);

    keys.push_back(false);
    keys.push_back(false);
    keys.push_back(false);
}

Level::Level(QString levelFileName)
{
    QJsonDocument levelJson;
    QByteArray dataJson;
    QFile levelFile(levelFileName);
    QJsonObject level;
    QMap<QString, bool> isWall;
    QVector<QString> animationsName;
    if(levelFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        dataJson = levelFile.readAll();
        levelJson = levelJson.fromJson(dataJson);
        level = levelJson.object();
        xWindow = level["start_x"].toInt();
        yWindow = level["start_y"].toInt();
        nbRows = level["rows"].toInt();
        nbCols = level["columns"].toInt();
        for(int row = 0; row < nbRows; row++){
            map.push_back(QVector<Tile*>(nbCols));
        }
        QJsonArray tilesData = level["tiles"].toArray();
        int nbTilesData = tilesData.size();
        for(int tileId = 0; tileId < nbTilesData; tileId++){
            QJsonObject tileData = tilesData[tileId].toObject();
            isWall.insert(tileData["id"].toString(), tileData["wall"].toBool());
            Animation * animation = new Animation();
            int fileNumber = tileData["images_number"].toInt();
            QJsonArray paths = tileData["paths"].toArray();
            for(int pic = 0; pic < fileNumber; pic++){
                QPixmap * pixmap = new QPixmap(":/"+paths[pic].toString());
                animation->addImage(pixmap);
            }
            animationsMap.insert(tileData["id"].toString(), animation);
        }
        QJsonObject obj = tilesData[0].toObject();
        QJsonArray mapJson = level["map"].toArray();
        for(int row = 0; row < nbRows; row++){
            QJsonArray tilesRow = mapJson[row].toArray();
            for(int col = 0; col < nbCols; col++){
                QString idTile = tilesRow[col].toString();
                map[row][col] = new Tile(isWall[idTile], col, row, animationsMap[idTile]);
            }
        }
        QMap<int, QString> mapEntities;
        QJsonArray entitiesData = level["entities"].toArray();
        int nbEntityTypes = entitiesData.size();
        /*for(int entityId = 0; entityId < nbTilesData; entityId++){
            QJsonObject entityData = entitiesData[entityId].toObject();
            mapEntities.insert(entityData["id"].toInt(), entityData["name"].toString());
            Animation * animation = new Animation();
            QJsonArray roombaSprites = entityData["roomba"].toArray();
            int fileNumber = roombaSprites.size();
            for(int pic = 0; pic < fileNumber; pic++){
                QPixmap * pixmap = new QPixmap(":/"+roombaSprites[pic].toString());
                animation->addImage(pixmap);
            }
            animationsMap.insert("roomba", animation);
            Animation * animDeath = new Animation();
            QJsonArray deathRoombaSprites = entityData["roomba_death"].toArray();
            fileNumber = deathRoombaSprites.size();
            for(int pic = 0; pic < fileNumber; pic++){
                QPixmap * pixmap = new QPixmap(":/"+deathRoombaSprites[pic].toString());
                animDeath->addImage(pixmap);
            }
            animationsMap.insert("roomba_death", animDeath);
        }*/
    }
    else{
        qDebug() << "File not found";
    }
}
