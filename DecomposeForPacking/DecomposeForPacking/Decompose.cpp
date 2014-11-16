#include "Decompose.h"
#include "DecomposeResult.h"
#include "IsPartFitVisitor.h"

Decompose::Decompose(WorldPtr world, PartListPtr partList) : m_world(world), m_partList(partList), m_locationSetToPart(new SetToPartMap()), m_locationSetToOrient(new SetToOrientationMap())
{
}

Decompose::~Decompose()
{
}

DecomposeResult Decompose::decompose()
{
	shared_ptr<DLXSolver> dlxSolver(new DLXSolver(m_world->getNumberOfPoints())); // Creates DLXSolver for the decomposition

	// For each part in the part list creates a new visitor of the world. Runs world.accept() on the visitor
	for each (const PartPtr& part in *m_partList) {
		IWorldVisitorPtr visitor(new IsPartFitVisitor(part, dlxSolver, m_locationSetToPart, m_locationSetToOrient));
		m_world->accept(visitor);
	}

	auto solutions = dlxSolver->solve();	// Runs solver
	
	std::shared_ptr<vector<PartsCountPtr>> partsCountList = std::make_shared<vector<PartsCountPtr>>();
	std::shared_ptr<vector<PartLocationListPtr>> listOfPartLocationLists = std::make_shared<vector<PartLocationListPtr>>();

	for each (const DLX_SOLUTION& solution in solutions) {
		PartsCountPtr partsCount = std::make_shared<PartsCount>();
		PartLocationListPtr partLocationList = std::make_shared<PartLocationList>();

		for each (const DLX_VALUES_SET& locationSet in solution) {
			PartPtr currPart = m_locationSetToPart->at(locationSet);
			if (partsCount->find(currPart) == partsCount->end()) {
				partsCount->insert(std::make_pair<PartPtr, int>(std::move(currPart), 1));
			}
			else {
				(*partsCount)[currPart]++;
			}

			PartOrientationPtr currOrient = m_locationSetToOrient->at(locationSet);
			int currAnchorIndex = m_world->getIndexFromPoint(currOrient->getAnchor());
			partLocationList->push_back(std::make_tuple(currOrient, currAnchorIndex));
		}

		partsCountList->push_back(partsCount);
		listOfPartLocationLists->push_back(partLocationList);
	}

	return DecomposeResult(partsCountList, listOfPartLocationLists);
}