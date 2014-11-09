#pragma once

#include <memory>
#include "IWorldVisitor.h"
#include "Part.h"
#include "DLXSolver.h"
#include "Decompose.h"
#include "World.h"
#include "Point.h"

using std::shared_ptr;

class IsPartFitVisitor : public IWorldVisitor
{
public:
	IsPartFitVisitor(PartPtr partPtr, shared_ptr<DLXSolver> dlxSolver, SetsToPartMapPtr locationSetToPart);
	virtual ~IsPartFitVisitor();
	
	void visit(World& world, Point point) override;

private:
	PartPtr _partPtr;
	shared_ptr<DLXSolver> _dlxSolver;
	SetsToPartMapPtr _locationSetToPart;
};