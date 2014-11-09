#include "IsFitVisitor.h"
#include "Part.h"

IsFitVisitor::IsFitVisitor(PartPtr partPtr)
{
	_partPtr = partPtr;
}

IsFitVisitor::~IsFitVisitor()
{
}

void IsFitVisitor::visit(WorldPtr worldPtr, Point point)
{
	PointList pointList = _partPtr->getPointList();

	int index = 0;
	int pointListSize = pointList.size();
	bool isFit = true;
	while ((index < pointListSize) && isFit)
	{
		if (!worldPtr->isPointPainted(point + pointList[index]))
		{
			isFit = false;
		}
	}

	if ((index == pointListSize) && isFit)
	{

	}
}
