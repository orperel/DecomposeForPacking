#include "Point.h"

Point::Point()
{
}


Point::Point(int x, int y, int z /*= 0*/, int color /*= BLACK*/) : m_x(x), m_y(y), m_z(z), m_color(color)
{

}

Point::~Point()
{
}

Point Point::operator+(Point p)
{
	return Point(this->m_x + p.m_x, this->m_y + p.m_y, this->m_z + p.m_z, this->m_color);
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
	return (this->getX() == point.getX() && this->getY() == point.getY() && this->getZ() == point.getZ());
}

int Point::getColor() const
{
	return m_color;
}

int Point::getZ() const
{
	return m_z;
}
