#pragma once

#include <tuple>
#include "DecomposeResult.h"

class PackResult;
typedef int DECOMPOSE_SOULTION_INDEX; // TODO: Remove

using std::tuple;

class PackSolutionMetaData
{
public:
	PackSolutionMetaData(tuple<PartLocationList, DECOMPOSE_SOULTION_INDEX>& packSolution);
	virtual ~PackSolutionMetaData();

	const int packageSize() const;
	const int PackSolutionMetaData::numberOfParts() const;
private:
	void generateMetaData(tuple<PartLocationList, DECOMPOSE_SOULTION_INDEX>& packSolution);
	void calculateBoundingBox(tuple<PartLocationList, DECOMPOSE_SOULTION_INDEX>& packSolution);
	void countNumberOfParts();

	int _boundTopLeft, _boundBottomRight;
	int _numberOfParts;
};