#include "world.h"

World::World(BooleanDynamicArrayPtr mat, int numberOfPoints) : m_mat(mat), m_numberOfPoints(numberOfPoints)
{
	int index = 0;
	for (int i = 0; i < m_mat->getRows(); i++) {
		for (int j = 0; j < m_mat->getCols(); j++) {
			Point point(i, j);
			if ((*m_mat)[point.getX()][point.getY()]) {
				m_indexToPoint[index] = point;
				m_pointToIndex[point] = index;
				index++;
			}
		}
	}
}


World::~World()
{
}

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

	for each (const std::pair<int, Point>& p in m_indexToPoint) {
		visitor->visit(WorldPtr(this), p.second);
	}
}

bool World::isPointExist(Point point)
{
	//return (*m_mat)[point.getX()][point.getY()];
	return m_pointToIndex.find(point) != m_pointToIndex.end();
}

Point World::getPointFromIndex(int index)
{
	return m_indexToPoint[index];
}

int World::getIndexFromPoint(Point point)
{
	return m_pointToIndex[point];
}