#ifndef SKILL_H
#define SKILL_H

#include <QString>

#include "position.h"

class skill
{
public:
    QString name;
    QString description;
    int cooldown = 0;
    bool used = false;
    bool placed = false;
    bool destroy = false;

    int getCost() const;
    void setCost(int value);

    pos getAoeSize() const;
    void setAoeSize(pos size);

private:
    int cost = 0;
    pos aoeSize;
};

#endif // SKILL_H
