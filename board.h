#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <QString>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QPainter>

#define SQUARE 40
#define NUM_SQUARES 12
#define NUM_SHIP 6

typedef enum
{
    hit,
    miss,
    nothing,

}squareState;

class board
{
public:

    // Scene
    QGraphicsScene *scene;

    // Paints
    QBrush *redPaint;
    QBrush *greenPaint;
    QBrush *oceanPaint;

    // Line
    QPen *blackLine;

    // Cross
    QBrush cross;

    // Draws the squares in the scene
    void DrawLayout();

    // Loads miscellaneous for the board
    void loadMisc();
};

#endif // BOARD_H
