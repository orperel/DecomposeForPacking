#pragma once

#include <limits>
#include "World.h"
#include "Decompose.h"
#include "DecomposeResult.h"
#include "PackResult.h"

static const int CANDIDATES_FOR_PACKING = std::numeric_limits<int>::max();	// Number of decomposition results to be packed

/** Manages the packing process, that is calling for the visitor and the DLX solver, and arranging the results
	in a PackResult object. */
class Packing
{
public:
	/** Constructs a new packing object from a box and decomposition result. */
	Packing(WorldPtr box, std::shared_ptr<DecomposeResult> decomposeResult);
	
	virtual ~Packing();

	/** Implements the class purposes and returns the packing result.
	 *	The results are ordered according to the decompose result, one solution for each decomposition.
	 *  This solution is with the minimal bounding box, i.e. the best solution.
	 */
	std::shared_ptr<PackResult> pack();

private:
	WorldPtr _box;	// The box to pack into

	// From decomposition:
	std::shared_ptr<vector<PartsCountPtr>> _partsCountList;	
	std::shared_ptr<vector<SizeIndex>> _partsCountBySize;
	
	SetToPartMapPtr _locationSetToPart;	// Map from location sets to parts
	SetToOrientationMapPtr _locationSetToOrient;	// Map from location sets to part orientations

	/** Returns a vector of the bounding box sizes of all solutions, for one decomposition. */
	std::shared_ptr<vector<int>> getBoundingBoxes(std::shared_ptr<vector<PartLocationListPtr>> listOfPartLocationLists);
};

