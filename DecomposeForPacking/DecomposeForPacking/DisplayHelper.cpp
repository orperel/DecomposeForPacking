#include "DisplayHelper.h"
#include "WorldBuilder.h"
#include "DecomposeResult.h"

shared_ptr<CImgDisplay> DisplayHelper::showWorld(WorldPtr world)
{
	shared_ptr<CImg<unsigned char>> worldImg = WorldBuilder::toImage(world);

	shared_ptr<CImg<unsigned char>> bigDimen(new CImg<unsigned char>(500, 300));

	worldImg->resize(*bigDimen);

	shared_ptr<CImgDisplay> display(new CImgDisplay(*worldImg));

	return display;
}

shared_ptr<vector<shared_ptr<CImgDisplay>>> DisplayHelper::showDecomposeResult(WorldPtr world, std::vector<PartLocationList>& decomposeResult, int numOfResultsToShow)
{
	shared_ptr<vector<shared_ptr<CImgDisplay>>> displayVector(new vector<shared_ptr<CImgDisplay>>());

	int count = 0;
	for each (const PartLocationList& partLocationList in decomposeResult) { // For every result
		if (numOfResultsToShow == count) {	// Show only NUM_OF_RESULTS_TO_SHOW results
			break;
		}

		int color = 0;

		PointListPtr pointList(new PointList());

		for each (const tuple<PartOrientationPtr, int>& partOrientTuple in partLocationList) { // For every part
			PartOrientationPtr partOrient = get<0>(partOrientTuple);
			int index = get<1>(partOrientTuple);
			Point basePoint = world->getPointFromIndex(index);
			color += 255 / partLocationList.size();

			for each (const Point& point in *partOrient->getPointList()) { // For each point of the part
				pointList->push_back(Point(point.getX() + basePoint.getX(), point.getY() + basePoint.getY(), color));
			}
		}

		WorldPtr newWorld(new World(pointList, world->getWidth(), world->getHeight()));

		displayVector->push_back(DisplayHelper::showWorld(newWorld));

		count++;
	}

	return displayVector;
}
