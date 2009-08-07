#ifndef _SHAPEINSTANCE_H_
#define _SHAPEINSTANCE_H_

#include "shape.h"

class ShapeInstance
{
public:
    ShapeInstance(const Shape *);
    ~ShapeInstance(void);

    bool CreateGraphics();
    QGraphicsItem *item() const { return m_pItem; }

    const Shape *shape() const { return m_pShape; }
private:
    const Shape *m_pShape;
    QGraphicsItem *m_pItem;
};

#endif