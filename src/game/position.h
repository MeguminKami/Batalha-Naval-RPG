#ifndef POSITION_H
#define POSITION_H

struct pos
{
    int x = 0;
    int y = 0;
};

struct squares
{
    ::pos pos;
    bool hit = false;
};

#endif // POSITION_H
