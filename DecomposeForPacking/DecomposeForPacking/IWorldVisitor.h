#pragma once

#include <memory>
#include "Point.h"

class World;
typedef std::shared_ptr<World> WorldPtr;

class IWorldVisitor
{
public:
	virtual void visit(WorldPtr worldPtr, PointPtr point) = 0;
};

typedef std::shared_ptr<IWorldVisitor> IWorldVisitorPtr;