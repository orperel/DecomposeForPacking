#include "IsPartFitVisitor.h"
#include "Part.h"
#include <iostream>

using std::make_pair;

IsPartFitVisitor::IsPartFitVisitor(PartPtr partPtr,	shared_ptr<DLXSolver> dlxSolver, SetsToPartMapPtr locationSetToPart)
{
	_partPtr = partPtr;
	_dlxSolver = dlxSolver;
	_locationSetToPart = locationSetToPart;
}

IsPartFitVisitor::~IsPartFitVisitor()
{
}

void IsPartFitVisitor::visit(World& world, Point point)
{
	PointList pointList = _partPtr->getPointList();
	shared_ptr<DLX_VALUES_SET> partLocationSet = std::make_shared<DLX_VALUES_SET>();

	int i = 0;
	int pointListSize = pointList.size();
	bool isPartFit = true;
	while ((i < pointListSize) && isPartFit)
	{
		Point relatedPartPoint = (point + pointList[i]);

		if (!world.isPointExist(relatedPartPoint))
		{
			isPartFit = false;
			break;
		}

		partLocationSet->insert(world.getIndexFromPoint(relatedPartPoint));

		i++;
	}


	if ((i == pointListSize) && isPartFit)
	{
		//_dlxSolver->addRow(partLocationSet);
		_locationSetToPart->insert(std::make_pair<DLX_VALUES_SET, PartPtr>(std::move(*partLocationSet), std::move(_partPtr)));
	}

}
