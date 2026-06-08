#include "cpu.h"


// Randomizes the board
void cpu::randomizeBoard()
{
    // Resets the boolean map
    for(int i = 0; i < NUM_SHIP; i++) setObjectBool(i,false);

    // Initialize random number engine
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 11);
    uniform_int_distribution<> fifty(0, 1);
    qreal randomX, randomY;
    int i = 0;

    // Loop that finds random valid positions
    while(i < NUM_SHIP)
    {
        // Generates a random position
        randomX  = dis(gen) * SQUARE;
        randomY = dis(gen) * SQUARE;

        // Rotates the object or not
        if(fifty(gen)) myShip[i].rotateObject();

        // Checks if the random position is valid
        if(bondsCheck(randomX,randomY,i))
        {
            if(areaCheck(randomX,randomY,i))
            {
                myShip[i].setPos({randomX,randomY});
                myShip[i].getSquares();
                setObjectBool(i,true);
                i++;
            }
        }
    }
}

// Updates the boolean map
void cpu::setObjectBool(int ID, bool state)
{
    // state TRUE -> Adds the ship to the boolean map
    // state FALSE -> Removes the ship to the boolean map
    for(int i = myShip[ID].pos().x()/SQUARE; i < myShip[ID].pos().x()/SQUARE + myShip[ID].rect().width()/SQUARE; i++)
    {
        if(i >= NUM_SQUARES) continue;
        for(int j = myShip[ID].pos().y()/SQUARE; j < myShip[ID].pos().y()/SQUARE + myShip[ID].rect().height()/SQUARE; j++)
        {
            if(j >= NUM_SQUARES) continue;
            myMap[i][j] = state;

        }
    }
}

