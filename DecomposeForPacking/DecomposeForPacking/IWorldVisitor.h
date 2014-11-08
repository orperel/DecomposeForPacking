#pragma once

#include "Point.h"

class World;

class IWorldVisitor
{
public:
	void visit(World world, Point point);
};

