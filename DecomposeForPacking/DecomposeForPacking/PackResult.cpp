#pragma once

#include "PackResult.h"

PackResult::PackResult(std::shared_ptr<vector<PartLocationListPtr>> packPerDecompose)
{
	_packPerDecompose = packPerDecompose;

	_hasSolution = false;
	for each (const PartLocationListPtr& partLocationList in *packPerDecompose) {
		if (partLocationList->size() > 0) {
			_hasSolution = true;
		}
	}
}

PackResult::~PackResult()
{
}

std::shared_ptr<vector<PartLocationListPtr>> PackResult::getPackPerDecomposeList()
{
	return _packPerDecompose;
}

bool PackResult::hasSolution()
{
	return _hasSolution;
}