// Initiates the CPU ships
void cpu::makeShips()
{
    // Clears the map
    for(int i = 0; i < NUM_SQUARES; i++)
        for(int j = 0; j < NUM_SQUARES; j++)
        {
            enemyHits[i][j] = nothing;
            myHits[i][j] = nothing;
            myMap[i][j] = false;
        }

    // Sets the initial size of the ships
    myShip[0].setRect(0,0,SQUARE,SQUARE*2);
    myShip[1].setRect(0,0,SQUARE,SQUARE*2);
    myShip[2].setRect(0,0,SQUARE,SQUARE*3);
    myShip[3].setRect(0,0,SQUARE,SQUARE*3);
    myShip[4].setRect(0,0,SQUARE,SQUARE*4);
    myShip[5].setRect(0,0,SQUARE,SQUARE*5);

    // Loads all the images from the ships
    QPixmap shipImage[6];
    switch (characterID) {

    case CAPTAIN_ID:
        // Load pix images from ships
        shipImage[0].load(":/resources/Captain Objects/captain_ship_1.png");
        shipImage[0] = shipImage[0].scaled(SQUARE,2*SQUARE,Qt::IgnoreAspectRatio);

        shipImage[1].load(":/resources/Captain Objects/captain_ship_2.png");
        shipImage[1]= shipImage[1].scaled(SQUARE,2*SQUARE,Qt::IgnoreAspectRatio);

        shipImage[2].load(":/resources/Captain Objects/captain_ship_3.png");
        shipImage[2] = shipImage[2].scaled(SQUARE,3*SQUARE,Qt::IgnoreAspectRatio);

        shipImage[3].load(":/resources/Captain Objects/captain_ship_4.png");
        shipImage[3] = shipImage[3].scaled(SQUARE,3*SQUARE,Qt::IgnoreAspectRatio);

        shipImage[4].load(":/resources/Captain Objects/captain_ship_5.png");
        shipImage[4] = shipImage[4].scaled(SQUARE,4*SQUARE,Qt::IgnoreAspectRatio);

        shipImage[5].load(":/resources/Captain Objects/captain_ship_6.png");
        shipImage[5] = shipImage[5].scaled(SQUARE,5*SQUARE,Qt::IgnoreAspectRatio);
    break;

    case PIRATE_ID:
        // Load pix images from ships
        shipImage[0].load(":/resources/Pirate Objects/pirate_ship_1.png");
        shipImage[0] = shipImage[0].scaled(SQUARE,2*SQUARE,Qt::IgnoreAspectRatio);

        shipImage[1].load(":/resources/Pirate Objects/pirate_ship_2.png");
        shipImage[1]= shipImage[1].scaled(SQUARE,2*SQUARE,Qt::IgnoreAspectRatio);

        shipImage[2].load(":/resources/Pirate Objects/pirate_ship_3.png");
        shipImage[2] = shipImage[2].scaled(SQUARE,3*SQUARE,Qt::IgnoreAspectRatio);

        shipImage[3].load(":/resources/Pirate Objects/pirate_ship_4.png");
        shipImage[3] = shipImage[3].scaled(SQUARE,3*SQUARE,Qt::IgnoreAspectRatio);

        shipImage[4].load(":/resources/Pirate Objects/pirate_ship_5.png");
        shipImage[4] = shipImage[4].scaled(SQUARE,4*SQUARE,Qt::IgnoreAspectRatio);

        shipImage[5].load(":/resources/Pirate Objects/pirate_ship_6.png");
        shipImage[5] = shipImage[5].scaled(SQUARE,5*SQUARE,Qt::IgnoreAspectRatio);
    break;

    case SPACE_ID:
        // Load pix images from ships
        shipImage[0].load(":/resources/Space Objects/space_ship_1.png");
        shipImage[0] = shipImage[0].scaled(SQUARE,2*SQUARE,Qt::IgnoreAspectRatio);

        shipImage[1].load(":/resources/Space Objects/space_ship_2.png");
        shipImage[1]= shipImage[1].scaled(SQUARE,2*SQUARE,Qt::IgnoreAspectRatio);

        shipImage[2].load(":/resources/Space Objects/space_ship_3.png");
        shipImage[2] = shipImage[2].scaled(SQUARE,3*SQUARE,Qt::IgnoreAspectRatio);

        shipImage[3].load(":/resources/Space Objects/space_ship_4.png");
        shipImage[3] = shipImage[3].scaled(SQUARE,3*SQUARE,Qt::IgnoreAspectRatio);

        shipImage[4].load(":/resources/Space Objects/space_ship_5.png");
        shipImage[4] = shipImage[4].scaled(SQUARE,4*SQUARE,Qt::IgnoreAspectRatio);

        shipImage[5].load(":/resources/Space Objects/space_ship_6.png");
        shipImage[5] = shipImage[5].scaled(SQUARE,5*SQUARE,Qt::IgnoreAspectRatio);
    break;

    default:
    break;
    }

    // Makes the ships and adds them to the scene
    QBrush brush;
    QPen *pen = new QPen(Qt::transparent);
    for(int i = 0; i < NUM_SHIP; i++)
    { 
        brush.setTexture(shipImage[i]);
        myShip[i].rotated = false;
        myShip[i].canRotate = true;
        myShip[i].myID = i;
        myShip[i].setBrush(brush);
        myShip[i].setPen(*pen);
        myShip[i].setPos(SQUARE*i*2,0);
    }


}

// Initiates enemy ships
void cpu::makeEnemyShips()
{

    // Sets the initial size of the ships
    enemyShip[0].setRect(0,0,SQUARE,SQUARE*2);
    enemyShip[1].setRect(0,0,SQUARE,SQUARE*2);
    enemyShip[2].setRect(0,0,SQUARE,SQUARE*3);
    enemyShip[3].setRect(0,0,SQUARE,SQUARE*3);
    enemyShip[4].setRect(0,0,SQUARE,SQUARE*4);
    enemyShip[5].setRect(0,0,SQUARE,SQUARE*5);

    for(int i = 0; i < NUM_SHIP; i++)
    {

        enemyShip[i].rotated = false;
        enemyShip[i].canRotate = true;
        enemyShip[i].myID = i;
    }
}

