#include "objects.h"

// Returns the number of times a "number" can be divided by the "divisor"
int objects::modDivision(int number, int divisor)
{
    if(divisor == 0) return 0; // If zero returns 0
    else if(divisor < 0) divisor = 0-divisor; // If negative turns positive

    int count = 0;
    while(number >= divisor)
    {
        number = number - divisor;
        count++;
    }
    return count;
}

void objects::rotateObject()
{
    // Creates a new rotated ship image
    QPixmap pixmap = this->brush().texture();
    QTransform transform;
    if(rotated) transform.rotate(-90);
    else transform.rotate(90);
    pixmap = pixmap.transformed(transform);

    // Rotates the ship and sets the new rotated image
    this->setRect(0,0,this->rect().height(),this->rect().width());
    this->setBrush(pixmap);

    // Resets
    rotated = !rotated;
}

void objects::getSquares()
{
    squares square;
    mySquares.clear();
    for(int i = this->pos().x()/SQUARE; i < this->pos().x()/SQUARE + this->rect().width()/SQUARE; i++)
    {
        if(i < 0 || i >= NUM_SQUARES) continue; // Out-of-bounds
        for(int j = this->pos().y()/SQUARE; j < this->pos().y()/SQUARE + this->rect().height()/SQUARE; j++)
        {
            if(j < 0 || j >= NUM_SQUARES) continue; // Out-of-bounds
            square.pos.x = i;
            square.pos.y = j;
            square.hit = false;
            mySquares.append(square);
        }
    }
}

void objects::resetObject()
{
    sunk = false;
    canPlace = false;
    canRotate = false;
    rotated = false;
    mySquares.clear();
    myID = -1;
}

void objects::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Left button click -> takes the ship in hold
    if(event->button() == Qt::LeftButton)
    {
        event->accept();
        // Offset position
        initialClickPosition = event->scenePos() - pos();
        // Thats the layer of the ship
        normalZValue = this->zValue();
        // Saves the ship initial position
        oldPos = this->pos();
        // Removes the ship from the boolean map
        emit setBool(this->myID,false);
    }

    // Rotates the object
    if((event->button() == Qt::RightButton) && (event->buttons() & Qt::LeftButton))
    {

        // Accepts event
        event->accept();

        // Rotates object
        rotateObject();

        // Places the ship on the mouse
        this->setPos(event->scenePos());

        // Paints the shadow
        emit shipMoving(modDivision(this->scenePos().x(),SQUARE) * SQUARE,modDivision(this->scenePos().y(),SQUARE) * SQUARE,this->myID);

        // If canPlace then the object canRotate
        canRotate = canPlace;

        // Resets initalClickPosition
        initialClickPosition = {0,0};
    }
}

void objects::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

    // Moves the ship along the map
    if(event->buttons() & Qt::LeftButton)
    {
        // Accepts the event
        event->accept();

        // Inital offset
        this->setPos(event->scenePos() - initialClickPosition);

        // Places the ship that is beeing moved in the top scene layer
        this->setZValue(1000);

        // Calculates new position if the ship is dropped
        // X pos
        if((this->scenePos().x() + rect().width()) > SQUARE*NUM_SQUARES) newPos.setX(SQUARE*NUM_SQUARES-rect().width());
        else if(this->scenePos().x() <= 0) newPos.setX(0);
        else newPos.setX(modDivision(this->scenePos().x(),SQUARE) * SQUARE);
        // Y pos
        if((this->scenePos().y() + rect().height()) > SQUARE*NUM_SQUARES) newPos.setY(SQUARE*NUM_SQUARES-rect().height());
        else if(this->scenePos().y() <= 0) newPos.setY(0);
        else newPos.setY(modDivision(this->scenePos().y(),SQUARE) * SQUARE);

        // Emits the signal so the higher layer can validate each new position
        emit shipMoving(newPos.x(),newPos.y(),this->myID);

    }
}

void objects::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // If the left button was previously pressed and is now released
    if (event->button() == Qt::LeftButton)
    {
        // Accepts the event
        event->accept();

        // Rotates the object back if it cant be rotated to the current pos
        if(!canRotate && !canPlace) rotateObject();
        canRotate = true;

        // Updates the position and signals the upper layer
        if(canPlace) this->setPos(newPos);
        else this->setPos(oldPos);

        // Builds the actives positions vector
        getSquares();

        // Resets the variable
        canPlace = false;

        // Emits the signal so the higher layer can remove the shadow and update the boolean map
        emit setBool(this->myID,true);

        // Returns the ship to the scene it belongs
        this->setZValue(normalZValue);

        // Resets initial mouse postiion
        initialClickPosition = {0,0};
    }
}
