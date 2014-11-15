#include "Decompose.h"
#include "IsPartFitVisitor.h"

Decompose::Decompose(WorldPtr world, PartListPtr partList) : m_world(world), m_partList(partList), m_locationSetToOrient(new SetsToOrientationMap())
{
}

Decompose::~Decompose()
{
}

vector<DLX_SOLUTION> Decompose::decompose()
{
	shared_ptr<DLXSolver> dlxSolver(new DLXSolver(m_world->getNumberOfPoints())); // Creates DLXSolver for the decomposition

	// For each part in the part list creates a new visitor of the world. Runs world.accept() on the visitor
	for each (const PartPtr& part in *m_partList) {
		for each (const PartOrientationPtr& partOrient in *part->getPartOrientations()) {
			IWorldVisitorPtr visitor(new IsPartFitVisitor(partOrient, dlxSolver, m_locationSetToOrient));
			m_world->accept(visitor);
		}
	}

	auto solutions = dlxSolver->solve();	// Runs solver

	return solutions;
}