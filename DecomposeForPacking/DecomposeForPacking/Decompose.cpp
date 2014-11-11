#include "Decompose.h"
#include "IsPartFitVisitor.h"


Decompose::Decompose(WorldPtr world, PartListPtr partList) : m_world(world), m_partList(partList)
{
}


Decompose::~Decompose()
{
}

void Decompose::decompose()
{
	shared_ptr<DLXSolver> dlx(new DLXSolver(m_world->getNumberOfPoints()));

	for each (const PartPtr& part in *m_partList) {
		IWorldVisitorPtr visitor(new IsPartFitVisitor(part, dlx, m_locationSetToPart));
		m_world->accept(visitor);
	}
}
