#pragma once

#include "World.h"
#include "Part.h"
#include "DLXSolver.h"
#include "DecomposeResult.h"
#include "PackResult.h"

/**
 *	Packs an input of given parts within a given box.
 *	The algorithm returns the parts positioned and oriented within the box.
 *	Multiple packing solutions may be returned.
 */
class Packing
{
public:
	Packing(WorldPtr box, vector<PartsCount> partsToPack);
	virtual ~Packing();

	PackResult pack();

private:
	WorldPtr _box;
	vector<PartsCount> _partsToPack;
};

