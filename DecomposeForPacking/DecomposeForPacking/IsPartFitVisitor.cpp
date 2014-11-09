#include "IsPartFitVisitor.h"
#include "Part.h"

IsPartFitVisitor::IsPartFitVisitor(PartPtr partPtr,	shared_ptr<DLXSolver> dlxSolver)
{
	_partPtr = partPtr;
	_dlxSolver = dlxSolver;
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
		partLocationSet->insert(world.getIndexFromPoint(relatedPartPoint));

		if (!world.isPointExist(relatedPartPoint))
		{
			isPartFit = false;
		}

		i++;
	}

	if ((i == pointListSize) && isPartFit)
	{
		//_dlxSolver->addRow(partLocationSet);
	}
}
