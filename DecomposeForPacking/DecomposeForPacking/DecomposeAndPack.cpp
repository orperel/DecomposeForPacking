#include "DecomposeAndPack.h"

#include "CImg.h"
#include "DisplayHelper.h"
#include "PartBuilder.h"
#include "Decompose.h"
#include "PackResult.h"
#include "WorldBuilder.h"
#include "Packing.h"

using namespace std;

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

shared_ptr<DecomposeResult> DecomposeAndPack::decompose()
{
	PartListPtr partList = PartBuilder::buildStandartPartPack(1);

	Decompose decomposer(m_world, partList);

	cout << "Starting decomposing..." << endl;

	shared_ptr<DecomposeResult> decomposeResult = decomposer.decompose();

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
