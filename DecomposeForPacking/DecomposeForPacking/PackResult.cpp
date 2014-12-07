#include <algorithm>
#include "PackResult.h"

using std::shared_ptr;

#pragma region - PackResultIterator

PackResult::PackResultIterator::PackResultIterator(PackResult& packResult) : _packResult(packResult)
{
	_innerIterator = _packResult._packedPartsToDecompose.begin();
	_iteratorEnd = _packResult._packedPartsToDecompose.end();
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
	auto decomposedPartsLocations = _packResult._decomposition->getListOfPartLocationLists()->at(decomposeSolutionIndex);
	auto partsInSolution = _packResult._decomposition->getPartsCountList()->at(decomposeSolutionIndex);

	// Advance to the next item
	_innerIterator++;

	auto tuple = std::make_tuple(packedPartsLocations, decomposedPartsLocations, partsInSolution);

	return NULL;
	//return std::make_unique<DAPSolution>( );
}


#pragma region - PackResult

PackResult::PackResult(std::shared_ptr<vector<PartLocationListPtr>> packPerDecompose)
{
	_packPerDecompose = packPerDecompose;

	_hasSolution = false;

	for each (PartLocationListPtr partLocationList in *packPerDecompose) {
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



/**
* Grades are given according to the following criteria (priority descending):
* 1) Solution is shaped as a box +5 points.
* 2) Solution with least parts gets +5 points.
* 4) Solution with smaller total size +3 points.
* 3) Solution contains holes -3 points.
*/
bool PackResult::gradeByShapeAndNumOfParts(const PackToDecomposeTuple& solutionA, const PackToDecomposeTuple & solutionB)
{
	int aGrade = 0;
	int bGrade = 0;

	return aGrade > bGrade;
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

bool PackResult::hasSolution()
{
	return _hasSolution;
}