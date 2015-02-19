#pragma once

#include <limits>
#include "World.h"
#include "Decompose.h"
#include "DecomposeResult.h"
#include "PackResult.h"

/** Manages the packing process, i.e. calling for the visitor and the DLX solver, and arranging the results
	in a PackResult object. */
class Packing
{
public:
	/** Constructs a new packing object from a box and decomposition result. */
	Packing(WorldPtr box, std::shared_ptr<DecomposeResult> decomposeResult);
	
	/** Dtor. */
	virtual ~Packing();

	/** Implements the class purposes and returns the packing result.
	 *	The results are ordered according to the decompose result, one solution for each decomposition.
	 *  This solution is with the minimal bounding box, meaning the best solution.
	 */
	std::shared_ptr<PackResult> pack();

	/** Returns the SolutionsNumOfParts vector got from decomposition. This is used for DecomposeAndPack. */
	std::shared_ptr<vector<int>> publicSolutionsNumOfParts();

	/** Returns the bounding box vector of the pack result. */
	std::shared_ptr<vector<int>> getResultsBoundingBox();

private:
	WorldPtr _box;	// The box to pack into

	// From decomposition:
	std::shared_ptr<vector<PartsCountPtr>> _partsCountList;
	std::shared_ptr<vector<int>> _solutionsNumOfParts;

	SetToPartMapPtr _locationSetToPart;	// Map from location sets to parts
	SetToOrientationMapPtr _locationSetToOrient;	// Map from location sets to part orientations

	std::shared_ptr<vector<int>> _resultsBoundingBox;	// Bounding box sizes of the pack result

	/** Returns a vector of the bounding box sizes of all solutions, for one decomposition. */
	std::shared_ptr<vector<int>> getBoundingBoxes(std::shared_ptr<vector<PartLocationListPtr>> listOfPartLocationLists);
};

