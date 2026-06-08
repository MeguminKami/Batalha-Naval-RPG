#include "preview.h"

#include <QGraphicsSceneMouseEvent>
#include <QtGlobal>

#include "board.h"

preview::preview(QGraphicsItem *parent)
    : QGraphicsRectItem(parent)
{
}

void preview::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragOffset = event->pos();
        event->accept();
        return;
    }

    QGraphicsRectItem::mousePressEvent(event);
}

void preview::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        setPos(snappedPosition(event->scenePos() - dragOffset));
        event->accept();
        return;
    }

    QGraphicsRectItem::mouseMoveEvent(event);
}

void preview::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        setPos(snappedPosition(pos()));
        event->accept();
        return;
    }

    QGraphicsRectItem::mouseReleaseEvent(event);
}

QPointF preview::snappedPosition(const QPointF &position) const
{
    const qreal maxX = qMax<qreal>(0, NUM_SQUARES * SQUARE - rect().width());
    const qreal maxY = qMax<qreal>(0, NUM_SQUARES * SQUARE - rect().height());
    const qreal snappedX = qRound(position.x() / SQUARE) * SQUARE;
    const qreal snappedY = qRound(position.y() / SQUARE) * SQUARE;

    return {
        qBound<qreal>(0, snappedX, maxX),
        qBound<qreal>(0, snappedY, maxY),
    };
}
