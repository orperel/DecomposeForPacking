#pragma once

#include <memory>
#include "IWorldVisitor.h"

class IsFitVisitor : public IWorldVisitor
{
public:
	IsFitVisitor();
	virtual ~IsFitVisitor();
	
	virtual void visit(WorldPtr worldPtr, Point point);
};

