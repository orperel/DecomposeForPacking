#pragma once

#include "PackResult.h"

/** Constructs a new packing result composed of:
*   List of part location list of the best packing for each decomposition solution.
*/
PackResult::PackResult(std::shared_ptr<vector<PartLocationListPtr>> packPerDecompose)
{
	_packPerDecompose = packPerDecompose;

	// Checks if the given "packPerDecompose" has any non-empty solution
	_hasSolution = false;
	for each (const PartLocationListPtr& partLocationList in *packPerDecompose) {
		if (partLocationList->size() > 0) {
			_hasSolution = true;
		}
	}
}

/** Dtor. */
PackResult::~PackResult()
{
}

/** Returns the list of part location list per decomposition solution. */
std::shared_ptr<vector<PartLocationListPtr>> PackResult::getPackPerDecomposeList()
{
	return _packPerDecompose;
}

/** Returns "true" if the packing process has any solution for the decompose result. */
bool PackResult::hasSolution()
{
	return _hasSolution;
}