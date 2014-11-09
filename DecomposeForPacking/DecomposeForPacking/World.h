#pragma once
#include "IWorldVisitor.h"
#include <memory>

/* Represents the world to be decompose or packed into, i.e. the spatial in which we locate the parts that assemble
   the object. The world is composed of transparent and non-transparent pixels; the non-transparents are the pixels
   of the object/pack istself. */
class World
{

public:
	World();
	virtual ~World();

	/* Returns true if the point is part of the object, else false. */
	bool isPointPainted(Point point);

	void accept(IWorldVisitorPtr visitor);
};

typedef std::shared_ptr<World> WorldPtr;

//
///* Returns the world's height. */
//int getHeight();
//
///* Returns the world's width. */
//int getWidth();