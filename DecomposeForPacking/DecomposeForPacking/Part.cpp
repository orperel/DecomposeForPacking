#include "Part.h"


Part::Part(int pixelSize /*= 1*/) : m_pixelSize(pixelSize)
{
	addPoint(Point(0, 0));
	m_headPointList.push_back(Point(0, 0));
}


Part::~Part()
{
}

int Part::addPointToRight(int pointIndex)
{
	addPoint(Point(m_headPointList[pointIndex].getX() + 10, m_headPointList[pointIndex].getY()));
	m_headPointList.push_back(Point(m_headPointList[pointIndex].getX() + 10, m_headPointList[pointIndex].getY()));

	return m_headPointList.size() - 1;
}

PointList Part::getPointList()
{
	return m_pointList;
}

void Part::addPoint(Point startPoint)
{
	for (int i = 0; i < m_pixelSize; i++) {
		for (int j = 0; j < m_pixelSize; j++) {
			m_pointList.push_back(startPoint + Point(i, j));
		}
	}
}
