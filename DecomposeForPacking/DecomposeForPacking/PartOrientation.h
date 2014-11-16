#pragma once
#include "Point.h"
#include <vector>
#include <memory>
#include <unordered_map>

class Part;
typedef std::shared_ptr<Part> PartPtr;

class PartOrientation;
typedef std::shared_ptr<PartOrientation> PartOrientationPtr;
typedef std::vector<PartOrientationPtr> PartOrientationList;
typedef std::shared_ptr<PartOrientationList> PartOrientationListPtr;

class PartOrientation
{
public:
	PartOrientation(int pixelSize = 1);
	PartOrientation(PointListPtr pointList);

	virtual ~PartOrientation();

	PointListPtr getPointList();

	int addPointToRight(int pointIndex);

	int addPointBelow(int pointIndex);

	Point getAnchor();


private:
	PointListPtr m_pointList;
	std::unordered_map<Point, bool, PointHash<Point>> m_pointMap;

	PointListPtr m_headPointList;

	int m_pixelSize;

	void addPoint(Point startPoint);


	PartOrientationPtr rotate();
	PartOrientationPtr verticalMirror();
	PartOrientationPtr horizonalMirror();

	bool isVerticalSymmetrical();
	bool isHorizonalSymmetrical();
	bool isSymmetrical();

	friend Part;
};


// TODO: check symmetrical...

// TODO: add lock part orientation so addPoint won't work