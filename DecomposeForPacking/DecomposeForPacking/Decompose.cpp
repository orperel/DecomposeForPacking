#include "Decompose.h"
#include "IsPartFitVisitor.h"


Decompose::Decompose(WorldPtr world, PartListPtr partList) : m_world(world), m_partList(partList), m_locationSetToPart(new SetsToPartMap())
{
}


Decompose::~Decompose()
{
}

void Decompose::decompose()
{
	shared_ptr<DLXSolver> dlxSolver(new DLXSolver(m_world->getNumberOfPoints())); // Creates DLXSolver for the decomposition
	
	// For each part in the part list creates a new visitor of the world. Runs world.accept() on the visitor
	for each (const PartPtr& part in *m_partList) {
		IWorldVisitorPtr visitor(new IsPartFitVisitor(part, dlxSolver, m_locationSetToPart));
		m_world->accept(visitor);
	}

	auto solutions = dlxSolver->solve();
	
	printSolutions(solutions);
}



#include <iostream>
using namespace std;

void Decompose::printSolutions(std::vector<DLX_SOLUTION> solutions)
{
	if (solutions.empty())
	{
		cout << "No solutions found" << endl;
		return;
	}

	int solutionNum = 1;

	for each(const DLX_SOLUTION& solution in solutions)
	{
		if (solutionNum > 10)
		{
			cout << "+" << (solutions.size() - 10) << " solutions omitted." << endl << endl;
			break;
		}

		int setNum = 1;
		cout << "Solution " << solutionNum << ":" << endl;

		for each(const DLX_VALUES_SET& currSet in solution)
		{
			cout << "Set #" << setNum << ": { ";

			for each(const int& value in currSet)
			{
				cout << value << " ";
			}

			cout << "}" << endl;
			setNum++;
		}

		solutionNum++;
		cout << endl << endl;
	}
}