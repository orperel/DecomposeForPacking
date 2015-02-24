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

	bool isContainsPointAbovePoint(Point);
	bool isContainsPointBelowPoint(Point);
	bool isContainsPointRightOfPoint(Point);
	bool isContainsPointLeftPoint(Point);
	const int getId() const;

	bool operator!=(PartOrientation& partOrient) const;

private:
	PointListPtr m_pointList;
	std::unordered_map<Point, bool, PointHash<Point>> m_pointMap;

	PointListPtr m_headPointList;

	int m_pixelSize;

	int m_orientationId;
	static int sequentialPartsOrientationId;

	void addPoint(Point startPoint);


	PartOrientationPtr XYRotate();
	PartOrientationPtr YZMirror();
	PartOrientationPtr XZMirror();
	PartOrientationPtr ZYRotate();
	PartOrientationPtr ZXRotate();

	bool isContainsPointAtLocation(int x, int y);

	friend Part;
};


// TODO: add lock part orientation so addPoint won't work