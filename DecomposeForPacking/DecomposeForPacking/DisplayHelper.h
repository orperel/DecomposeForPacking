#pragma once

#include "CImg.h"
#include "World.h"
#include "DecomposeResult.h"

using namespace cimg_library;
using namespace std;

class DisplayHelper
{
public:
	static shared_ptr<CImgDisplay> showWorld(WorldPtr world);

	static shared_ptr<vector<shared_ptr<CImgDisplay>>> showResult(WorldPtr world, std::shared_ptr<vector<PartLocationListPtr>> resultVector, int numOfResultsToShow);
};

