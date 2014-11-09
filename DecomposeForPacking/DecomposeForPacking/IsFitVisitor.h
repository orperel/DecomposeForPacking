#pragma once

#include <memory>
#include "IWorldVisitor.h"
#include "Part.h"
#include "DLXSolver.h"
#include "World.h"
#include "Point.h"

using std::shared_ptr;

class IsFitVisitor : public IWorldVisitor
{
public:
	IsFitVisitor(PartPtr partPtr, shared_ptr<DLXSolver> dlxSolver);
	virtual ~IsFitVisitor();
	
	void visit(WorldPtr worldPtr, Point point) override;

private:
	PartPtr _partPtr;
	shared_ptr<DLXSolver> _dlxSolver;
};