#include "DecomposeResult.h"

DecomposeResult::DecomposeResult(std::shared_ptr<vector<PartsCountPtr>> partsCountList, std::shared_ptr<vector<PartLocationListPtr>> listOfPartLocationLists)
{
	_partsCountList = partsCountList;
	_listOfPartLocationLists = listOfPartLocationLists;
}

DecomposeResult::~DecomposeResult()
{
}

std::shared_ptr<vector<PartLocationListPtr>> DecomposeResult::getListOfPartLocationLists()
{
	return _listOfPartLocationLists;
}