// Checks if the ship is out-of-bounds (out side the map)
bool cpu::bondsCheck(int x, int y, int id)
{
    bool valid = true;
    for(int i = x/SQUARE; i < x/SQUARE + myShip[id].rect().width()/SQUARE; i++)
    {
        for(int j = y/SQUARE; j < y/SQUARE + myShip[id].rect().height()/SQUARE; j++)
        {
            if((i < 0 || j < 0) || (i >= NUM_SQUARES || j >= NUM_SQUARES))
            {
                valid = false;
                goto theEnd;
            }
        }
    }
theEnd:
    return valid;
}

// Checks if theres no ships within 1 block
bool cpu::areaCheck(int x, int y, int id)
{
    bool valid = true;
    for(int i = x/SQUARE - 1; i <= x/SQUARE + myShip[id].rect().width()/SQUARE; i++)
    {
        if(i < 0 || i >= NUM_SQUARES) continue; // Out-of-bounds
        for(int j = y/SQUARE - 1; j <= y/SQUARE + myShip[id].rect().height()/SQUARE; j++)
        {
            if(j < 0 || j >= NUM_SQUARES) continue; // Out-of-bounds

            // Invalid position detection
            if(myMap[i][j])
            {
                valid = false;
                goto theEnd;
            }
        }
    }
theEnd:
    return valid;
}

// Build the random board for the bot
void cpu::makeBoard()
{
    // Initialize random number engine
    // Chooses a random character
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> heroSelect(1, 3);
    this->characterID = heroSelect(gen);
    hero.loadCharacter(this->characterID);
    makeShips();
    randomizeBoard();

}

// Builds the hits vector
void cpu::buildHits(skill& skill)
{
    hits.clear();

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0,11);
    qreal randomX , randomY;
    // Loop that finds random valid positions

    theTop:

    // Generates a random position
    randomX  = dis(gen);
    randomY = dis(gen);

    // Checks if the random position is valid
    if(randomX + skill.getAoeSize().x > NUM_SQUARES) goto theTop;
    if(randomY + skill.getAoeSize().y > NUM_SQUARES) goto theTop;
    if(myHits[(int)randomX][(int)randomY] == miss) goto theTop;
    for(int i = randomX; i < randomX + skill.getAoeSize().x; i++)
    {
        if(i < 0 || i >= NUM_SQUARES) continue;
        for(int j = randomY; j < randomY + skill.getAoeSize().y; j++)
        {
            if(j < 0 || j >= NUM_SQUARES) continue;
            hits.append({i,j});
        }
    }
}

// Checks if the pos in the argument is a hit
int cpu::respondHit(pos pos)
{
    if(!myMap[pos.x][pos.y]) // Miss
    {
        if(enemyHits[pos.x][pos.y] == nothing)
        {
            enemyHits[pos.x][pos.y] = miss;
            return 1;
        }
        else return -1; // Duplicate Miss
    }
    else
    {
        if(enemyHits[pos.x][pos.y] == nothing)
        {
            enemyHits[pos.x][pos.y] = hit;
            return 2; // Hit
        }
        else return -2; // Duplicate hit
    }
}

// Check if there is something in pos
bool cpu::respondRadar(pos pos)
{
    return myMap[pos.x][pos.y];
}

// Check if a ship was destroyed
bool cpu::respondDestroyed(pos pos, bool destroy)
{
    // Cycle to search for the object which was hit
    for(int ID = 0; ID < NUM_SHIP; ID++)
    {
        if(myShip[ID].sunk) continue;
        for(int i = 0; i < myShip[ID].mySquares.size(); i++)
        {
            // Checks in which object the hitted position is
            if((myShip[ID].mySquares[i].pos.x == pos.x) && (myShip[ID].mySquares[i].pos.y == pos.y))
            {
                if(destroy) for(int j = 0; j < myShip[ID].mySquares.size(); j++) myShip[ID].mySquares[j].hit = true;
                else myShip[ID].mySquares[i].hit = true;

                for(int j = 0; j < myShip[ID].mySquares.size(); j++)
                    if(myShip[ID].mySquares[j].hit == false) return false; // The ship is not destroyed

                myShip[ID].sunk = true;
                drawEnemyDestroyed(ID);
                pos.x = myShip[ID].pos().x();
                pos.y = myShip[ID].pos().y();
                emit giveDestroyedPos(pos,ID,myShip[ID].rotated);
                return true; // The ship is destroyed
            }
        }
    }
    return false; // Didnt find a valid square
}

