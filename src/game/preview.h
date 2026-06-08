#ifndef PREVIEW_H
#define PREVIEW_H

#include <QGraphicsRectItem>
#include <QPointF>

class preview : public QGraphicsRectItem
{
public:
    explicit preview(QGraphicsItem *parent = nullptr);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QPointF dragOffset;

    QPointF snappedPosition(const QPointF &position) const;
};

#endif // PREVIEW_H
