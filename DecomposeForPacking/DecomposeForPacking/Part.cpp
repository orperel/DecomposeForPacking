#include "Part.h"


Part::Part()
{
	m_pointList.push_back(Point(0, 0));
}


Part::~Part()
{
}

int Part::addPointToRight(int pointIndex)
{
	m_pointList.push_back(Point(m_pointList[pointIndex].getX() + 1, m_pointList[pointIndex].getY()));

	return m_pointList.size() - 1;
}

PointList Part::getPointList()
{
	return m_pointList;
}
