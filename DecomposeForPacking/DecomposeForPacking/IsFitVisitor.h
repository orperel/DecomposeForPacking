#pragma once

#include <memory>
#include "IWorldVisitor.h"
#include "Part.h"
#include "World.h"
#include "Point.h"

class IsFitVisitor : public IWorldVisitor
{
public:
	IsFitVisitor(PartPtr partPtr);
	virtual ~IsFitVisitor();
	
	void visit(WorldPtr worldPtr, Point point) override;

private:
	PartPtr _partPtr;
};