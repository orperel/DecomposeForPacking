#include "Decompose.h"
#include "DecomposeResult.h"
#include "DecomposePartFitVisitor.h"

/** Constructs a new decompose object from a world and part list. */
Decompose::Decompose(WorldPtr world, PartListPtr partList) : m_world(world), m_partList(partList),
	m_locationSetToPart(new SetToPartMap()), m_locationSetToOrient(new SetToOrientationMap())
{
}

Decompose::~Decompose()
{
}

/** Implements the class purposes and returns the decomposition result. */
std::shared_ptr<DecomposeResult> Decompose::decompose(bool isPartial /*= true*/)
{
	shared_ptr<DLXSolver> dlxSolver(new DLXSolver(m_world->getNumberOfPoints())); // Creates DLXSolver for the decomposition

	// For each part in the part list creates a new visitor of the world. Runs world.accept() on the visitor
	for each (const PartPtr& part in *m_partList) {
		IWorldVisitorPtr visitor(new DecomposePartFitVisitor(part, dlxSolver, m_locationSetToPart, m_locationSetToOrient));
		m_world->accept(visitor);
	}

	auto solutions = dlxSolver->solve(isPartial); // Runs solver
	
	// Creates the result vectors of the decomposition:
	// 1) Parts count list of all solutions in the decomposition
	// 2) List of part location lists of all solutions in the decomposition
	std::shared_ptr<vector<PartsCountPtr>> partsCountList = std::make_shared<vector<PartsCountPtr>>();
	std::shared_ptr<vector<PartLocationListPtr>> listOfPartLocationLists = std::make_shared<vector<PartLocationListPtr>>();

	// Goes over each solution to compute its parts count and part location list
	for each (const DLX_SOLUTION& solution in solutions) {
		PartsCountPtr partsCount = std::make_shared<PartsCount>();
		PartLocationListPtr partLocationList = std::make_shared<PartLocationList>();

		// Computes parts count
		// Goes over each location set in the current solution and counts repeated parts
		for each (const DLX_VALUES_SET& locationSet in solution) {
			PartPtr currPart = m_locationSetToPart->at(locationSet);
			if (partsCount->find(currPart) == partsCount->end()) {	// First occurrence. currPart is not in the map yet
				partsCount->insert(std::make_pair<PartPtr, int>(std::move(currPart), 1));
			}
			else {	// currPart is already in the map. Increases its counter
				(*partsCount)[currPart]++;
			}

			// Computes part location list
			// Pushes-back tuple of part orientation and its origin point to the vector
			partLocationList->push_back(m_locationSetToOrient->at(locationSet));
		}

		// Check duplicate decompose solutions here, we index the decomposition results and check
		// them by hash-code for fast comparison.
		// (the hash-code depends on the part type and its number of occurences)
		DecomposeSolutionKey solutionKey(partsCount);
		unordered_set<DecomposeSolutionKey>::const_iterator indexedResult = m_duplicatesFilter.find(solutionKey);
		if (indexedResult == m_duplicatesFilter.end())
		{
			// First time the solution is encountered, add it to the results
			partsCountList->push_back(partsCount);
			listOfPartLocationLists->push_back(partLocationList);

			m_duplicatesFilter.insert(solutionKey);
		}
	}

	DecomposeResult decomposeResult = DecomposeResult(partsCountList, listOfPartLocationLists);
	return std::make_shared<DecomposeResult>(decomposeResult);
}
