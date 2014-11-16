#include "world.h"

//World::World(BooleanDynamicArrayPtr mat, int numberOfPoints) : m_mat(mat), m_numberOfPoints(numberOfPoints)
//{
//	int index = 0;
//	for (int i = 0; i < m_mat->getRows(); i++) {
//		for (int j = 0; j < m_mat->getCols(); j++) {
//			Point point(i, j);
//			if ((*m_mat)[point.getX()][point.getY()]) {
//				m_indexToPoint[index] = point;
//				m_pointToIndex[point] = index;
//				index++;
//			}
//		}
//	}
//}

World::World(PointList pointList, int width, int height) : m_pointList(pointList), m_width(width), m_height(height), m_numberOfPoints(pointList.size())
{
	int index = 0;
	for each (const Point& point in m_pointList) {
		m_indexToPoint[index] = point;
		m_pointToIndex[point] = index;
		index++;
	}
}



World::~World()
{
}

/* Accepts the visitor operation on this */
void World::accept(IWorldVisitorPtr visitor)
{
	//for (int i = 0; i < m_mat->getRows(); i++) {
	//	for (int j = 0; j < m_mat->getCols(); j++) {
	//		Point point(i, j);
	//		if (isPointPainted(point)) {
	//			visitor->visit(WorldPtr(this), point);
	//		}
	//	}
	//}

	// For each point in the world activates the visitor operation
	for each (const std::pair<int, Point>& p in m_indexToPoint) {
		visitor->visit(*this, p.second);
	}
}

bool World::isPointExist(Point point)
{
	//return (*m_mat)[point.getX()][point.getY()];
	bool res = (m_pointToIndex.find(point) != m_pointToIndex.end());
	return res;
}

Point World::getPointFromIndex(int index)
{
	return m_indexToPoint[index];
}

int World::getIndexFromPoint(Point point)
{
	return m_pointToIndex.at(point);
}

int World::getNumberOfPoints()
{
	return m_numberOfPoints;
}

int World::getWidth()
{
	return m_width;
}

int World::getHeight()
{
	return m_height;
}

PointList& World::getPointList()
{
	return m_pointList;
}
