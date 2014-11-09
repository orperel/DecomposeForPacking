#include "Point.h"

Point::Point()
{
}

Point::Point(int x, int y) : m_x(x), m_y(y)
{
}

Point::~Point()
{
}

Point Point::operator+(Point p)
{
	return Point(this->m_x + p.m_x, this->m_y + p.m_y);
}

int Point::getX() const
{
	return m_x;
}

int Point::getY() const
{
	return m_y;
}


bool Point::operator==(const Point& point) const
{
	return (this->getX() == point.getX() && this->getY() == point.getY());
}

