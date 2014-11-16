#pragma once

#include "AbstractPartFitVisitor.h"

class DecomposePartFitVisitor : public AbstractPartFitVisitor
{
public:
	DecomposePartFitVisitor(PartPtr part, shared_ptr<DLXSolver> dlxSolver, SetToPartMapPtr locationSetToPart, SetToOrientationMapPtr locationSetToOrient);
	
	virtual ~DecomposePartFitVisitor();
	
	void visit(World& world, Point point) override;
};