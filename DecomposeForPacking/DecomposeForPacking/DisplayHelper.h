#pragma once

#include "CImg.h"
#include "World.h"
#include "AlgoXResult.h"

using namespace cimg_library;
using namespace std;

class DisplayHelper
{
public:
	static shared_ptr<CImgDisplay> showWorld(WorldPtr world);

	static shared_ptr<vector<shared_ptr<CImgDisplay>>> showDecomposeResult(WorldPtr world, std::vector<PartLocationList>& decomposeResult, int numOfResultsToShow);
};

