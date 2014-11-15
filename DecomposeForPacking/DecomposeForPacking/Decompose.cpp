#include "Decompose.h"
#include "IsPartFitVisitor.h"


Decompose::Decompose(WorldPtr world, PartListPtr partList) : m_world(world), m_partList(partList), m_locationSetToPart(new SetsToPartMap())
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
		IWorldVisitorPtr visitor(new IsPartFitVisitor(part, dlxSolver, m_locationSetToPart));
		m_world->accept(visitor);
	}

	auto solutions = dlxSolver->solve();	// Runs solver
}