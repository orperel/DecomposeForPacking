#pragma once

#include <unordered_map>
#include <tuple>
#include "Part.h"
#include "AlgoXResult.h"

using std::tuple;
using std::unordered_map;

typedef int PartLocation;
typedef int PartOrientation;
typedef unordered_map<Part, tuple<PartLocation, PartOrientation>> PartToPositionMap;
typedef std::shared_ptr<PartToPositionMap> PartToPositionMapPtr;

class AlgoXResult
{
public:
	AlgoXResult(PartList partList, vector<DLX_SOLUTION>);
	virtual ~AlgoXResult();

private:
	PartList _partList;
	PartToPositionMapPtr _PartToPositionMap;
};

