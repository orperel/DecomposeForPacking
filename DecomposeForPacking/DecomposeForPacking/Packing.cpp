#include "Packing.h"
#include <vector>

Packing::Packing(WorldPtr box, vector<PartsCount> partsToPack) : _box(box), _partsToPack(partsToPack)
{

}

Packing::~Packing()
{

}

AlgoXResult Packing::pack()
{
	vector<DLX_SOLUTION> results;

	// Creates DLXSolver for the decomposition
	shared_ptr<DLXSolver> dlxSolver(new DLXSolver(_box->getNumberOfPoints()));

	// For each part in the part list creates a new visitor of the world. Runs world.accept() on the visitor
	for each (const PartPtr& part in *m_partList) {
		for each (const PartOrientationPtr& partOrient in *part->getPartOrientations()) {
			IWorldVisitorPtr visitor(new IsPartFitVisitor(partOrient, dlxSolver, m_locationSetToOrient));
			m_world->accept(visitor);
		}
	}

	auto solutions = dlxSolver->solve();	// Runs solver

	return solutions;

	return AlgoXResult(NULL, results);
}