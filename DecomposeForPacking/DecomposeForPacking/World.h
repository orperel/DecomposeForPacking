#pragma once

#include <memory>
#include <unordered_map>
#include "IWorldVisitor.h"
#include "DynamicArray.h"

// TODO: two ways data store - from index to pixel and from pixel to index with map or smtng

/* Represents the world to be decompose or packed into, i.e. the spatial in which we locate the parts that assemble
   the object. The world is composed of transparent and non-transparent pixels; the non-transparents are the pixels
   of the object/pack istself. */
class World
{

public:
	World(BooleanDynamicArrayPtr mat, int numberOfPoints);
	virtual ~World();

	/* Returns true if the point is part of the object, else false. */
	bool isPointPainted(Point point);

	void accept(IWorldVisitorPtr visitor);

	int getNumberOfPoints();

	bool isPointExist(Point point);

	Point getPointFromIndex(int index);

	int getIndexFromPoint(Point point);

private:
	int m_numberOfPoints;

	BooleanDynamicArrayPtr m_mat;

	std::unordered_map<Point, int, PointHash<Point>> m_pointToIndex;
	std::unordered_map<int, Point> m_indexToPoint;
};

typedef std::shared_ptr<World> WorldPtr;

//
///* Returns the world's height. */
//int getHeight();
//
///* Returns the world's width. */
//int getWidth();
