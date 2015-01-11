#include "DecomposeAndPack.h"

#include "CImg.h"
#include "DisplayHelper.h"
#include "PartBuilder.h"
#include "Decompose.h"
#include "PackResult.h"
#include "WorldBuilder.h"
#include "Packing.h"

#include <algorithm>

using namespace std;

const int DECOMPOSE_NUMBER_OF_ITERATIONS = 3;

DecomposeAndPack::DecomposeAndPack(WorldPtr world) : m_world(world)
{
}


DecomposeAndPack::~DecomposeAndPack()
{
}

DecomposeAndPackResult DecomposeAndPack::run()
{
	shared_ptr<DecomposeResult> decomposeResult = decompose();

	shared_ptr<PackResult> packResult = pack(decomposeResult);

	return std::make_tuple(decomposeResult->getListOfPartLocationLists(), packResult->getPackPerDecomposeList());
}

shared_ptr<DecomposeResult> DecomposeAndPack::extendDecompose(WorldPtr world, PartListPtr partList,
	PartsCountPtr partsCount, PartLocationListPtr partLocationList, shared_ptr<DecomposeResult> totalDecomposeResults) {

	PointListPtr newPointList(new PointList(*world->getPointList()));
	std::vector<int> pointIndexesToDelete;

	for each (const tuple<PartOrientationPtr, Point>& partOrientTuple in *partLocationList) { // For every part
		PartOrientationPtr partOrient = get<0>(partOrientTuple);
		Point basePoint = get<1>(partOrientTuple);

		for each (const Point& pointOffset in *partOrient->getPointList()) { // For each point of the part
			Point point(pointOffset.getX() + basePoint.getX(), pointOffset.getY() + basePoint.getY(), 0, BLACK);
			pointIndexesToDelete.push_back(world->getIndexFromPoint(point));
		}
	}

	std::sort(pointIndexesToDelete.begin(), pointIndexesToDelete.end(), sortIntDesc);

	for each (const int index in pointIndexesToDelete) {
		newPointList->erase(newPointList->begin() + index);
	}

	WorldPtr newWorld(new World(newPointList, world->getWidth(), world->getHeight(), world->getDepth(), world->getPixelResolution()));

	// The new world is empty, an exact decomposition was found.
	if ((newWorld->getHeight() == 0) || (newWorld->getWidth() == 0))
	{
		return NULL;
	}

	// Else continue decomposing the remaining blocks in the world
	Decompose decomposer(newWorld, partList);
	shared_ptr<DecomposeResult> decomposeResult = decomposer.decompose();
	decomposeResult->extend(partsCount, partLocationList);

	totalDecomposeResults->add(decomposeResult);

	return decomposeResult;
}

shared_ptr<DecomposeResult> DecomposeAndPack::decompose()
{
	cout << "Starting decomposing..." << endl;

	int numberOfIteration = 1;
	float partSizePercentOfWorld = 20;

	int minEdge = round(min(m_world->getWidth(), m_world->getHeight()) * partSizePercentOfWorld / 100);

	shared_ptr<DecomposeResult> decomposeResult;

	for (int i = 0; i < numberOfIteration; i++) {
		int partSize = minEdge - round(minEdge*i / numberOfIteration);

		if (0 == partSize) {
			partSize = 1;
		}

		shared_ptr<DecomposeResult> newDecomposeResult(new DecomposeResult());

		PartListPtr partList = PartBuilder::buildStandartPartPack(partSize);

		if (NULL == decomposeResult) {
			Decompose decomposer(m_world, partList);
			decomposeResult = decomposer.decompose();
		}
		else {
			for (int j = 0; j < decomposeResult->getListOfPartLocationLists()->size(); j++) {
				shared_ptr<DecomposeResult> nextDecomposition = extendDecompose(m_world, partList,
					(*decomposeResult->getPartsCountList())[j],
					(*decomposeResult->getListOfPartLocationLists())[j],
					newDecomposeResult);

				// Exact decomposition was found, quit
				if (nextDecomposition == NULL)
					break;
			}

			decomposeResult = newDecomposeResult;
		}

		if (1 == partSize) {
			break;
		}
	}

	cout << "Finished decomposing..." << endl;

	return decomposeResult;
}


shared_ptr<PackResult> DecomposeAndPack::pack(shared_ptr<DecomposeResult> decomposeResult)
{
	int width, height;

	shared_ptr<PackResult> packResult;

	cout << "Starting packing..." << endl;

	do {
		width = ceil(sqrt(m_world->getNumberOfPoints()));
		height = width;
		WorldPtr box = WorldBuilder::buildBox(width, height);
		Packing packer(box, decomposeResult);
		packResult = packer.pack();

		if (packResult->hasSolution()) {
			cout << "Found Solution!!!" << endl;
			break;
		}

		width = ceil(width + 0.1*width);
		height = width;
	} while (m_world->getWidth() >= width && m_world->getHeight() >= height);

	cout << "Finished packing..." << endl;

	return packResult;
}
