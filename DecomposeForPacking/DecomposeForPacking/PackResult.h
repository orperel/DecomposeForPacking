#pragma once

#include "DecomposeResult.h"

/** Represents the packing result. */
class PackResult
{
public:
	/** Constructs a new packing result composed of:
	*   List of part location list of the best packing for each decomposition solution (the best packing is the one with
	*	the minimal bounding box).
	*/
	PackResult(std::shared_ptr<vector<PartLocationListPtr>> packPerDecompose);
	
	/** Dtor. */
	virtual ~PackResult();

	/** Returns the list of part location list per decomposition solution. */
	std::shared_ptr<vector<PartLocationListPtr>> getPackPerDecomposeList();

	/** Returns "true" if the packing process has any solution for the decompose result. */
	bool hasSolution();

private:
	// List of part location list of the best packing for each decomposition solution
	std::shared_ptr<vector<PartLocationListPtr>> _packPerDecompose;
	// Indicates if the packing has any solution for the decompose result
	bool _hasSolution;
};