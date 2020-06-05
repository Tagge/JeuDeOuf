#include "level.h"
#include "player.h"
#include "roomba.h"
#include "luckyblock.h"
#include "header.h"
#include "endgate.h"
#include "spawngate.h"
#include "movingplatform.h"
#include "brick.h"
#include "buzzer.h"
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include "checkpoint.h"



//Mock level
Level::Level()
{
    int nbPix = 10;
    nbRows = 10;
    nbCols = 100;
    duration = 60000;
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

    levelId = 1;


    beginDate = std::chrono::steady_clock::now();

    keys.push_back(false);
    keys.push_back(false);
    keys.push_back(false);
}

Level::~Level() {
    for(int i = 0; i<map.size(); i++) {
        for(int k = 0; k<map[i].size(); k++) {
            delete(map[i][k]);
        }
    }
    for(int i = 0; i<livingEntities.size(); i++) {
        delete(livingEntities[i]);
    }
    for(auto const& x : animationsMap) {
        delete(x);
    }
}

void Level::setTimeElapsed(bool value) {
    timeElapsed = value;
    if(timeElapsed == true) {
        beginDate = std::chrono::steady_clock::now();
        player->setHealth(-1);
    }
}

int Level::timeleft() {
    auto now = std::chrono::steady_clock::now();
    auto difference  = now-beginDate;
    int timeElapsed = difference.count()/1000000000;
    return (duration/1000)-timeElapsed;
}

void Level::verifyTime() {
    auto now = std::chrono::steady_clock::now();
    auto difference  = now-beginDate;
    int timeElapsed = difference.count()/1000000000;
    if(timeElapsed > duration/1000) {
        setTimeElapsed(true);
    }
}

Level::Level(QString levelFileName, int livesLeft, std::chrono::time_point<std::chrono::steady_clock> beginDate, bool check) : Level::Level(levelFileName, livesLeft) {
    this->beginDate = beginDate;
    qDebug() << check;
    if(check){
        player->setPosTmp(checkpoint->getXCheckpoint()+player->getHitbox().width(), player->getHitbox().top());
        checkpoint->setChecked(true);
        player->validatePos();
    }
}

Level::Level(QString levelFileName, int livesLeft) : Level::Level(levelFileName) {
   player->setLivesLeft(livesLeft);
}

Level::Level(QString levelFileName)
{
    keys.push_back(false);
    keys.push_back(false);
    keys.push_back(false);
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
        nbRows = level["rows"].toInt();
        nbCols = level["columns"].toInt();
        duration = level["duration"].toInt();
        levelId = level["id"].toInt();
        xWindow = level["start_x"].toInt()*constants::TILE_WIDTH;
        yWindow = (nbRows-level["start_y"].toInt())*constants::TILE_HEIGHT;
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
        QJsonArray mapJson = level["map"].toArray();
        for(int row = 0; row < nbRows; row++){
            QJsonArray tilesRow = mapJson[row].toArray();
            for(int col = 0; col < nbCols; col++){
                QString idTile = "0";
                if(tilesRow[col].isString()){
                    idTile = tilesRow[col].toString();
                }
                map[row][col] = new Tile(isWall[idTile], col, row, animationsMap[idTile]);
            }
        }
        QMap<int, QString> mapEntities;
        QMap<int, QJsonObject> mapParameters;
        QJsonArray entitiesData = level["entities"].toArray();
        int nbEntityTypes = entitiesData.size();
        for(int entityId = 0; entityId < nbEntityTypes; entityId++){
            QJsonObject entityData = entitiesData[entityId].toObject();
            mapEntities.insert(entityData["id"].toInt(), entityData["type"].toString());
            mapParameters.insert(entityData["id"].toInt(), entityData["parameters"].toObject());
            addAnimationFromJson(entityData);
        }
        //QJsonArray mapEntitiesJson = level["entities_map"].toArray();
        for(int row = 0; row < nbRows; row++){
            QJsonArray entitiesRow = mapJson[row].toArray();
            for(int col = 0; col < nbCols; col++){
                int idEntity = 0;
                if(entitiesRow[col].isDouble()){
                    idEntity = entitiesRow[col].toInt();
                }
                if(idEntity > 0){
                    createEntityFromJson(mapEntities[idEntity], col, row, mapParameters[idEntity]);
                }
            }
        }
        QJsonObject player = level["player"].toObject();
        addAnimationFromJson(player);
        createEntityFromJson("Player", player["x"].toInt(), player["y"].toInt(), QJsonObject());
        beginDate = std::chrono::steady_clock::now();
    }
    else{
        qDebug() << "File not found";
    }
}

