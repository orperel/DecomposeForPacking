#pragma once

#include "World.h"
#include "Decompose.h"
#include "DecomposeResult.h"
#include "PackResult.h"

static const int CANDIDATES_FOR_PACKING = 3;

/**
 *	Packs an input of given parts within a given box.
 *	The algorithm returns the parts positioned and oriented within the box.
 *	Multiple packing solutions may be returned.
 */
class Packing
{
public:
	Packing(WorldPtr box, std::shared_ptr<DecomposeResult> decomposeResult);
	virtual ~Packing();

	std::shared_ptr<PackResult> pack();

private:
	WorldPtr _box;
	std::shared_ptr<vector<PartsCountPtr>> _partsCountList;
	std::shared_ptr<vector<SizeIndex>> _partsCountBySize;
	// Map from location sets to their parts
	SetToPartMapPtr _locationSetToPart;
	// Map from location sets to their part orientations
	SetToOrientationMapPtr _locationSetToOrient;
};

