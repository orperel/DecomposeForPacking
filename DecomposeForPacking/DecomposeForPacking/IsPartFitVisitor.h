#pragma once

#include <memory>
#include "IWorldVisitor.h"
#include "Part.h"
#include "DLXSolver.h"
#include "Decompose.h"
#include "World.h"
#include "Point.h"

using std::shared_ptr;

/** Implements the IWorldVisitor as a checker if a part is fit to the given point and its relevant neighbors. */
class IsPartFitVisitor : public IWorldVisitor
{
public:
	/** Constructs a new visitor of the part partPtr.
	 *  Receives a DLXSolver in order to add to it the new location set of the part, if it's fit.
	 *  (Location set is a set of indices of the world, instead of points; each point is mapped to unique index.)
	 *  Receives also a map that maps the location sets to their part orientations.
	 */
	IsPartFitVisitor(PartPtr part, shared_ptr<DLXSolver> dlxSolver, SetToPartMapPtr locationSetToPart, SetToOrientationMapPtr locationSetToOrient);
	
	/** Dtor to release resources allocated by the visitor. */
	virtual ~IsPartFitVisitor();
	
	/* Activates the isFit operation on the world for the given point. */
	void visit(World& world, Point point) override;

private:
	PartPtr _part;	// The part which the visitor belongs to
	shared_ptr<DLXSolver> _dlxSolver;	// The DLXSolver that saves the fitting locations
	SetToPartMapPtr _locationSetToPart;	// The map that maps a location set to its part
	SetToOrientationMapPtr _locationSetToOrient;	// The map that maps a location set to its part orientation
};