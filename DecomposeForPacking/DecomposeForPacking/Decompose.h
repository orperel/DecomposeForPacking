#pragma once
#include "World.h"
#include "Part.h"

class Decompose
{
public:
	// Runs on the part list, for each part 
	Decompose(World world, PartList partList);
	virtual ~Decompose();

	void decompose();

private:
	World world;
	PartList partList;
};