// CPU decides a play - returns true if the CPU won
bool cpu::makePlay()
{
    // Initialize random number engine
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 3);
    int skillID;

    while(1)
    {
        skillID = dis(gen);
        switch (skillID)
        {
            case 1:
                if(hero.primary.used) break;
                if(getActionPoints() < hero.primary.getCost()) break;
                if(useSkill(hero.primary)) return true;
            break;

            case 2:
                if(hero.secondary.used) break;
                if(getActionPoints() < hero.secondary.getCost()) break;
                if(useSkill(hero.secondary)) return true;
            break;

            case 3:
                if(hero.ultimate.used) break;
                if(getActionPoints() < hero.ultimate.getCost()) break;
                if(useSkill(hero.ultimate)) return true;
            break;

            default:
            break;
        }

        // Certifies a deathlock wont happen
        if((getActionPoints() < hero.primary.getCost()) || hero.primary.used)
            if((getActionPoints() < hero.secondary.getCost()) || hero.secondary.used)
                if((getActionPoints() < hero.ultimate.getCost()) || hero.ultimate.used) break;   
    }

    return false;
}

// CPU uses a skill
bool cpu::useSkill(skill& skill)
{
    int hitted;

    // Builds the hits pos vector
    this->buildHits(skill);

    // Runs the vector
    while(!hits.isEmpty())
    {
        // Mega Cannon Skill
        if(skill.name == "Humungous!")
        {
            if(myHits[hits[0].x][hits[0].y] == hit) // Checks if the spot is already hitted
            {
                // Checks if this is a duplicate hit
                if(emit askHit(hits[0]) == -2)
                {
                    // Destroys the ship
                    if(emit askDestroyed(hits[0],skill.destroy))
                        if(emit checkVictory()) return true;
                    goto theEnd;
                }
                else goto theEnd; // If the ship is already destroyed nothing happens
            }
            else goto theEnd;
        }

        // Ion Blaster Skill
        if(skill.name == "Ion Blaster")
        {
            string here;
            if(hits[0].x >= NUM_SQUARES/2 && hits[0].y >= NUM_SQUARES/2) here = "left_up";
            else if(hits[0].x < NUM_SQUARES/2 && hits[0].y >= NUM_SQUARES/2) here = "right_up";
            else if(hits[0].x < NUM_SQUARES/2 && hits[0].y < NUM_SQUARES/2) here = "right_down";
            else if(hits[0].x >= NUM_SQUARES/2 && hits[0].y < NUM_SQUARES/2) here = "left_down";

            while(emit askHit(hits[0]) != 2)
            {
                if(myHits[hits[0].x][hits[0].y] == nothing) myHits[hits[0].x][hits[0].y] = miss;
                // Calculates the next position
                if(here == "left_up")
                {
                    hits[0].x -= 1;
                    hits[0].y -= 1;
                }
                else if(here == "right_up")
                {
                    hits[0].x += 1;
                    hits[0].y -= 1;
                }
                else if(here == "right_down")
                {
                    hits[0].x += 1;
                    hits[0].y += 1;
                }
                else if(here == "left_down")
                {
                    hits[0].x -= 1;
                    hits[0].y += 1;
                }

                // If the next hit is outside the board the skill stops
                if(hits[0].x < 0 || hits[0].y < 0 || hits[0].x >= NUM_SQUARES || hits[0].y >= NUM_SQUARES) goto theEnd;
            }

            // Draws the hit
            myHits[hits[0].x][hits[0].y] = hit;
            // Checks if this hit destroy a ship
            if(emit askDestroyed(hits[0],skill.destroy))
                if(emit checkVictory()) return true;

            goto theEnd;
        }

        // If this spot was already targeted is ignored
        if(myHits[hits[0].x][hits[0].y] != nothing) goto theEnd;

        // Checks if it is a hit
        hitted = emit askHit(hits[0]);
        if(hitted == 2)
        {
            // Hit
            myHits[hits[0].x][hits[0].y] = hit;
            // Checks if this hit destroy a ship
            if(emit askDestroyed(hits[0],skill.destroy))
                if(emit checkVictory()) return true;
        }
        else if(hitted == 1) myHits[hits[0].x][hits[0].y] = miss; // Miss

        theEnd:
            hits.removeAt(0);
    }

    this->setActionPoints(this->getActionPoints()-skill.getCost());
    skill.used = true;
    return false;
}

