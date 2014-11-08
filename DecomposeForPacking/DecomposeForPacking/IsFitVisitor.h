#pragma once

#include "IWorldVisitor.h"
#include "World.h"
#include "Point.h"

class IsFitVisitor : public IWorldVisitor
{
public:
	IsFitVisitor();
	virtual ~IsFitVisitor();
	
	void visit(World world, Point point);
};

