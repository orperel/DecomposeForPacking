#include "world.h"

World::World(PointListPtr pointList, int width, int height, int depth /*= 1*/, int pixelResolution) :
m_pointList(pointList), m_width(width), m_height(height), m_depth(depth), m_pixelResolution(pixelResolution)
{
	for (int i = 0; i < m_pointList->size(); i++) {
		m_pointToIndex[(*m_pointList)[i]] = i;
	}
}

World::World(WorldPtr otherWorld):
World(PointListPtr(new PointList(*otherWorld->getPointList())), otherWorld->getWidth(), otherWorld->getHeight(), otherWorld->getDepth())
{
	
}


World::~World()
{
}

void World::accept(IWorldVisitorPtr visitor)
{
	for each (const Point& p in *m_pointList) {
		visitor->visit(*this, p);
	}
}

bool World::isPointExist(Point point)
{
	bool res = (m_pointToIndex.find(point) != m_pointToIndex.end());
	return res;
}

Point World::getPointFromIndex(int index)
{
	return (*m_pointList)[index];
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

int World::getPixelResolution()
{
	return m_pixelResolution;
}

PointListPtr World::getPointList()
{
	return m_pointList;
}

int World::getDepth()
{
	return m_depth;
}

void World::deletePoint(Point point)
{
	if (isPointExist(point)) {
		int index = getIndexFromPoint(point);

		m_pointList->erase(m_pointList->begin() + index);

		m_pointToIndex.erase(point);
	}	
}
