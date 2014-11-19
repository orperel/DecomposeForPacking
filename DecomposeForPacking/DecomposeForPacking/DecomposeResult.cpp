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
