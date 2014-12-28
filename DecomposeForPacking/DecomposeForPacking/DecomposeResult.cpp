#include <algorithm>
#include "DecomposeResult.h"

/** Constructs a new decomposition result. It is composed of:
 *  1) Parts count list of all solutions in the decomposition process
 *  2) List of part location lists of all solutions in the decomposition process
 */
DecomposeResult::DecomposeResult(std::shared_ptr<vector<PartsCountPtr>> partsCountList, std::shared_ptr<vector<PartLocationListPtr>> listOfPartLocationLists)
{
	_partsCountList = partsCountList;
	_listOfPartLocationLists = listOfPartLocationLists;
}

/** Dtor to release resources allocated by the DecomposeResult. */
DecomposeResult::~DecomposeResult()
{
}

/** Returns the parts count list of all solutions. */
std::shared_ptr<vector<PartsCountPtr>> DecomposeResult::getPartsCountList()
{
	return _partsCountList;
}

/** Returns the list of part location lists of all solutions. */
std::shared_ptr<vector<PartLocationListPtr>> DecomposeResult::getListOfPartLocationLists()
{
	return _listOfPartLocationLists;
}

/** A lambda expression implements the size criteria for sorting the parts count lists. */
bool wayToSort(const SizeIndex& x, const SizeIndex& y) { return (std::get<0>(x) < std::get<0>(y)); }

/** Returns indices of the parts count lists, ordered by size. */
std::shared_ptr<vector<SizeIndex>> DecomposeResult::getPartsCountBySize()
{
	std::shared_ptr<vector<SizeIndex>> partsCountBySize = std::make_shared<vector<SizeIndex>>();

	// Computes the sizes of all solutions 
	for (int index = 0; index < _partsCountList->size(); index++) {
		// Computes the size of the current solution
		int currSolutionSize = 0;		
		for (auto partCount : *_partsCountList->at(index)) {
			currSolutionSize += partCount.second;
		}
		partsCountBySize->push_back(SizeIndex(currSolutionSize, index));
	}

	std::sort(partsCountBySize->begin(), partsCountBySize->end(), wayToSort);	// Sorts by size

	return partsCountBySize;
}