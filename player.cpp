#include "player.h"


// Placement
//
// Checks if the position the ship is meant to be place is valid
void player::isValid(int x, int y, int id)
{
    bool valid = true;

    // Checks if the ship is out-of-bounds
    valid = bondsCheck(x,y,id);
    if(!valid) goto theEnd;

    // Checks if theres no ships within 1 block
    valid = areaCheck(x,y,id);
    if(!valid) goto theEnd;

    theEnd:
    // Updates the shadow
    myShip[id].canPlace = valid; // TRUE or FALSE
    if(valid) shadow.setBrush(*myBoard.greenPaint);
    else shadow.setBrush(*myBoard.redPaint);
    shadow.setOpacity(0.3);
    shadow.setRect(x,y,myShip[id].rect().width(),myShip[id].rect().height());
}

// Updates the boolean map
void player::setObjectBool(int ID, bool state)
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

    // Disables the shadow
    shadow.setOpacity(0);
}

// Checks if the ship is out-of-bounds (out side the map)
bool player::bondsCheck(int x, int y, int id)
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
bool player::areaCheck(int x, int y, int id)
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

// Randomizes the board
void player::randomizeBoard()
{
    // Resets the boolean map
    for(int i = 0; i < NUM_SHIP; i++) setObjectBool(i,false);

    // Initialize random number engine
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 12);
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

// Player
//
// Initiates the players ships
void player::makePlayerShips()
{
    // Clears the map
    for(int i = 0; i < NUM_SQUARES; i++)
        for(int j = 0; j < NUM_SQUARES; j++) myMap[i][j] = false;

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
        default:

        break;
    }

    // Makes the ships and adds them to the scene
    QBrush brush;
    QPen *pen = new QPen(Qt::transparent);
    for(int i = 0; i < NUM_SHIP; i++)
    {
        for(int j = 0; j < myShip[i].rect().height()/SQUARE; j++) myMap[i*2][j] = true; // Places the ships positions in the bool map
        brush.setTexture(shipImage[i]);
        myShip[i].rotated = false;
        myShip[i].canRotate = true;
        myShip[i].myID = i;
        myShip[i].setBrush(brush);
        myShip[i].setPen(*pen);
        myShip[i].setPos(SQUARE*i*2,0);
        myShip[i].setFlag(QGraphicsItem::ItemIsMovable);
        connect(&myShip[i],&objects::shipMoving,this,&player::isValid);
        connect(&myShip[i],&objects::setBool,this,&player::setObjectBool);
        myBoard.scene->addItem(&myShip[i]);
        myShip[i].getSquares();
    }

    // Adds the shadow, that will be green or red on the map, to the scene
    shadow.setRect(0,0,1,1); // Doesnt really matter, just made so the item can be added to the scene it will be updated before placed
    shadow.setOpacity(0); // Makes the item "invisible"
    shadow.setZValue(-1);   // We want the shadow to be the in the last layer so it stays underneath the ships
    myBoard.scene->addItem(&shadow);

}

// Builds the hits vector
void player::buildHits()
{
    pos pos;

    for(int i = hero.preview.pos().x()/SQUARE; i < (hero.preview.pos().x() + hero.preview.rect().width())/SQUARE; i++)
    {
        if(i < 0 || i >= NUM_SQUARES) continue;
        for(int j = hero.preview.pos().y()/SQUARE; j < (hero.preview.pos().y() + hero.preview.rect().height())/SQUARE; j++)
        {
            if(j < 0 || j >= NUM_SQUARES) continue;
            hits.append({i,j});
        }
    }

}

