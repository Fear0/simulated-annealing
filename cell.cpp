#include "cell.h"
#include <utility>

bool Coordinates::operator< (const Coordinates& c)
{
    return (this->x < c.x )&& (this->y < c.y);
}

unsigned int Cell::getXlength () const
{
    return this->xlength;
}

unsigned int Cell::getYlength () const
{
    return this->ylength;
}

const std::string& Cell::getName () const
{
    return this->name;
}

bool operator==(const Cell& c1,const Cell& c2)
{
    return((c1.getName()==c2.getName())&&(c1.getXlength()==c2.getXlength())&&(c1.getYlength()==c2.getYlength()));
}

void Cell::rotate()
{
    std::swap(this->xlength, this->ylength);
}

