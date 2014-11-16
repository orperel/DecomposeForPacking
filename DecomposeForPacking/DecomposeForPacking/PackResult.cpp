#include "PackResult.h"

#pragma region - PackResultIterator

PackResult::PackResultIterator::PackResultIterator(vector<tuple<PartLocationList, DECOMPOSE_SOULTION_INDEX>>& packedResults)
{
	_innerIterator = packedResults.begin();
	_iteratorEnd = packedResults.end();
}

PackResult::PackResultIterator::~PackResultIterator()
{
}

const bool PackResult::PackResultIterator::hasNext()
{
	return _innerIterator != _iteratorEnd;
}

unique_ptr<DAPSolution> PackResult::PackResultIterator::nextSolution()
{
	if (!hasNext())
	{
		return NULL;
	}

	tuple<PartLocationList, DECOMPOSE_SOULTION_INDEX> nextPackSolution = *_innerIterator;
	auto packedPartsLocations = std::get<0>(nextPackSolution);
	int decomposeSolutionIndex = std::get<1>(nextPackSolution);
	auto decomposedPartsLocations = _decomposition->getPackedParts(decomposeSolutionIndex);
	auto partsInSolution = _decomposition->getPartsLocation(decomposeSolutionIndex);

	// Advance to the next item
	_innerIterator++;

	return std::make_unique<DAPSolution>(
		   std::make_tuple(packedPartsLocations, decomposedPartsLocations, partsInSolution));
}

#pragma region - PackResult

/**
* Grades are given according to the following criteria (priority descending):
* 1) Solution is shaped as a box +5 points.
* 2) Solution with least parts gets +5 points.
* 4) Solution with smaller total size +3 points.
* 3) Solution contains holes -3 points.
*/
bool gradeByShapeAndNumOfParts(const tuple<PartLocationList, DECOMPOSE_SOULTION_INDEX>& solutionA,
							   const tuple<PartLocationList, DECOMPOSE_SOULTION_INDEX>& solutionB)
{
	int aGrade = 0;
	int bGrade = 0;



	return aGrade > bGrade;
}

PackResult::PackResult(shared_ptr<AlgoXResult> decomposeResult) : _decomposition(decomposeResult)
{
}


PackResult::~PackResult()
{
}

void PackResult::addPackingSolution(int decomposeSolutionIndex, PartLocationList packedParts)
{
	_packedPartsToDecompose.push_back(std::make_tuple(packedParts, decomposeSolutionIndex));
}

void PackResult::sortByCriteria(std::function < bool(const tuple<PartLocationList, DECOMPOSE_SOULTION_INDEX>&,
										 const tuple<PartLocationList, DECOMPOSE_SOULTION_INDEX>&) > criteria)
{
	std::sort(_packedPartsToDecompose.begin(), _packedPartsToDecompose.end(), criteria);
}

unique_ptr<PackResult::PackResultIterator> PackResult::iterator()
{
	sortByCriteria(gradeByShapeAndNumOfParts);
	return std::make_unique<PackResult::PackResultIterator>(_packedPartsToDecompose);
}