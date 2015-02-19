#include "PackingPartFitVisitor.h"

PackingPartFitVisitor::PackingPartFitVisitor(PartPtr part, int partId, int count, shared_ptr<DLXSolver> dlxSolver,
	SetToPartMapPtr locationSetToPart, SetToOrientationMapPtr locationSetToOrient) :
	AbstractPartFitVisitor(part, dlxSolver, locationSetToPart, locationSetToOrient), _partId(partId), _count(count)
{
}

PackingPartFitVisitor::~PackingPartFitVisitor()
{
}

/** Activates the isFit operation on the box for the given point.
*  In practice, checks if the part which the visitor belongs to, and all of its orientations, can be located in the given
*  point as origin. Meaning that the other points of the part (except for the origin) must be inside the box as well.
*  In addition, updates the maps locationSetToPart and locationSetToOrient if a new location set was found.
*/
void PackingPartFitVisitor::visit(World& box, Point point)
{
	// For each orientation of the part checks if it's fit
	for each (const PartOrientationPtr& partOrient in *_part->getPartOrientations()) {
		shared_ptr<DLX_VALUES_SET> partLocationSet = getLocationSet(box, point, partOrient);
		if (partLocationSet) {	// partLocationSet is not null
			// For each copy of the part adds an appropriate row to the DLXSolver
			for (int copyIndex = 0; copyIndex < _count; copyIndex++)
			{
				DLX_VALUES_SET currCopyLocationSet = *partLocationSet;
				currCopyLocationSet.insert(_partId + copyIndex);	// Adds the ID of the part to the location set
				_dlxSolver->addRow(std::make_shared<DLX_VALUES_SET>(currCopyLocationSet));
				// Maps the new location set to its part
				(*_locationSetToPart)[currCopyLocationSet] = _part;
				// Maps the new location set to its part orientation
				(*_locationSetToOrient)[currCopyLocationSet] = std::make_tuple(partOrient, point);
			}
		}
	}
}