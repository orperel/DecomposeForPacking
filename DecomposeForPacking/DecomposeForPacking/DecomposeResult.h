#pragma once

#include <map>
#include <tuple>
#include "Part.h"
#include "DLXSolver.h"

using std::tuple;
using std::map;

typedef map<PartPtr, int> PartsCount;
typedef std::shared_ptr<PartsCount> PartsCountPtr;
typedef vector<tuple<PartOrientationPtr, int>> PartLocationList;
typedef std::shared_ptr<PartLocationList> PartLocationListPtr;

class DecomposeResult
{
public:
	DecomposeResult(std::shared_ptr<vector<PartsCountPtr>> partsCountList, std::shared_ptr<vector<PartLocationListPtr>> listOfPartLocationLists);
	virtual ~DecomposeResult();

	std::shared_ptr<vector<PartLocationListPtr>> getListOfPartLocationLists();
private:
	std::shared_ptr<vector<PartsCountPtr>> _partsCountList;
	std::shared_ptr<vector<PartLocationListPtr>> _listOfPartLocationLists;
};

