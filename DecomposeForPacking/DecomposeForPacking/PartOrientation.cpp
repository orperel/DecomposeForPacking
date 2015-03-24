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

PartOrientationPtr PartOrientation::XYRotate()
{
	PointListPtr tempPointList = PointListPtr(new PointList());

	int minX = 0;
	int minY = 0;
	int minZ = 0;

	for each (const Point& point in *m_pointList) {
		Point newPoint(point.getY()*-1, point.getX(), point.getZ());
		tempPointList->push_back(newPoint);

		if (newPoint.getX() < minX) minX = newPoint.getX();
		if (newPoint.getY() < minY) minY = newPoint.getY();
		if (newPoint.getZ() < minZ) minZ = newPoint.getZ();

	}

	PointListPtr newPointList = PointListPtr(new PointList());

	for each (const Point& point in *tempPointList) {
		Point newPoint(-1 * minX + point.getX(), -1 * minY + point.getY(), -1 * minZ + point.getZ());
		newPointList->push_back(newPoint);
	}

	return PartOrientationPtr(new PartOrientation(newPointList));
}

bool PartOrientation::operator!=(PartOrientation& partOrient) const
{
	PointListPtr partOrientPointList = partOrient.getPointList();

	for each (const Point& point in *partOrientPointList) {
		if (m_pointMap.find(point) == m_pointMap.end()) {
			return true;
		}
	}

	return false;
}

PartOrientationPtr PartOrientation::ZYRotate()
{
	PointListPtr tempPointList = PointListPtr(new PointList());

	int minX = 0;
	int minY = 0;
	int minZ = 0;

	for each (const Point& point in *m_pointList) {
		Point newPoint(point.getX(), point.getZ(), point.getY()*-1);
		tempPointList->push_back(newPoint);

		if (newPoint.getX() < minX) minX = newPoint.getX();
		if (newPoint.getY() < minY) minY = newPoint.getY();
		if (newPoint.getZ() < minZ) minZ = newPoint.getZ();

	}

	PointListPtr newPointList = PointListPtr(new PointList());

	for each (const Point& point in *tempPointList) {
		Point newPoint(-1 * minX + point.getX(), -1 * minY + point.getY(), -1 * minZ + point.getZ());
		newPointList->push_back(newPoint);
	}

	return PartOrientationPtr(new PartOrientation(newPointList));
}

PartOrientationPtr PartOrientation::ZXRotate()
{
	PointListPtr tempPointList = PointListPtr(new PointList());

	int minX = 0;
	int minY = 0;
	int minZ = 0;

	for each (const Point& point in *m_pointList) {
		Point newPoint(point.getZ(), point.getY(), point.getX()*-1);
		tempPointList->push_back(newPoint);

		if (newPoint.getX() < minX) minX = newPoint.getX();
		if (newPoint.getY() < minY) minY = newPoint.getY();
		if (newPoint.getZ() < minZ) minZ = newPoint.getZ();

	}

	PointListPtr newPointList = PointListPtr(new PointList());

	for each (const Point& point in *tempPointList) {
		Point newPoint(-1 * minX + point.getX(), -1 * minY + point.getY(), -1 * minZ + point.getZ());
		newPointList->push_back(newPoint);
	}

	return PartOrientationPtr(new PartOrientation(newPointList));
}

PartOrientationPtr PartOrientation::YZMirror()
{
	PointListPtr newPointList = PointListPtr(new PointList());

	for each (const Point& point in *m_pointList) {
		newPointList->push_back(Point(point.getX(), point.getZ(), point.getY()));
	}

	return PartOrientationPtr(new PartOrientation(newPointList));
}

PartOrientationPtr PartOrientation::XZMirror()
{
	PointListPtr newPointList = PointListPtr(new PointList());

	for each (const Point& point in *m_pointList) {
		newPointList->push_back(Point(point.getZ(), point.getY(), point.getX()));
	}

	return PartOrientationPtr(new PartOrientation(newPointList));
}

const int PartOrientation::getPartId() const
{
	return m_partId;
}

void PartOrientation::setPartId(int partId)
{
	m_partId = partId;
}
