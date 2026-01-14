#ifndef CPU_H
#define CPU_H
#include "board.h"
#include "objects.h"
#include "position.h"
#include "character.h"
#include <iostream>
#include <random>
using namespace std;

// The CPU must be like a player robot
class cpu : public QObject
{
    Q_OBJECT
private:

    // Action points variables
    int interest;
    int actionPoints;

public:
    // Bot
    //
    // Character variables
    character hero;
    int characterID;

    // Boolean map
    bool myMap[NUM_SQUARES][NUM_SQUARES];

    // Bot objects
    objects myShip[NUM_SHIP];

    // Indicates what the CPU hitted in the enemy board
    squareState myHits[NUM_SQUARES][NUM_SQUARES];

    // Hit parameters
    QVector<pos> hits;
    void buildHits(skill& skill);

    // Playing functions
    bool makePlay();
    bool useSkill(skill& skill);

    // Check hit methods
    int respondHit(pos pos);
    bool respondRadar(pos pos);
    bool respondDestroyed(pos pos, bool destroy);

    // Adds the destroyed object to the squareState map
    void drawCPUDestroyed(pos pos,int shipID, bool rotated);

    // Check is the CPU lost
    bool checkLost();

    // Enemy
    //
    // Enemy objects
    objects enemyShip[NUM_SHIP];

    // Indicates what the Enemy hitted in the enemy board
    squareState enemyHits[NUM_SQUARES][NUM_SQUARES];

    // Loads the enemy objects
    void makeEnemyShips();

    // Updates "enemyHits"
    void drawEnemyDestroyed(int ID);

    // Placemente
    //
    // Placemente methods
    void makeBoard();
    void makeShips();
    void randomizeBoard();
    void setObjectBool(int ID, bool state);
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

signals:
    // Signals
    //
    int askHit(pos pos);
    bool askDestroyed(pos pos, bool destroy);
    void giveDestroyedPos(pos pos, int shipID, bool rotated);
    bool checkVictory();
};

#endif // CPU_H
