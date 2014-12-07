#include "world.h"

World::World(PointListPtr pointList, int width, int height, int depth /*= 1*/) : m_pointList(pointList), m_width(width), m_height(height), m_depth(depth)
{
	int index = 0;
	for each (const Point& point in *m_pointList) {
		m_indexToPoint[index] = point;
		m_pointToIndex[point] = index;
		index++;
	}
}


World::~World()
{
}

void World::accept(IWorldVisitorPtr visitor)
{
	for each (const std::pair<int, Point>& p in m_indexToPoint) {
		visitor->visit(*this, p.second);
	}
}

bool World::isPointExist(Point point)
{
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
	return m_pointList->size();
}

int World::getWidth()
{
	return m_width;
}

int World::getHeight()
{
	return m_height;
}

PointListPtr World::getPointList()
{
	return m_pointList;
}

int World::getDepth()
{
	return m_depth;
}
