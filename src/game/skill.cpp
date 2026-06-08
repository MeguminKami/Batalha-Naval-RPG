#include "skill.h"

int skill::getCost() const
{
    return cost;
}

void skill::setCost(int value)
{
    cost = value < 0 ? 0 : value;
}

pos skill::getAoeSize() const
{
    return aoeSize;
}

void skill::setAoeSize(pos size)
{
    aoeSize.x = size.x < 0 ? 0 : size.x;
    aoeSize.y = size.y < 0 ? 0 : size.y;
}
