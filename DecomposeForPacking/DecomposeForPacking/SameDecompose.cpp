#include "SameDecompose.h"
#include "Part.h"
#include "PartBuilder.h"
#include "Decompose.h"
#include "DecomposeResult.h"
#include "Packing.h"
#include "GLDisplayHelper.h"

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
	Decompose decomposer1(m_world1, partList);
	shared_ptr<DecomposeResult> decomposeResult1 = decomposer1.decompose();

	//Packing packer(m_world1, decomposeResult);
	//shared_ptr<PackResult> packResult = packer.pack();

	Decompose decomposer2(m_world2, partList);
	shared_ptr<DecomposeResult> decomposeResult2 = decomposer2.decompose();

	std::shared_ptr<vector<PartLocationListPtr>> partLocationListVector1 = decomposeResult1->getListOfPartLocationLists();
	std::shared_ptr<vector<PartLocationListPtr>> partLocationListVector2 = decomposeResult2->getListOfPartLocationLists();

	std::shared_ptr<vector<PartLocationListPtr>> ret1(new vector<PartLocationListPtr>());
	std::shared_ptr<vector<PartLocationListPtr>> ret2(new vector<PartLocationListPtr>());

	for (unsigned i = 0; i < partLocationListVector1->size(); i++) {
		for (unsigned j = 0; j < partLocationListVector1->size(); j++) {
			if ((*partLocationListVector1)[i] == (*partLocationListVector1)[j]) {
				ret1->push_back((*partLocationListVector1)[i]);
				ret2->push_back((*partLocationListVector2)[j]);
			}
		}
	}

	std::tuple<std::shared_ptr<std::vector<PartLocationListPtr>>, std::shared_ptr<std::vector<PartLocationListPtr>> > bla = std::make_tuple(ret1, ret2);

	GLDisplayHelper displayHelper;
	displayHelper.displayResults(m_world1, bla);

	return ret1;
}
