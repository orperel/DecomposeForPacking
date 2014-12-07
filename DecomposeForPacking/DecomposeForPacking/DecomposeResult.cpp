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

bool wayToSort(const SizeIndex& x, const SizeIndex& y) { return (std::get<0>(x) < std::get<0>(y)); }

std::shared_ptr<vector<SizeIndex>> DecomposeResult::getPartsCountBySize()
{
	std::shared_ptr<vector<SizeIndex>> partsCountBySize = std::make_shared<vector<SizeIndex>>();

	for (int index = 0; index < _partsCountList->size(); index++) {
		int currSolutionSize = 0;		
		for (auto partCount : *_partsCountList->at(index)) {
			currSolutionSize += partCount.second;
		}
		partsCountBySize->push_back(SizeIndex(currSolutionSize, index));
	}

	std::sort(partsCountBySize->begin(), partsCountBySize->end(), wayToSort);

	return partsCountBySize;
}