#include "AbstractPartFitVisitor.h"

/** Constructs a new visitor of the part partPtr.
 *  Receives a DLXSolver in order to add to it the new location set of the part, if it's fit.
 *  (Location set is a set of indices of the world, instead of points; each point is mapped to unique index.)
 *  Receives also maps that maps the location sets to their parts and part orientations.
 */
AbstractPartFitVisitor::AbstractPartFitVisitor(PartPtr part, shared_ptr<DLXSolver> dlxSolver, SetToPartMapPtr locationSetToPart, SetToOrientationMapPtr locationSetToOrient)
{
	_part = part;
	_dlxSolver = dlxSolver;
	_locationSetToPart = locationSetToPart;
	_locationSetToOrient = locationSetToOrient;
}

AbstractPartFitVisitor::~AbstractPartFitVisitor()
{
}

/** Returns the location set of the partOrient, if it's fit to point. Else returns NULL. */
shared_ptr<DLX_VALUES_SET> AbstractPartFitVisitor::getLocationSet(World& world, Point point, PartOrientationPtr partOrient)
{
	// Gets the point list of the part orientation (composed of origin point and neighboring points in relation to it)
	PointListPtr pointList = partOrient->getPointList();
	// Initializes a new location set to be added to the DLXSolver, if the part orientation is fit
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

	if ((i == pointListSize) && isPartFit)	// The part orientation is fit
	{
		return partLocationSet;
	}
	else
	{
		return NULL;
	}
}