// Uses a the skill
bool player::useSkill(skill& skill,Sounds& destroySound)
{
    int hitted;
    int destroyed;
    skill.used = true;
    skill.placed = false;

    // Builds the hits pos vector
    this->buildHits();

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
                    {
                        if(emit checkVictory())
                        {
                            skill.used = true;
                            skill.placed = false;
                            hero.preview.setRect(0,0,0,0);
                            return true;
                        }
                        destroySound.playSound();
                    }

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

            while(emit askHit(hits[0]) != 2) // Cycle till it hits
            {
                // Checks in the square has nothing to draw in
                if(myHits[hits[0].x][hits[0].y] == nothing) drawPlayerMiss(hits[0]);

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

                // Checks if the next hit is inside the board
                if(hits[0].x < 0 || hits[0].y < 0 || hits[0].x >= NUM_SQUARES || hits[0].y >= NUM_SQUARES) goto theEnd;
            }

            // Draws the hit
            drawPlayerHit(hits[0]);
            // Checks if this hit destroy a ship
            if(emit askDestroyed(hits[0],skill.destroy))
            {
                if(emit checkVictory())
                {
                    skill.used = true;
                    skill.placed = false;
                    hero.preview.setRect(0,0,0,0);
                    return true;
                }
                destroySound.playSound();
            }

            goto theEnd;
        }

        // Radar
        if(skill.name == "Radar")
        {
            if(emit askRadar(hits[0])) drawRadar(hits[0]);
            goto theEnd;
        }

        // If this spot was already targeted is ignored
        if(myHits[hits[0].x][hits[0].y] != nothing) goto theEnd;

        // Checks if it is a hit
        hitted = emit askHit(hits[0]);
        if(hitted == 2) // Hit
        {
            // Draws the hit
            drawPlayerHit(hits[0]);

            // Checks if this hit destroy a ship
            if(emit askDestroyed(hits[0],skill.destroy))
            {
                if(emit checkVictory())
                {
                    hero.preview.setRect(0,0,0,0);
                    return true;
                }
                destroySound.playSound();
            }

            if(skill.name == "Phaser Weapon") skill.used = false;

        }
        else if(hitted == 1) drawPlayerMiss(hits[0]); // Missed


    theEnd:
        hits.removeAt(0);
    }

    hero.preview.setRect(0,0,0,0);
    return false;
}

// Clears the square state map
void player::resetHits()
{
    for(int i = 0; i < NUM_SQUARES; i++)
        for(int j = 0; j < NUM_SQUARES; j++)
        {
            myHits[i][j] = nothing;
            enemyHits[i][j] = nothing;
        }
}

// Draws the hit on the enemy board scene
void player::drawPlayerHit(pos pos)
{
    QGraphicsRectItem *lastItem = enemyBoard.scene->addRect(pos.x*SQUARE,pos.y*SQUARE,SQUARE,SQUARE,QPen(Qt::red),myBoard.cross);
    lastItem->setZValue(1000);
    myHits[pos.x][pos.y] = hit;
}

// Draws the miss on the enemy board scene
void player::drawPlayerMiss(pos pos)
{
    QGraphicsRectItem *lastItem = enemyBoard.scene->addRect(pos.x*SQUARE,pos.y*SQUARE,SQUARE,SQUARE,QPen(Qt::transparent),*myBoard.oceanPaint);
    lastItem->setZValue(-1);
    lastItem->setOpacity(0.5);
    myHits[pos.x][pos.y] = miss;

}

// Draws the destroyed ship on the enemy board scene
void player::drawPlayerDestroyed(pos pos,int shipID, bool rotated)
{
    if(rotated) enemyShip[shipID].rotateObject();

    // Draws the hits
    for(int i = pos.x/SQUARE ; i < pos.x/SQUARE + enemyShip[shipID].rect().width()/SQUARE; i++)
    {
        if(i < 0 || i >= NUM_SQUARES) continue; // Out-of-bounds
        for(int j = pos.y/SQUARE; j < pos.y/SQUARE + enemyShip[shipID].rect().height()/SQUARE; j++)
        {
            if(j < 0 || j >= NUM_SQUARES) continue; // Out-of-bounds
            if(myHits[i][j] == nothing) drawPlayerHit({i,j});
        }
    }

    // Draws the miss
    for(int i = pos.x/SQUARE - 1; i <= pos.x/SQUARE + enemyShip[shipID].rect().width()/SQUARE; i++)
    {
        if(i < 0 || i >= NUM_SQUARES) continue; // Out-of-bounds
        for(int j = pos.y/SQUARE - 1; j <= pos.y/SQUARE + enemyShip[shipID].rect().height()/SQUARE; j++)
        {
            if(j < 0 || j >= NUM_SQUARES) continue; // Out-of-bounds
            if(myHits[i][j] == nothing) drawPlayerMiss({i,j});
        }
    }

    // Draws the ship
    enemyShip[shipID].setPos(QPointF(pos.x,pos.y));
    enemyBoard.scene->addItem(&enemyShip[shipID]);
    enemyShip[shipID].setZValue(-1);
}