void Level::addAnimationFromJson(const QJsonObject &object)
{
    QString entityName = object["type"].toString();
    if(entityName == "Roomba"){
        QJsonArray roombaSprites = object["roomba"].toArray();
        addAnimation(roombaSprites, "roomba");
        QJsonArray deathRoombaSprites = object["roomba_death"].toArray();
        addAnimation(deathRoombaSprites, "roomba_death");
    }
    else if(entityName == "Player"){
        QJsonArray character = object["character"].toArray();
        addAnimation(character, "character");
        QJsonArray characterMove = object["character_move"].toArray();
        addAnimation(characterMove, "character_move");
        QJsonArray characterJump = object["character_jump"].toArray();
        addAnimation(characterJump, "character_jump");
        QJsonArray character2 = object["character2"].toArray();
        addAnimation(character2, "character2");
        QJsonArray characterMove2 = object["character2_move"].toArray();
        addAnimation(characterMove2, "character_move2");
        QJsonArray characterJump2 = object["character2_jump"].toArray();
        addAnimation(characterJump2, "character_jump2");
        QJsonArray characterDeath = object["character_death"].toArray();
        addAnimation(characterDeath, "character_death");
    }
    else if(entityName == "LuckyBlock"){
        QJsonArray lucky = object["lucky"].toArray();
        addAnimation(lucky, "lucky");
        QJsonArray luckyUsed = object["lucky_used"].toArray();
        addAnimation(luckyUsed, "lucky_used");
    }
    else if(entityName == "PowerUp"){
        QJsonArray bolt = object["bolt"].toArray();
        addAnimation(bolt, "bolt");
    }
    else if(entityName == "EndGate"){
        QJsonArray endGate = object["end_gate"].toArray();
        addAnimation(endGate, "end_gate");
    }
    else if(entityName == "SpawnGate"){
        QJsonArray endGate = object["spawn_gate"].toArray();
        addAnimation(endGate, "spawn_gate");
    }
    else if(entityName == "Platform"){
        QJsonArray platform2 = object["platform2"].toArray();
        addAnimation(platform2, "platform2");
        QJsonArray platform3 = object["platform3"].toArray();
        addAnimation(platform3, "platform3");
        QJsonObject params = object["parameters"].toObject();
        QJsonArray xStarts = params["x_start"].toArray();
        QJsonArray yStarts = params["y_start"].toArray();
        QJsonArray xEnds = params["x_end"].toArray();
        QJsonArray yEnds = params["y_end"].toArray();
        QJsonArray sizes = params["size"].toArray();
        int nbPlatforms = xStarts.size();
        for(int platform = 0; platform < nbPlatforms; platform++){
            livingEntities.push_back(new MovingPlatform(xStarts[platform].toInt(), yStarts[platform].toInt(), xEnds[platform].toInt(), yEnds[platform].toInt(), sizes[platform].toInt(), animationsMap));
        }
    }
    else if(entityName == "Checkpoint"){
        QJsonArray checkpointOn = object["checkpoint_on"].toArray();
        addAnimation(checkpointOn, "checkpoint_on");
        QJsonArray checkpointOff = object["checkpoint_off"].toArray();
        addAnimation(checkpointOff, "checkpoint_off");
    }
    else if(entityName == "Brick"){
        QJsonArray brick = object["brick"].toArray();
        addAnimation(brick, "brick");
    }
    else if(entityName == "Buzzer"){
        QJsonArray buzzer = object["buzzer"].toArray();
        addAnimation(buzzer, "buzzer");
        QJsonArray buzzer_death = object["buzzer_death"].toArray();
        addAnimation(buzzer_death, "buzzer_death");
        QJsonObject params = object["parameters"].toObject();
        QJsonArray xStarts = params["x_start"].toArray();
        QJsonArray yStarts = params["y_start"].toArray();
        QJsonArray xEnds = params["x_end"].toArray();
        QJsonArray yEnds = params["y_end"].toArray();
        int nbPlatforms = xStarts.size();
        for(int platform = 0; platform < nbPlatforms; platform++){
            livingEntities.push_back(new Buzzer(xStarts[platform].toInt(), yStarts[platform].toInt(), xEnds[platform].toInt(), yEnds[platform].toInt(), animationsMap));
        }
    }
}

void Level::addAnimation(const QJsonArray &images, QString name)
{
    Animation * animation = new Animation();
    int fileNumber = images.size();
    for(int pic = 0; pic < fileNumber; pic++){
        QPixmap * pixmap = new QPixmap(":/"+images[pic].toString());
        animation->addImage(pixmap);
    }
    animationsMap.insert(name, animation);
}

void Level::createEntityFromJson(QString name, int x, int y, const QJsonObject & param)
{
    if(name == "Roomba"){
        livingEntities.push_back(new Roomba(x, y, animationsMap));
    }
    else if(name == "Player"){
        player = new Player(x, y, animationsMap);
        livingEntities.push_back(player);
    }
    else if(name == "LuckyBlock"){
        livingEntities.push_back(new LuckyBlock(x, y, animationsMap, param["object"].toInt()));
    }
    else if(name == "EndGate"){
        livingEntities.push_back(new EndGate(x, y, animationsMap));
    }
    else if(name == "SpawnGate"){
        livingEntities.push_back(new SpawnGate(x, y, animationsMap));
    }
    else if(name == "Checkpoint"){
        checkpoint = new Checkpoint(x, y, animationsMap);
        livingEntities.push_back(checkpoint);
    }
    else if(name == "Brick"){
        livingEntities.push_back(new Brick(x, y, animationsMap));
    }
}
