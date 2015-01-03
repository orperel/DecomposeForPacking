#include "SameDecompose.h"
#include "Part.h"
#include "PartBuilder.h"
#include "Decompose.h"
#include "DecomposeResult.h"
#include "Packing.h"

SameDecompose::SameDecompose(WorldPtr world1, WorldPtr world2) : m_world1(world1), m_world2(world2)
{
}


SameDecompose::~SameDecompose()
{
}

std::shared_ptr<vector<PartLocationListPtr>> SameDecompose::run()
{
	std::shared_ptr<vector<PartLocationListPtr>> ret(new vector<PartLocationListPtr>());

	PartListPtr partList = PartBuilder::buildStandartPartPack(1);
	Decompose decomposer(m_world1, partList);
	shared_ptr<DecomposeResult> decomposeResult = decomposer.decompose();

	Packing packer(m_world2, decomposeResult);
	shared_ptr<PackResult> packResult = packer.pack();

	std::shared_ptr<vector<PartLocationListPtr>> partLocationListVector = packResult->getPackPerDecomposeList();

	for (int i = 0; i < partLocationListVector->size(); i++) {
	
	}

	return ret;
}
