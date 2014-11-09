#include "IsFitVisitor.h"
#include "Part.h"

using std::unique_ptr;

IsFitVisitor::IsFitVisitor(PartPtr partPtr,	shared_ptr<DLXSolver> dlxSolver)
{
	_partPtr = partPtr;
	_dlxSolver = dlxSolver;
}

IsFitVisitor::~IsFitVisitor()
{
}

void IsFitVisitor::visit(WorldPtr worldPtr, Point point)
{
	PointList pointList = _partPtr->getPointList();
	unique_ptr<DLX_VALUES_SET> partLocationSet = std::make_unique<DLX_VALUES_SET>();

	int i = 0;
	int pointListSize = pointList.size();
	bool isPartFit = true;
	while ((i < pointListSize) && isPartFit)
	{
		Point relatedPartPoint = (point + pointList[i]);
		partLocationSet->insert(worldPtr->pointToIndex(relatedPartPoint));

		if (!worldPtr->isPointPainted(relatedPartPoint))
		{
			isPartFit = false;
		}
	}

	if ((i == pointListSize) && isPartFit)
	{
		_dlxSolver->addRow(partLocationSet);
	}
}
