#include "board.h"

void board::DrawLayout()
{

    loadMisc();

    // Builds the grid
    for(int x = 0; x < NUM_SQUARES; x++)
    {
        for(int y = 0; y < NUM_SQUARES; y++)
        {
            scene->addRect(x*SQUARE,y*SQUARE,SQUARE,SQUARE,*blackLine,QBrush(Qt::transparent));
        }
    }

}

void board::loadMisc()
{
    // Defines the colours used in the board
    redPaint = new QBrush(Qt::red);
    greenPaint = new QBrush(Qt::green);
    oceanPaint = new QBrush(Qt::blue);
    blackLine = new QPen(Qt::black);
    blackLine->setWidth(1);

    // Defines the cross
    QPixmap crossImage;
    crossImage.load(":/resources/cross.png");
    crossImage = crossImage.scaled(SQUARE,SQUARE,Qt::IgnoreAspectRatio);
    cross.setTexture(crossImage);
}
