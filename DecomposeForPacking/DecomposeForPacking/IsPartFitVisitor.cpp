#include "IsPartFitVisitor.h"
#include "Part.h"
#include <iostream>

using std::make_pair;

/** Constructs a new visitor of the part partPtr.
 *  Receives a DLXSolver in order to add to it the new location set of the part, if it's fit.
 *  (Location set is a set of indices of the world, instead of points; each point is mapped to unique index.)
 *  Receives also a map that maps the location sets to their part orientations.
 */
IsPartFitVisitor::IsPartFitVisitor(PartPtr part, shared_ptr<DLXSolver> dlxSolver, SetToPartMapPtr locationSetToPart, SetToOrientationMapPtr locationSetToOrient)
{
	_part = part;
	_dlxSolver = dlxSolver;
	_locationSetToPart = locationSetToPart;
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
	// For each orientation of the part checks if it's fit
	for each (const PartOrientationPtr& partOrient in *_part->getPartOrientations()) {
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
			_dlxSolver->addRow(partLocationSet);
			// Maps the new location set to its part
			//PartPtr copyOfPart(_part);
			//_locationSetToPart->insert(std::make_pair<DLX_VALUES_SET, PartPtr>(std::move(*partLocationSet), std::move(copyOfPart)));
			(*_locationSetToPart)[*partLocationSet] = _part;
			// Maps the new location set to its part orientation
			//PartOrientationPtr copyOfPartOrient(partOrient);
			//_locationSetToOrient->insert(std::make_pair<DLX_VALUES_SET, PartOrientationPtr>(std::move(*partLocationSet), std::move(copyOfPartOrient)));
			(*_locationSetToOrient)[*partLocationSet] = partOrient;
		}
	}
}