// Check if there is something in pos
bool player::respondRadar(pos pos)
{
    return myMap[pos.x][pos.y];
}

// Draws green if the radar finds anything
void player::drawRadar(pos pos)
{
    QGraphicsRectItem *lastItem = enemyBoard.scene->addRect(pos.x*SQUARE,pos.y*SQUARE,SQUARE,SQUARE,QPen(Qt::transparent),*myBoard.greenPaint);
    lastItem->setZValue(-1);
    lastItem->setOpacity(0.5);
}

// Enemy
//
// Initiates the Enemy ships
void player::makeEnemyShips(int enemyCharacterID)
{
    // Sets the initial size of the ships
    enemyShip[0].setRect(0,0,SQUARE,SQUARE*2);
    enemyShip[1].setRect(0,0,SQUARE,SQUARE*2);
    enemyShip[2].setRect(0,0,SQUARE,SQUARE*3);
    enemyShip[3].setRect(0,0,SQUARE,SQUARE*3);
    enemyShip[4].setRect(0,0,SQUARE,SQUARE*4);
    enemyShip[5].setRect(0,0,SQUARE,SQUARE*5);


    // Loads all the images from the ships
    QPixmap shipImage[6];
    switch (enemyCharacterID) {

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
    default:

        break;
    }

    // Makes the ships and adds them to the scene
    QBrush brush;
    QPen *pen = new QPen(Qt::transparent);
    for(int i = 0; i < NUM_SHIP; i++)
    {
        brush.setTexture(shipImage[i]);
        enemyShip[i].rotated = false;
        enemyShip[i].canRotate = true;
        enemyShip[i].myID = i;
        enemyShip[i].setBrush(brush);
        enemyShip[i].setPen(*pen);
        enemyShip[i].setPos(SQUARE*i*2,0);
    }
}

// Checks if the pos in the argument is a hit
int player::respondHit(pos pos)
{
    if(!myMap[pos.x][pos.y])
    {
        if(enemyHits[pos.x][pos.y] == nothing)
        {
            drawEnemyMiss(pos);
            return 1; // Miss
        }
        else return -1; // Duplicate Miss
    }
    else
    {
        if(enemyHits[pos.x][pos.y] == nothing)
        {
            drawEnemyHit(pos);
            return 2; // Hit
        }
        else return -2; // Duplicate hit
    }
}

// Draws the hit on the player board scene
void player::drawEnemyHit(pos pos)
{
    QGraphicsRectItem *lastItem = myBoard.scene->addRect(pos.x*SQUARE,pos.y*SQUARE,SQUARE,SQUARE,QPen(Qt::red),myBoard.cross);
    lastItem->setZValue(1000);
    enemyHits[pos.x][pos.y] = hit;
}

// Draws the miss on the player board scene
void player::drawEnemyMiss(pos pos)
{
    QGraphicsRectItem *lastItem = myBoard.scene->addRect(pos.x*SQUARE,pos.y*SQUARE,SQUARE,SQUARE,QPen(Qt::transparent),*myBoard.oceanPaint);
    lastItem->setZValue(-1);
    lastItem->setOpacity(0.5);
    enemyHits[pos.x][pos.y] = miss;
}

// Draws the destroyed ship on the player board scene
void player::drawEnemyDestroyed(int ID)
{
    // Draws the hits
    for(int i = myShip[ID].pos().x()/SQUARE ; i < myShip[ID].pos().x()/SQUARE + myShip[ID].rect().width()/SQUARE; i++)
    {
        if(i < 0 || i >= NUM_SQUARES) continue; // Out-of-bounds
        for(int j = myShip[ID].pos().y()/SQUARE; j < myShip[ID].pos().y()/SQUARE + myShip[ID].rect().height()/SQUARE; j++)
        {
            if(j < 0 || j >= NUM_SQUARES) continue; // Out-of-bounds
            if(enemyHits[i][j] == nothing) drawEnemyHit({i,j});
        }
    }

    // Draws the miss
    for(int i = myShip[ID].pos().x()/SQUARE - 1; i <= myShip[ID].pos().x()/SQUARE + myShip[ID].rect().width()/SQUARE; i++)
    {
        if(i < 0 || i >= NUM_SQUARES) continue; // Out-of-bounds
        for(int j = myShip[ID].pos().y()/SQUARE - 1; j <= myShip[ID].pos().y()/SQUARE + myShip[ID].rect().height()/SQUARE; j++)
        {
            if(j < 0 || j >= NUM_SQUARES) continue; // Out-of-bounds
            if(enemyHits[i][j] == nothing) drawEnemyMiss({i,j});
        }
    }

    // No need to add the objects to the scene cause they are already there
}

