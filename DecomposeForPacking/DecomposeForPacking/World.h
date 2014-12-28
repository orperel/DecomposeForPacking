#pragma once

#include <memory>
#include <unordered_map>
#include "IWorldVisitor.h"

/* Represents the world to be decompose or packed into,
   i.e. the spatial in which we locate the parts that assemble the object.
   The world is composed of points, each point gets an index. */
class World
{

public:
	/*
	 * C'tor - Builds world from list of points, width and height and depth
	 */
	World(PointListPtr points, int width, int height, int pixelResolution, int depth = 1);

	/*
	 * Default D'tor
	 */
	virtual ~World();

	/*
	 * Activates the visitor operation for each point in the world 
	 */
	void accept(IWorldVisitorPtr visitor);

	/*
	 * Return the number of points in the world.
	 */
	int getNumberOfPoints();

	/*
	 * Checks if the point exist in the world.
	 */
	bool isPointExist(Point point);

	/*
	 * Get point from index.
	 */
	Point getPointFromIndex(int index);

	/*
	 * Get index from point.
	 */
	int getIndexFromPoint(Point point);

	/*
	 * Width getter.
	 */
	int getWidth();

	/*
	* Height getter.
	*/
	int getHeight();

	/*
	* Depth getter.
	*/
	int getDepth();

	/*
	* Pixel Resolution getter.
	*/
	int getPixelResolution();

	/*
	* World's point list getter.
	*/
	PointListPtr getPointList();

private:
	int m_width; // Width
	int m_height; // Height
	int m_depth; // Depth

	// Pixel resolution of "how many pixels in the real object are mapped to a point in the world"
	int m_pixelResolution;
	PointListPtr m_pointList; // Point List
	std::unordered_map<Point, int, PointHash<Point>> m_pointToIndex; // Map of point to index
	std::unordered_map<int, Point> m_indexToPoint; // Map of index to point
};

typedef std::shared_ptr<World> WorldPtr;