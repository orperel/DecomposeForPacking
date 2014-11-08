#pragma once
#include "Point.h"
#include <vector>
#include <memory>

class Part
{
public:
	Part();
	virtual ~Part();

	PointList getPointList();
};

typedef std::shared_ptr<Part> PartPtr;
typedef std::vector<PartPtr> PartList;
