#pragma once

#include <unordered_map>
#include <tuple>
#include "Part.h"
#include "DLXSolver.h"

using std::tuple;
using std::unordered_map;

typedef unordered_map<Part, int> PartsCount;
typedef vector<tuple<PartOrientation, int>> PartLocationList;

class AlgoXResult
{
public:
	AlgoXResult(PartListPtr partList, vector<DLX_SOLUTION>);
	virtual ~AlgoXResult();

private:
	PartListPtr _partList;
	PartLocationList _partLocationList;
};

