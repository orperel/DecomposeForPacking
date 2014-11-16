#include "IsPartFitVisitor.h"
#include "Part.h"
#include <iostream>

using std::make_pair;

/** Constructs a new visitor of the part partPtr.
 *  Receives a DLXSolver in order to add to it the new location set of the part, if it's fit.
 *  (Location set is a set of indices of the world, instead of points; each point is mapped to unique index.)
 *  Receives also a map that maps the location sets to their part orientations.
 */
IsPartFitVisitor::IsPartFitVisitor(PartOrientationPtr partOrient, shared_ptr<DLXSolver> dlxSolver, SetsToOrientationMapPtr locationSetToOrient)
{
	_partOrient = partOrient;
	_dlxSolver = dlxSolver;
	_locationSetToOrient = locationSetToOrient;
}

/** Dtor to release resources allocated by the visitor. */
IsPartFitVisitor::~IsPartFitVisitor()
{
}

/** Activates the isFit operation on the world for the given point.
 *  In practice, checks if the part which the visitor belongs to can be located in the given point as origin.
 *  Meaning that the other points of the part (except for the origin) must be inside the world as well.
 */
void IsPartFitVisitor::visit(World& world, Point point)
{
	// Gets the point list of the part (composed of origin point and neighboring points in relation to it)
	PointListPtr pointList = _partOrient->getPointList();
	// Initializes a new location set to be added to the DLXSolver, if the part is fit
	shared_ptr<DLX_VALUES_SET> partLocationSet = std::make_shared<DLX_VALUES_SET>();

	int i = 0;
	int pointListSize = pointList->size();
	bool isPartFit = true;
	while ((i < pointListSize) && isPartFit)
	{
		Point relatedPartPoint = (point + (*pointList)[i]);	// Recomputes the current point of the part in relation to the world

		if (!world.isPointExist(relatedPartPoint))	// The current point doesn't exist in the world
		{
			isPartFit = false;
			break;
		}

		partLocationSet->insert(world.getIndexFromPoint(relatedPartPoint));	// Add the index of the part point to the location set

		i++;
	}


	if ((i == pointListSize) && isPartFit)	// The part is fit
	{
		_dlxSolver->addRow(partLocationSet);
		// Maps the new location set to its part orientation
		PartOrientationPtr copyOfPartOrient(_partOrient);
		_locationSetToOrient->insert(std::make_pair<DLX_VALUES_SET, PartOrientationPtr>(std::move(*partLocationSet), std::move(copyOfPartOrient)));
	}
}