// Draws the destroyed ship on the player board scene
void cpu::drawEnemyDestroyed(int shipID)
{
    // Adds the hits to the squarestate table
    for(int i = myShip[shipID].pos().x()/SQUARE ; i < myShip[shipID].pos().x()/SQUARE + myShip[shipID].rect().width()/SQUARE; i++)
    {
        if(i < 0 || i >= NUM_SQUARES) continue; // Out-of-bounds
        for(int j = myShip[shipID].pos().y()/SQUARE; j < myShip[shipID].pos().y()/SQUARE + myShip[shipID].rect().height()/SQUARE; j++)
        {
            if(j < 0 || j >= NUM_SQUARES) continue; // Out-of-bounds
            if(enemyHits[i][j] == nothing) enemyHits[i][j] = hit;
        }
    }

    // Adds the miss to the squarestate table
    for(int i = myShip[shipID].pos().x()/SQUARE - 1; i <= myShip[shipID].pos().x()/SQUARE + myShip[shipID].rect().width()/SQUARE; i++)
    {
        if(i < 0 || i >= NUM_SQUARES) continue; // Out-of-bounds
        for(int j = myShip[shipID].pos().y()/SQUARE - 1; j <= myShip[shipID].pos().y()/SQUARE + myShip[shipID].rect().height()/SQUARE; j++)
        {
            if(j < 0 || j >= NUM_SQUARES) continue; // Out-of-bounds
            if(enemyHits[i][j] == nothing) enemyHits[i][j] = miss;
        }
    }

}

// Draws the destroyed ship on the enemy board scene
void cpu::drawCPUDestroyed(pos pos,int shipID, bool rotated)
{
    if(rotated) enemyShip[shipID].rotateObject();

    // Draws the hits
    for(int i = pos.x/SQUARE ; i < pos.x/SQUARE + enemyShip[shipID].rect().width()/SQUARE; i++)
    {
        if(i < 0 || i >= NUM_SQUARES) continue; // Out-of-bounds
        for(int j = pos.y/SQUARE; j < pos.y/SQUARE + enemyShip[shipID].rect().height()/SQUARE; j++)
        {
            if(j < 0 || j >= NUM_SQUARES) continue; // Out-of-bounds
            if(myHits[i][j] == nothing) myHits[i][j] = hit;
        }
    }

    // Draws the miss
    for(int i = pos.x/SQUARE - 1; i <= pos.x/SQUARE + enemyShip[shipID].rect().width()/SQUARE; i++)
    {
        if(i < 0 || i >= NUM_SQUARES) continue; // Out-of-bounds
        for(int j = pos.y/SQUARE - 1; j <= pos.y/SQUARE + enemyShip[shipID].rect().height()/SQUARE; j++)
        {
            if(j < 0 || j >= NUM_SQUARES) continue; // Out-of-bounds
            if(myHits[i][j] == nothing) myHits[i][j] = miss;

        }
    }
}

// Checks if the CPU lost
bool cpu::checkLost()
{
    for(int i = 0; i < NUM_SHIP; i++)
        if(!myShip[i].sunk) return false;

    return true;
}

// Getters
int cpu::getInterest()
{
    return interest;
}
int cpu::getActionPoints()
{
    return actionPoints;
}

// Setters
void cpu::setInterest(int i)
{
    if(i < 0) interest = 0;
    else interest = i;
}
void cpu::setActionPoints(int ap)
{
    if(ap < 0) actionPoints = 0;
    else if(ap > 10) actionPoints = (10+interest);
    else actionPoints = ap;
}
