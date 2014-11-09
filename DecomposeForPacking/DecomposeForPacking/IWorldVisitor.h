#pragma once

#include <memory>
#include "Point.h"

class World;

class IWorldVisitor
{
public:
	virtual void visit(World& world, Point point) = 0;
};

typedef std::shared_ptr<IWorldVisitor> IWorldVisitorPtr;