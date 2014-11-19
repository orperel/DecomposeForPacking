#include "PackResult.h"

#pragma region - PackResultIterator

PackResult::PackResultIterator::PackResultIterator(PackResult& packedResults) : _packedResults(packedResults)
{
	_innerIterator = _packedResults._packedPartsToDecompose.begin();
	_iteratorEnd = _packedResults._packedPartsToDecompose.end();
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

	PackToDecomposeTuple nextPackSolution = *_innerIterator;
	auto packedPartsLocations = std::get<0>(nextPackSolution);
	int decomposeSolutionIndex = std::get<1>(nextPackSolution);
	auto decomposedPartsLocations = _packedResults._decomposition->getListOfPartLocationLists()->at(decomposeSolutionIndex);
	auto partsInSolution = _packedResults._decomposition->getPartsCountList()->at(decomposeSolutionIndex);

	// Advance to the next item
	_innerIterator++;

	auto tuple = std::make_tuple(packedPartsLocations, decomposedPartsLocations, partsInSolution);

	return NULL;
	//return std::make_unique<DAPSolution>(
		 //  );
}

#pragma region - PackResult

/**
* Grades are given according to the following criteria (priority descending):
* 1) Solution is shaped as a box +5 points.
* 2) Solution with least parts gets +5 points.
* 4) Solution with smaller total size +3 points.
* 3) Solution contains holes -3 points.
*/
bool gradeByShapeAndNumOfParts(const PackToDecomposeTuple& solutionA, const PackToDecomposeTuple & solutionB)
{
	int aGrade = 0;
	int bGrade = 0;



	return aGrade > bGrade;
}

PackResult::PackResult(shared_ptr<DecomposeResult> decomposeResult) : _decomposition(decomposeResult)
{
}


PackResult::~PackResult()
{
}

void PackResult::addPackingSolution(int decomposeSolutionIndex, PartLocationListPtr packedParts)
{
	_packedPartsToDecompose.push_back(std::make_tuple(packedParts, decomposeSolutionIndex));
}

void PackResult::sortByCriteria(std::function < bool(const PackToDecomposeTuple&,
													 const PackToDecomposeTuple&) > criteria)
{
	std::sort(_packedPartsToDecompose.begin(), _packedPartsToDecompose.end(), criteria);
}

unique_ptr<PackResult::PackResultIterator> PackResult::iterator()
{
	//sortByCriteria(*gradeByShapeAndNumOfParts(const PackToDecomposeTuple&, const PackToDecomposeTuple&)); // TODO
	return std::make_unique<PackResult::PackResultIterator>(*this);
}