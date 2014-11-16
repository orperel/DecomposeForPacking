#pragma once

#include <map>
#include <tuple>
#include "Part.h"
#include "DLXSolver.h"

using std::tuple;
using std::map;

typedef map<Part, int> PartsCount;
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

