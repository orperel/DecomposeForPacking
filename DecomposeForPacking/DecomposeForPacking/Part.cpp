#include "Part.h"

Part::Part(PartOrientationPtr partOrientation)
{
	m_partOrientations = PartOrientationListPtr(new PartOrientationList());
	m_partOrientations->push_back(partOrientation);

	extendPartOrientations();
}

Part::~Part()
{

}

PartOrientationListPtr Part::getPartOrientations()
{
	return m_partOrientations;
}

PartOrientationPtr Part::getPartOrientationByIndex(int index)
{
	return m_partOrientations->at(index);
}

void Part::extendPartOrientations()
{
	PartOrientationPtr partOrient = getPartOrientationByIndex(0);
	PartOrientationPtr mirroredOrientation = extendMirrorOrientation(partOrient);
	PartOrientationPtr rotatedOrientation = extendRotateOrientation(partOrient);
	PartOrientationPtr rotatedMirroredOrientation = extendRotateOrientation(mirroredOrientation);

	m_partOrientations->push_back(mirroredOrientation);
	m_partOrientations->push_back(rotatedOrientation);
	m_partOrientations->push_back(rotatedMirroredOrientation);
}

PartOrientationPtr Part::extendMirrorOrientation(PartOrientationPtr partOrient)
{
	PointListPtr newPointList = PointListPtr(new PointList());

	for each (const Point& point in *partOrient->getPointList()) {
		newPointList->push_back(Point(point.getX() * -1, point.getY()));
	}

	return PartOrientationPtr(new PartOrientation(newPointList));

}

PartOrientationPtr Part::extendRotateOrientation(PartOrientationPtr partOrient)
{
	PointListPtr newPointList = PointListPtr(new PointList());

	for each (const Point& point in *partOrient->getPointList()) {
		newPointList->push_back(Point(point.getY(), point.getX()));
	}

	return PartOrientationPtr(new PartOrientation(newPointList));
}