// Check if a ship was destroyed
bool player::respondDestroyed(pos pos,bool destroy)
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

// Checks if the player lost
bool player::checkLost()
{
    for(int i = 0; i < NUM_SHIP; i++)
        if(!myShip[i].sunk) return false;

    return true;
}

// Getters
//
int player::getInterest()
{
    return interest;
}
int player::getActionPoints()
{
    return actionPoints;
}

// Setters
//
void player::setInterest(int i)
{
    if(i < 0) interest = 0;
    else interest = i;
}
void player::setActionPoints(int ap)
{
    if(ap < 0) actionPoints = 0;
    else if(ap > 10) actionPoints = (10+interest);
    else actionPoints = ap;
}

// Online
//
// In game call functions
int player::sendHit(pos pos)
{
    // Builds the data packet
    QByteArray data;
    data.append(QString::number(AHIT_P).toUtf8());
    data.append(QString::number(pos.x).toUtf8());
    data.append('.');
    data.append(QString::number(pos.y).toUtf8());
    data.append('.');
    client->sendData(data);

    // Waits for a packet to be received
    QEventLoop loop;
    QString result;
    connect(this, &player::sendSignal, &loop, [&](QString receivedResult) {
        result = receivedResult;
        loop.quit();
    });
    loop.exec();

    // Returns the response
    // 1 miss , -1 duplicated miss
    // 2 hit , -2 duplicated hit
    if(result.at(0).digitValue() == RHIT_P)
    {
        result.removeFirst();
        // Since the result can be negative "-2" -> 2 elements
        return result.toInt();
    }

    // If the packet was not correctly received
    return 0;
}

bool player::sendDestroyed(pos pos, bool destroy)
{
    // Builds the data packet
    QByteArray data;
    data.append(QString::number(ADESTROY_P).toUtf8());
    data.append(QString::number(pos.x).toUtf8());
    data.append('.');
    data.append(QString::number(pos.y).toUtf8());
    data.append('.');
    data.append(QString::number(destroy ? 1 : 0).toUtf8());
    client->sendData(data);

    // Waits for a packet to be received
    QEventLoop loop;
    QString result;
    connect(this, &player::sendSignal, &loop, [&](QString receivedResult) {
        result = receivedResult;
        loop.quit();
    });
    loop.exec();

    qDebug() << result;


    if(result.at(0).digitValue() == RDESTROY_P)
    {
        result.removeFirst();
        // The ship was destroyed
        // The packet will be break into the information needed
        if(result.at(0).digitValue() == 1)
        {
            result.removeFirst();
            QString aux;
            int i = 0;

            // Gets X pos
            while(result.at(i++) != '.') aux.append(result.at(i-1));
            pos.x = aux.toInt();
            aux.clear(); // Clears AUX

            // Gets Y pos
            while(result.at(i++) != '.') aux.append(result.at(i-1));
            pos.y = aux.toInt();
            aux.clear(); // Clears AUX

            // Gets the destroyed ship ID
            int id = result.at(i++).digitValue();

            // Gets the rotation status of the ship
            bool rotated = result.at(i).digitValue() == 1 ? true : false;

            // Draws in the scene the destroyed ship
            // it also saves the needed information
            drawPlayerDestroyed(pos,id,rotated);
            return true;
        }
    }
    return false;
}

void player::sendDestroyedPos(pos pos,int id,bool rotated)
{
    // Builds the data packet
    QByteArray data;
    data.append(QString::number(RDESTROY_P).toUtf8());
    data.append(QString::number(1).toUtf8());
    data.append(QString::number(pos.x).toUtf8());
    data.append('.');
    data.append(QString::number(pos.y).toUtf8());
    data.append('.');
    data.append(QString::number(id).toUtf8());
    data.append(QString::number(rotated ? 1 : 0).toUtf8());
    client->sendData(data);
}

