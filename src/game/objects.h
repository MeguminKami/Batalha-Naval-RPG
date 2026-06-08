#ifndef OBJECTS_H
#define OBJECTS_H
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QGraphicsTransform>
#include <QPainter>
#include <QCursor>
#include <QApplication>
#include <QPlainTextEdit>
#include <QKeyEvent>
#include "position.h"

#define NUM_SQUARES 12
#define SQUARE 40

class objects : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    // Is the object sunk
    bool sunk;

    // Can place in the mouseRelease event
    bool canPlace;

    // Can rotate in the mouseRelease event
    bool canRotate;

    // Is the object rotated
    bool rotated;   // Objects can only be rotated or not (0ª or 90ª)
    // Rotates the object
    void rotateObject();

    // Click position to be used in the placement phase
    QPointF initialClickPosition;
    // New position to be used in the placement phase
    QPointF newPos;
    // Saves the position of the object prio of it beeing moved
    QPointF oldPos;

    // Saves the active grid squares of this object
    // If this vector is empty the object is sunk
    QVector<squares> mySquares;
    void getSquares();

    // Object ID
    // the ID defines the his size in width and height
    // Objects witht the same ID have the same width and heigh
    int myID;

    // Objects layer on the board scene
    int normalZValue;

    // Returns how many times the "number" can be divided by the "divisor"
    int modDivision(int number, int divisor);

    // Reset all
    void resetObject();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override; //Mouse press event

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override; // Mouse move while pressed event

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;   // Mouse realease event

signals:
    // Used to comunicate with the higher layer "player.h"
    void shipMoving(int x, int y, int id);
    void setBool(int ID, bool state);
};

#endif // OBJECTS_H
