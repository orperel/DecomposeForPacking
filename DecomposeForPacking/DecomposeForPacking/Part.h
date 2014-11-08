#pragma once
#include "Point.h"
#include <vector>

class Part
{
public:
	Part();
	virtual ~Part();

	PointList getPointList();
};

typedef std::vector<Part> PartList;