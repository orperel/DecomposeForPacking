#include "PartOrientation.h"


PartOrientation::PartOrientation(int pixelSize /*= 1*/) : m_pixelSize(pixelSize)
{
	addPoint(Point(0, 0));
}


PartOrientation::~PartOrientation()
{
}

int PartOrientation::addPointToRight(int pointIndex)
{
	addPoint(Point(m_headPointList[pointIndex].getX() + m_pixelSize, m_headPointList[pointIndex].getY()));

	return m_headPointList.size() - 1;
}

PointList PartOrientation::getPointList()
{
	return m_pointList;
}

void PartOrientation::addPoint(Point startPoint)
{
	m_headPointList.push_back(startPoint);

	for (int i = 0; i < m_pixelSize; i++) {
		for (int j = 0; j < m_pixelSize; j++) {
			m_pointList.push_back(startPoint + Point(i, j));
		}
	}
}

int PartOrientation::addPointBelow(int pointIndex)
{
	addPoint(Point(m_headPointList[pointIndex].getX(), m_headPointList[pointIndex].getY() + m_pixelSize));

	return m_headPointList.size() - 1;
}