bool player::sendRadar(pos pos)
{
    // Builds the data packet
    QByteArray data;
    data.append(QString(ARADAR_P).toUtf8());
    data.append(QString::number(pos.x).toUtf8());
    data.append('.');
    data.append(QString::number(pos.y).toUtf8());
    data.append('.');
    client->sendData(data);

    // Waits for a packet to be received
    QEventLoop loop;
    QString result;
    connect(this, &player::sendSignal, &loop, [&](QString receivedResult) {
        result = receivedResult;
        loop.quit();
    });
    loop.exec();

    // Returns the response
    qDebug() << result;
    if(result.at(0).toLatin1() == (int)RRADAR_P)
    {
        result.removeFirst();
        return (result.at(0).digitValue() == 1 ? true : false);
    }

    // If the packet was not correctly received
    return false;

}

bool player::waitVictoryResponse()
{
    // Sends the packet asking if the enemy has all the ships destroyed
    QByteArray data;
    data.append(QString::number(AVIC_P).toUtf8());
    client->sendData(data);

    // Waits for a packet to be received
    QEventLoop loop;
    QString result;
    connect(this, &player::sendSignal, &loop, [&](QString receivedResult) {
        result = receivedResult;
        loop.quit();
    });
    loop.exec();

    // The returning result will be 1 or 2 (true or false)
    if(result.at(0).digitValue() == RVIC_P) return (result.at(1).digitValue() == 1 ? true : false);

    // If the packet was not correctly received
    return false;
}

bool player::waitPacket(QString dataString)
{
    // Gets the packet code
    int packetCode = getCode(dataString.at(0));
    dataString.removeFirst();

    // dataString current element
    int i = 0;

    // Used to get the returning of somefunctions
    int responseINT;
    bool responseBOOL;

    // Aux. variables
    pos pos;
    QByteArray data;
    QString aux;

    switch (packetCode) {
        case AHIT_P:
            // Gets X pos
            while(dataString.at(i++) != '.') aux.append(dataString.at(i-1));
            pos.x = aux.toInt();
            aux.clear(); // Clears AUX

            // Gets Y pos
            while(dataString.at(i++) != '.') aux.append(dataString.at(i-1));
            pos.y = aux.toInt();
            aux.clear(); // Clears AUX

            // Calculates response
            responseINT = respondHit(pos);

            // Communicates response
            data.append(QString::number(RHIT_P).toUtf8());
            data.append(QString::number(responseINT).toUtf8());
            client->sendData(data);
        break;

        case ADESTROY_P:
            // Gets X pos
            while(dataString.at(i++) != '.') aux.append(dataString.at(i-1));
            pos.x = aux.toInt();
            aux.clear(); // Clears AUX

            // Gets Y pos
            while(dataString.at(i++) != '.') aux.append(dataString.at(i-1));
            pos.y = aux.toInt();
            aux.clear(); // Clears AUX

            // Calculates response
            responseBOOL = respondDestroyed(pos,dataString.at(i).digitValue() == 1 ? true : false);

            // If the ship was indeed destroyed another function will communicate
            // But it returns only after the communication is done
            if(responseBOOL) break;
            // If the ship was not destroy so it returns the negative response
            data.append(QString::number(RDESTROY_P).toUtf8());
            data.append(QString::number(0).toUtf8());
            client->sendData(data);
        break;

        case AVIC_P:
            // Calculates response
            responseBOOL = checkLost();
            // Answers
            data.append(QString::number(RVIC_P).toUtf8());
            data.append(QString::number(responseBOOL ? 1 : 0).toUtf8());
            client->sendData(data);
            return responseBOOL;
        break;

        case (int)ARADAR_P:
            // Gets X pos
            while(dataString.at(i++) != '.') aux.append(dataString.at(i-1));
            pos.x = aux.toInt();
            aux.clear(); // Clears AUX

            // Gets Y pos
            while(dataString.at(i++) != '.') aux.append(dataString.at(i-1));
            pos.y = aux.toInt();
            aux.clear(); // Clears AUX

            // Calculates response
            responseBOOL = respondRadar(pos);
            // Answers
            data.append(QString(RRADAR_P).toUtf8());
            data.append(QString::number(responseBOOL ? 1 : 0).toUtf8());
            client->sendData(data);
        break;

        default:
        break;
    }

    return false;
}
