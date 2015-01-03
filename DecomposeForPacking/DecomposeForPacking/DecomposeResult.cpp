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

DecomposeResult::DecomposeResult()
{
	_partsCountList = std::shared_ptr<vector<PartsCountPtr>>(new vector<PartsCountPtr>);
	_listOfPartLocationLists = std::shared_ptr<vector<PartLocationListPtr>>(new vector<PartLocationListPtr>);
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

void DecomposeResult::extend(PartsCountPtr partsCount, PartLocationListPtr partLocationList)
{
	// TODO: check if needed
	//if (NULL == partsCount || NULL == partLocationList) {
	//	return;
	//}

	for (int i = 0; i < _partsCountList->size(); i++) {
		for each (const std::pair<PartPtr, int>& elem in *partsCount) {
			(*(*_partsCountList)[i])[elem.first] = elem.second;
		}
	}

	for (int i = 0; i < _listOfPartLocationLists->size(); i++) {
		for (int j = 0; j < partLocationList->size(); j++) {
			PartLocationListPtr pll = _listOfPartLocationLists->at(i);
			pll->push_back((*partLocationList)[j]);
		}
	}
}

void DecomposeResult::add(shared_ptr<DecomposeResult> decomposeResult)
{
	_partsCountList->insert(_partsCountList->end(), decomposeResult->getPartsCountList()->begin(), decomposeResult->getPartsCountList()->end());
	_listOfPartLocationLists->insert(_listOfPartLocationLists->end(), decomposeResult->getListOfPartLocationLists()->begin(), decomposeResult->getListOfPartLocationLists()->end());
}
