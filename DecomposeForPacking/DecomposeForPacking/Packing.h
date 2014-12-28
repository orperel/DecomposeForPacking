#pragma once

#include "World.h"
#include "Decompose.h"
#include "DecomposeResult.h"
#include "PackResult.h"

static const int CANDIDATES_FOR_PACKING = 3;	// Only these decomposition results will be packed for check 

/**
 *	
 */
class Packing
{
public:
	Packing(WorldPtr box, std::shared_ptr<DecomposeResult> decomposeResult);
	virtual ~Packing();

	std::shared_ptr<PackResult> pack();

private:
	WorldPtr _box;

	// From decomposition:
	std::shared_ptr<vector<PartsCountPtr>> _partsCountList;
	std::shared_ptr<vector<SizeIndex>> _partsCountBySize;
	
	// For packing:
	SetToPartMapPtr _locationSetToPart;
	SetToOrientationMapPtr _locationSetToOrient;
};

