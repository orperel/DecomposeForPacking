#pragma once
#include "Point.h"
#include <vector>
#include <memory>

class PartOrientation
{
public:
	PartOrientation(int pixelSize = 1);
	virtual ~PartOrientation();

	PointList getPointList();

	int addPointToRight(int pointIndex);

	int addPointBelow(int pointIndex);

private:
	PointList m_pointList;

	PointList m_headPointList;

	int m_pixelSize;

	void addPoint(Point startPoint);
};

typedef std::shared_ptr<PartOrientation> PartOrientationPtr;
typedef std::vector<PartOrientationPtr> PartOrientationList;
typedef std::shared_ptr<PartOrientationList> PartOrientationListPtr;

// TODO: check symmetrical...
