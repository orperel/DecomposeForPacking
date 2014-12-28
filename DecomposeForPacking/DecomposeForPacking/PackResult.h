#pragma once

#include "DecomposeResult.h"

/**	Contains multiple solution results of the packing process. */
class PackResult
{
public:
	PackResult(std::shared_ptr<vector<PartLocationListPtr>> packPerDecompose);
	
	virtual ~PackResult();

	std::shared_ptr<vector<PartLocationListPtr>> getPackPerDecomposeList();

	bool hasSolution();

private:
	std::shared_ptr<vector<PartLocationListPtr>> _packPerDecompose;	
	bool _hasSolution;
};