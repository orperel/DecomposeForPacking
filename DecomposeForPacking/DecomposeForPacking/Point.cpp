#include "Point.h"


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
