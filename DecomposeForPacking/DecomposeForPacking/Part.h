#pragma once
#include "Point.h"
#include <vector>
#include <memory>

class Part
{
public:
	Part(int pixelSize = 1);
	virtual ~Part();

	PointList getPointList();

	int addPointToRight(int pointIndex);

private:
	PointList m_pointList;

	PointList m_headPointList;

	int m_pixelSize;

	void addPoint(Point startPoint);
};

typedef std::shared_ptr<Part> PartPtr;
typedef std::vector<PartPtr> PartList;

// TODO: check symmetrical...
