#include "PartOrientation.h"

// Initialize sequential id counter
int PartOrientation::sequentialPartsOrientationId = 0;

PartOrientation::PartOrientation(int pixelSize /*= 1*/) : m_pixelSize(pixelSize)
{
	m_pointList = PointListPtr(new PointList());
	m_headPointList = PointListPtr(new PointList());
	addPoint(Point(0, 0));

	m_orientationId = sequentialPartsOrientationId;
	sequentialPartsOrientationId++;
}


PartOrientation::PartOrientation(PointListPtr pointList) : m_pointList(pointList)
{
	for each (const Point& point in *m_pointList) {
		m_pointMap[point] = true;
	}

	m_orientationId = sequentialPartsOrientationId;
	sequentialPartsOrientationId++;
}


PartOrientation::~PartOrientation()
{
}

int PartOrientation::addPointToRight(int pointIndex)
{
	addPoint(Point(m_headPointList->at(pointIndex).getX() + m_pixelSize, m_headPointList->at(pointIndex).getY()));

	return m_headPointList->size() - 1;
}

PointListPtr PartOrientation::getPointList()
{
	return m_pointList;
}

void PartOrientation::addPoint(Point startPoint)
{
	m_headPointList->push_back(startPoint);

	for (int i = 0; i < m_pixelSize; i++) {
		for (int j = 0; j < m_pixelSize; j++) {
			Point newPoint(startPoint + Point(i, j));
			m_pointList->push_back(newPoint);
			m_pointMap[newPoint] = true;
		}
	}
}

int PartOrientation::addPointBelow(int pointIndex)
{
	addPoint(Point(m_headPointList->at(pointIndex).getX(), m_headPointList->at(pointIndex).getY() + m_pixelSize));

	return m_headPointList->size() - 1;
}

Point PartOrientation::getAnchor()
{
	return m_pointList->at(0);
}


PartOrientationPtr PartOrientation::verticalMirror()
{
	PointListPtr newPointList = PointListPtr(new PointList());

	for each (const Point& point in *m_pointList) {
		newPointList->push_back(Point(point.getX() * -1, point.getY()));
	}

	return PartOrientationPtr(new PartOrientation(newPointList));
}

PartOrientationPtr PartOrientation::rotate()
{
	PointListPtr newPointList = PointListPtr(new PointList());

	for each (const Point& point in *m_pointList) {
		newPointList->push_back(Point(point.getY(), point.getX()));
	}

	return PartOrientationPtr(new PartOrientation(newPointList));
}

PartOrientationPtr PartOrientation::horizonalMirror()
{
	PointListPtr newPointList = PointListPtr(new PointList());

	for each (const Point& point in *m_pointList) {
		newPointList->push_back(Point(point.getX(), point.getY() * -1));
	}

	return PartOrientationPtr(new PartOrientation(newPointList));
}

bool PartOrientation::isVerticalSymmetrical(PartOrientationPtr partOrient)
{
	int width = 0;
	for each (const Point& point in *getPointList()) {
		if (point.getX() > width) width = point.getX();
	}


	for each (const Point& point in *partOrient->getPointList()) {
		if (m_pointMap.find(Point(point.getX() + width, point.getY())) == m_pointMap.end()) {
			return false;
		}
	}

	return true;
}

bool PartOrientation::isHorizonalSymmetrical(PartOrientationPtr partOrient)
{
	int height = 0;
	for each (const Point& point in *getPointList()) {
		if (point.getY() > height) height = point.getY();
	}

	for each (const Point& point in *partOrient->getPointList()) {
		if (m_pointMap.find(Point(point.getX(), point.getY() + height)) == m_pointMap.end()) {
			return false;
		}
	}

	return true;
}

bool PartOrientation::isSymmetrical()
{
	for each (const Point& point in *m_pointList) {
		if (m_pointMap.find(Point(point.getY(), point.getX())) == m_pointMap.end()) {
			return false;
		}
	}

	return true;
}

bool PartOrientation::isContainsPointAtLocation(int x, int y)
{
	for (auto& orientationPoint : *m_pointList)
	{
		if ((orientationPoint.getX() == x) && (orientationPoint.getY() == y))
			return true;
	}

	return false;
}

bool PartOrientation::isContainsPointAbovePoint(Point point)
{
	int searchedX = point.getX();
	int searchedY = point.getY() + 1;

	return isContainsPointAtLocation(searchedX, searchedY);
}

bool PartOrientation::isContainsPointBelowPoint(Point point)
{
	int searchedX = point.getX();
	int searchedY = point.getY() - 1;

	return isContainsPointAtLocation(searchedX, searchedY);
}

bool PartOrientation::isContainsPointRightOfPoint(Point point)
{
	int searchedX = point.getX() + 1;
	int searchedY = point.getY();

	return isContainsPointAtLocation(searchedX, searchedY);
}

bool PartOrientation::isContainsPointLeftPoint(Point point)
{
	int searchedX = point.getX() - 1;
	int searchedY = point.getY();

	return isContainsPointAtLocation(searchedX, searchedY);
}

const int PartOrientation::getId() const
{
	return m_orientationId;
}