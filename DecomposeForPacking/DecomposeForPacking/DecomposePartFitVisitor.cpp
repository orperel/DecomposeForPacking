#include "DecomposePartFitVisitor.h"
#include "Part.h"

using std::make_tuple;

DecomposePartFitVisitor::DecomposePartFitVisitor(PartPtr part, shared_ptr<DLXSolver> dlxSolver,
	SetToPartMapPtr locationSetToPart, SetToOrientationMapPtr locationSetToOrient):
	AbstractPartFitVisitor(part, dlxSolver, locationSetToPart, locationSetToOrient)
{
}

DecomposePartFitVisitor::~DecomposePartFitVisitor()
{
}

/** Activates the isFit operation on the world for the given point.
 *  In practice, checks if the part which the visitor belongs to can be located in the given point as origin.
 *  Meaning that the other points of the part (except for the origin) must be inside the world as well.
 */
void DecomposePartFitVisitor::visit(World& world, Point point)
{
	// For each orientation of the part checks if it's fit
	for each (const PartOrientationPtr& partOrient in *_part->getPartOrientations()) {
		shared_ptr<DLX_VALUES_SET> partLocationSet = getLocationSet(world, point, partOrient);
		_dlxSolver->addRow(partLocationSet);
		// Maps the new location set to its part
		(*_locationSetToPart)[*partLocationSet] = _part;
		// Maps the new location set to its part orientation
		(*_locationSetToOrient)[*partLocationSet] = std::make_tuple(partOrient, point);
	}
}
