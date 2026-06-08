#ifndef PLAYER_H
#define PLAYER_H
#include "board.h"
#include "objects.h"
#include "character.h"
#include "sounds.h"
#include "client.h"
#include "server.h"
#include "protocol.h"
#include <iostream>
#include <random>
#include <QThread>
using namespace std;

class player : public QObject
{
    Q_OBJECT
private:

    // Action points variables
    int interest;
    int actionPoints;

public:
    // Server and client
    //
    Server *server;
    Client *client;
    QString shareCode;

    // Nicknames
    //
    string playerName;
    string enemyName;

    // Player
    //
    // Character variables
    character hero;
    int characterID;
    int enemyCharacterID;

    // Player game board scene
    board myBoard;

    // Boolean players map
    bool myMap[NUM_SQUARES][NUM_SQUARES];

    // Indicates what the Player hitted in the enemy board
    squareState myHits[NUM_SQUARES][NUM_SQUARES];

    // Player objects
    objects myShip[NUM_SHIP];

    // Resetes the map
    void resetHits();

    // Builds player ships
    void makePlayerShips();

    // Hit parameters
    QVector<pos> hits;
    void buildHits();

    // Draws
    void drawPlayerMiss(pos pos);
    void drawPlayerHit(pos pos);
    void drawPlayerDestroyed(pos pos,int shipID, bool rotated);

    // Uses the skill
    bool useSkill(skill& skill,Sounds& destroySound);

    // Checks if the player has lost
    bool checkLost();

    // Enemy
    //
    // Enemy game board scene
    board enemyBoard;

    // Indicates what the Enemy hitted in the enemy board
    squareState enemyHits[NUM_SQUARES][NUM_SQUARES];

    // Enemy objects
    objects enemyShip[NUM_SHIP];

    // Enemy ships
    void makeEnemyShips(int characterID);

    // Checks if this positon is a hit
    int respondHit(pos pos);
    bool respondDestroyed(pos pos, bool destroy);
    bool respondRadar(pos pos);

    // Draws a miss and updates "enemyHits"
    void drawEnemyMiss(pos pos);
    // Draws a hit and updates "enemyHits"
    void drawEnemyHit(pos pos);
    // Updates "enemyHits"
    void drawEnemyDestroyed(int ID);
    // Draws green if the radar detects something
    void drawRadar(pos pos);

    // Placement
    //
    // Shadow use to show the next ship position
    QGraphicsRectItem shadow;

    // Placement methods
    void isValid(int x, int y, int id);
    void setObjectBool(int ID, bool state);
    void randomizeBoard();
    bool bondsCheck(int x, int y, int id);
    bool areaCheck(int x, int y, int id);

    // Getters
    //
    int getInterest();
    int getActionPoints();

    // Setters
    //
    void setInterest(int i);
    void setActionPoints(int ap);

    // Online
    //
    // In game call functions
    bool waitPacket(QString dataString);
    int sendHit(pos pos);
    bool sendDestroyed(pos pos, bool destroy);
    void sendDestroyedPos(pos pos,int id,bool rotated);
    bool sendRadar(pos pos);
    bool waitVictoryResponse();

    // Packet transformation
    int getCode(QChar code)
    {
        if(code.isDigit()) return code.digitValue();
        else return code.toLatin1();
    }

signals:
    // Signals
    //
    int askHit(pos pos);
    bool askRadar(pos pos);
    bool askDestroyed(pos pos, bool destroy);
    void giveDestroyedPos(pos pos, int shipID, bool rotated);
    bool checkVictory();
    void sendSignal(QString result);


};
#endif // PLAYER_H
