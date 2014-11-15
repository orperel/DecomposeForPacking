#pragma once

#include <unordered_map>
#include <tuple>
#include "Part.h"
#include "DLXSolver.h"

using std::tuple;
using std::unordered_map;

typedef unordered_map<Part, tuple<int, PartOrientation>> PartToPositionMap;
typedef std::shared_ptr<PartToPositionMap> PartToPositionMapPtr;

class AlgoXResult
{
public:
	AlgoXResult(PartListPtr partList, vector<DLX_SOLUTION>);
	virtual ~AlgoXResult();

private:
	PartListPtr _partList;
	PartToPositionMapPtr _PartToPositionMap;
};

