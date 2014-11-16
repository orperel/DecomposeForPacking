#pragma once

#include <memory>
#include "Point.h"

class World;

/** An interface of a world visitor, implementing the Visitor design pattern.
 *  It represents an operation to be performed on the world structure, without modifying this structure.
 */
class IWorldVisitor
{
public:
	/** The operation itself which the visitor operates on the world.
	 *  Receives the world object and a point in the world on which the operation is applied.
	 */
	virtual void visit(World& world, Point point) = 0;
};

typedef std::shared_ptr<IWorldVisitor> IWorldVisitorPtr;