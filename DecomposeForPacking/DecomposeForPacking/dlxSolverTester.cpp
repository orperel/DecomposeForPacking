#include "dlxSolverTester.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <algorithm>
#include <time.h>

using std::cout;
using std::endl;
using std::cin;
using std::string;

dlxSolverTester::dlxSolverTester()
{
	srand(time(NULL));
}


dlxSolverTester::~dlxSolverTester()
{
}

void dlxSolverTester::runTests()
{
	DLXSolverTesterConfig* configuration = new DLXSolverTesterConfig();
	configuration->printSolutions = true;
	configuration->measureTime = true;
	configuration->verifySolution = true;
	configuration->printSolverData = false;

	simpleTest(configuration);
	doubleCoverTest(configuration);
	wikipediaCoverTest(configuration);
	largeScaleRandomTest(configuration);
	emptyRowsTest(configuration);
	emptyColsTest(configuration);
	singleSetTest(configuration);
	singleSetImpossibleCoverTest(configuration);
	doubleSetImpossibleCoverTest(configuration);
	oneValueEachSetCoverTest(configuration);
	identicalSetsCoverTest(configuration);
	simpleOptionalCoverTest(configuration);
	oneMandatoryRestOptionalCoverTest(configuration);
	oneOptionalRestMandatoryCoverTest(configuration);
	onlyOptionalsCoverTest(configuration);
	largeScaleRandomWithOptionalTest(configuration);
	particalCoverSuccessTest(configuration);
	noParticalCoverSuccessTest(configuration);

	delete configuration;

	// Hold the console
	cout << "Tester complete." << endl << "Press any key to continue." << endl;
	int x;
	cin >> x;
}

void dlxSolverTester::solveAndPrint(DLXSolver& solver, int universeSize, DLXSolverTesterConfig* configuration,
									int optionalColumns, bool isPartialSolve)
{
	if (!configuration->measureTime &&
		!configuration->printSolutions &&
		!configuration->printSolverData &&
		!configuration->verifySolution) {
		solver.solve(isPartialSolve);
	}
	else {
		MEASURED_RESULT results = runSolverWithMeasures(solver, isPartialSolve);

		if (configuration->printSolverData)
			cout << solver;

		printMeasuredResultsAndSolutions(results, universeSize, configuration, optionalColumns);
	}

	cout << endl;
}

void dlxSolverTester::verifyUniverseCovered(std::vector<DLX_SOLUTION> solutions, int mandatoryCols, int optionalCols)
{
	if (solutions.empty())
	{
		cout << "Universe cover verification: 0 solutions (verification passes automatically)." << endl;
		return;
	}

	bool isSuccess = true;

	for each(const DLX_SOLUTION& solution in solutions)
	{
		set<int> universe;

		for (int index = optionalCols; index < optionalCols + mandatoryCols; index++)
		{
			universe.insert(index);
		}

		for each(const DLX_VALUES_SET& currSet in solution)
		{
			for each(const int& value in currSet)
			{
				universe.erase(value);
			}
		}

		if (!universe.empty())
		{
			isSuccess = false;
		}
	}

	string status = isSuccess ? "Passed" : "Failed";
	cout << "Universe cover verification: " << status << endl;
}

void dlxSolverTester::printSolutions(std::vector<DLX_SOLUTION> solutions)
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

string solverToString(DLXSolver& solver)
{
	std::ostringstream stringStream;
	stringStream << solver;
	string content = stringStream.str();

	return content;
}

MEASURED_RESULT dlxSolverTester::runSolverWithMeasures(DLXSolver& solver, bool isPartialSolve)
{
	string preSolveContent = solverToString(solver);

	auto start = std::chrono::system_clock::now();
	auto results = solver.solve(isPartialSolve);
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);

	string postSolveContent = solverToString(solver);

	bool isSolverIntact = preSolveContent.compare(postSolveContent) == 0;

	return std::make_tuple(std::make_shared<vector<DLX_SOLUTION>>(results), duration, isSolverIntact);
}

void dlxSolverTester::printMeasuredResultsAndSolutions(MEASURED_RESULT measures, int universeSize, DLXSolverTesterConfig* configuration, int optionalColumns)
{
	auto solutions = std::get<0>(measures);
	auto duration = std::get<1>(measures);
	bool isSolverIntact = std::get<2>(measures);

	if (configuration->printSolutions)
	{
		printSolutions(*solutions);
	}

	if (configuration->verifySolution)
	{
		verifyUniverseCovered(*solutions, universeSize, optionalColumns);
		cout << "Solver matrix coherence test: " << (isSolverIntact ? "Passed" : "Failed") << endl;
	}

	if (configuration->measureTime)
	{
		cout << "Algorithm X compute time: " << duration.count() << "ms" << endl;
	}
}

void dlxSolverTester::simpleTest(DLXSolverTesterConfig* configuration)
{
	cout << "<< Simple cover test >>" << endl;
	int elementCount = 5;
	DLXSolver solver(elementCount);

	auto set1 = std::make_shared<DLX_VALUES_SET>();
	set1->insert(1);
	set1->insert(3);
	solver.addRow(set1);

	auto set2 = std::make_shared<DLX_VALUES_SET>();
	set2->insert(2);
	set2->insert(4);
	set2->insert(0);
	solver.addRow(set2);

	auto set3 = std::make_shared<DLX_VALUES_SET>();
	set3->insert(2);
	set3->insert(1);
	set3->insert(3);
	solver.addRow(set3);

	auto set4 = std::make_shared<DLX_VALUES_SET>();
	set4->insert(0);
	solver.addRow(set4);

	solveAndPrint(solver, elementCount, configuration);
}

void dlxSolverTester::doubleCoverTest(DLXSolverTesterConfig* configuration)
{
	cout << "<< Double cover test >>" << endl;
	int elementCount = 5;
	DLXSolver solver(elementCount);

	auto set1 = std::make_shared<DLX_VALUES_SET>();
	set1->insert(1);
	set1->insert(3);
	solver.addRow(set1);

	auto set5 = std::make_shared<DLX_VALUES_SET>();
	set5->insert(4);
	set5->insert(0);
	solver.addRow(set5);

	auto set2 = std::make_shared<DLX_VALUES_SET>();
	set2->insert(2);
	set2->insert(4);
	set2->insert(0);
	solver.addRow(set2);

	auto set3 = std::make_shared<DLX_VALUES_SET>();
	set3->insert(2);
	set3->insert(1);
	set3->insert(3);
	solver.addRow(set3);

	auto set4 = std::make_shared<DLX_VALUES_SET>();
	set4->insert(0);
	solver.addRow(set4);

	solveAndPrint(solver, elementCount, configuration);
}

void dlxSolverTester::wikipediaCoverTest(DLXSolverTesterConfig* configuration)
{
	cout << "<< Wikipedia values cover test >>" << endl;
	int elementCount = 7;
	DLXSolver solver(elementCount);

	auto set1 = std::make_shared<DLX_VALUES_SET>();
	set1->insert(0);
	set1->insert(3);
	set1->insert(6);
	solver.addRow(set1);

	auto set2 = std::make_shared<DLX_VALUES_SET>();
	set2->insert(0);
	set2->insert(3);
	solver.addRow(set2);

	auto set3 = std::make_shared<DLX_VALUES_SET>();
	set3->insert(3);
	set3->insert(4);
	set3->insert(6);
	solver.addRow(set3);

	auto set4 = std::make_shared<DLX_VALUES_SET>();
	set4->insert(2);
	set4->insert(4);
	set4->insert(5);
	solver.addRow(set4);

	auto set5 = std::make_shared<DLX_VALUES_SET>();
	set5->insert(1);
	set5->insert(2);
	set5->insert(5);
	set5->insert(6);
	solver.addRow(set5);

	auto set6 = std::make_shared<DLX_VALUES_SET>();
	set6->insert(1);
	set6->insert(6);
	solver.addRow(set6);

	solveAndPrint(solver, elementCount, configuration);
}

void dlxSolverTester::largeScaleRandomTest(DLXSolverTesterConfig* configuration)
{
	cout << "<< Large scale random test >>" << endl;
	int elementCount = 10;
	int numOfSets = 100;

	DLXSolver solver(elementCount);

	for (int setIndex = 0; setIndex < numOfSets; setIndex++)
	{
		auto currSet = std::make_shared<DLX_VALUES_SET>();
		
		int randomNumOfValues = rand() % elementCount;

		for (int valIndex = 0; valIndex < elementCount/2; valIndex++)
		{
			int nextVal = rand() % elementCount;
			currSet->insert(nextVal);
		}

		solver.addRow(currSet);
	}

	solveAndPrint(solver, elementCount, configuration);
}

void dlxSolverTester::emptyRowsTest(DLXSolverTesterConfig* configuration)
{
	cout << "<< Empty rows cover test >>" << endl;
	int elementCount = 5;
	DLXSolver solver(elementCount);

	solveAndPrint(solver, elementCount, configuration);
}

void dlxSolverTester::emptyColsTest(DLXSolverTesterConfig* configuration)
{
	cout << "<< Empty columns cover test >>" << endl;
	int elementCount = 0;
	DLXSolver solver(elementCount);

	solveAndPrint(solver, elementCount, configuration);
}

void dlxSolverTester::singleSetTest(DLXSolverTesterConfig* configuration)
{
	cout << "<< Single set cover test >>" << endl;
	int elementCount = 5;
	DLXSolver solver(elementCount);

	auto set1 = std::make_shared<DLX_VALUES_SET>();
	set1->insert(1);
	set1->insert(0);
	set1->insert(3);
	set1->insert(2);
	set1->insert(4);
	solver.addRow(set1);

	solveAndPrint(solver, elementCount, configuration);
}

void dlxSolverTester::singleSetImpossibleCoverTest(DLXSolverTesterConfig* configuration)
{
	cout << "<< Single set impossible cover test >>" << endl;
	int elementCount = 5;
	DLXSolver solver(elementCount);

	auto set1 = std::make_shared<DLX_VALUES_SET>();
	set1->insert(1);
	set1->insert(0);
	set1->insert(2);
	set1->insert(4);
	solver.addRow(set1);

	solveAndPrint(solver, elementCount, configuration);
}

void dlxSolverTester::doubleSetImpossibleCoverTest(DLXSolverTesterConfig* configuration)
{
	cout << "<< Double sets impossible cover test >>" << endl;
	int elementCount = 5;
	DLXSolver solver(elementCount);

	auto set1 = std::make_shared<DLX_VALUES_SET>();
	set1->insert(1);
	set1->insert(0);
	set1->insert(2);
	set1->insert(4);
	solver.addRow(set1);

	auto set2 = std::make_shared<DLX_VALUES_SET>();
	set2->insert(1);
	set2->insert(2);
	set2->insert(3);
	set2->insert(4);
	solver.addRow(set2);

	solveAndPrint(solver, elementCount, configuration);
}

void dlxSolverTester::oneValueEachSetCoverTest(DLXSolverTesterConfig* configuration)
{
	cout << "<< One value each set cover test >>" << endl;
	int elementCount = 5;
	DLXSolver solver(elementCount);

	auto set1 = std::make_shared<DLX_VALUES_SET>();
	set1->insert(1);
	solver.addRow(set1);

	auto set2 = std::make_shared<DLX_VALUES_SET>();
	set2->insert(2);
	solver.addRow(set2);

	auto set3 = std::make_shared<DLX_VALUES_SET>();
	set3->insert(3);
	solver.addRow(set3);

	auto set4 = std::make_shared<DLX_VALUES_SET>();
	set4->insert(4);
	solver.addRow(set4);

	auto set5 = std::make_shared<DLX_VALUES_SET>();
	set5->insert(0);
	solver.addRow(set5);

	solveAndPrint(solver, elementCount, configuration);
}

void dlxSolverTester::identicalSetsCoverTest(DLXSolverTesterConfig* configuration)
{
	cout << "<< Identical sets cover test >>" << endl;
	int elementCount = 5;
	DLXSolver solver(elementCount);

	for (int i = 0; i < 2; i++)
	{
		auto set1 = std::make_shared<DLX_VALUES_SET>();
		set1->insert(1);
		set1->insert(3);
		solver.addRow(set1);

		auto set2 = std::make_shared<DLX_VALUES_SET>();
		set2->insert(2);
		set2->insert(4);
		set2->insert(0);
		solver.addRow(set2);

		auto set3 = std::make_shared<DLX_VALUES_SET>();
		set3->insert(2);
		set3->insert(1);
		set3->insert(3);
		solver.addRow(set3);

		auto set4 = std::make_shared<DLX_VALUES_SET>();
		set4->insert(0);
		solver.addRow(set4);
	}

	solveAndPrint(solver, elementCount, configuration);
}

void dlxSolverTester::simpleOptionalCoverTest(DLXSolverTesterConfig* configuration)
{
	cout << "<< Simple cover with 2,4 optionals missing test >>" << endl;
	cout << "[Note: 2, 4 should be missing from solution]" << endl;
	int elementCount = 2;
	int optionalCount = 5;
	DLXSolver solver(optionalCount, elementCount);

	auto set1 = std::make_shared<DLX_VALUES_SET>();
	set1->insert(1);
	set1->insert(3);
	set1->insert(5);
	solver.addRow(set1);

	auto set2 = std::make_shared<DLX_VALUES_SET>();
	set2->insert(2);
	set2->insert(4);
	set2->insert(0);
	solver.addRow(set2);

	auto set3 = std::make_shared<DLX_VALUES_SET>();
	set3->insert(2);
	set3->insert(1);
	set3->insert(3);
	solver.addRow(set3);

	auto set4 = std::make_shared<DLX_VALUES_SET>();
	set4->insert(0);
	set4->insert(6);
	solver.addRow(set4);

	solveAndPrint(solver, elementCount, configuration, optionalCount);
}


void dlxSolverTester::oneMandatoryRestOptionalCoverTest(DLXSolverTesterConfig* configuration)
{
	cout << "<< Only one mandatory and the rest are optional test >>" << endl;
	cout << "[Note: Only 0, 6 should appear in the solution]" << endl;
	int elementCount = 1;
	int optionalCount = 6;
	DLXSolver solver(optionalCount, elementCount);

	auto set1 = std::make_shared<DLX_VALUES_SET>();
	set1->insert(1);
	set1->insert(3);
	set1->insert(5);
	solver.addRow(set1);

	auto set2 = std::make_shared<DLX_VALUES_SET>();
	set2->insert(2);
	set2->insert(4);
	set2->insert(0);
	solver.addRow(set2);

	auto set3 = std::make_shared<DLX_VALUES_SET>();
	set3->insert(2);
	set3->insert(1);
	set3->insert(3);
	solver.addRow(set3);

	auto set4 = std::make_shared<DLX_VALUES_SET>();
	set4->insert(0);
	set4->insert(6);
	solver.addRow(set4);

	solveAndPrint(solver, elementCount, configuration, optionalCount);
}

void dlxSolverTester::oneOptionalRestMandatoryCoverTest(DLXSolverTesterConfig* configuration)
{
	cout << "<< Only one optional and the rest are mandatory test >>" << endl;
	cout << "[Note: Only 0 may not appear in the solution]" << endl;
	int elementCount = 6;
	int optionalCount = 1;
	DLXSolver solver(optionalCount, elementCount);

	auto set1 = std::make_shared<DLX_VALUES_SET>();
	set1->insert(1);
	set1->insert(3);
	set1->insert(5);
	solver.addRow(set1);

	auto set2 = std::make_shared<DLX_VALUES_SET>();
	set2->insert(2);
	set2->insert(4);
	solver.addRow(set2);

	auto set3 = std::make_shared<DLX_VALUES_SET>();
	set3->insert(2);
	set3->insert(1);
	set3->insert(3);
	set3->insert(0);
	solver.addRow(set3);

	auto set4 = std::make_shared<DLX_VALUES_SET>();
	set4->insert(6);
	solver.addRow(set4);

	solveAndPrint(solver, elementCount, configuration, optionalCount);
}

void dlxSolverTester::onlyOptionalsCoverTest(DLXSolverTesterConfig* configuration)
{
	cout << "<< Only optionals test >>" << endl;
	cout << "[Note: test should return nothing]" << endl;
	int elementCount = 0;
	int optionalCount = 7;
	DLXSolver solver(optionalCount, elementCount);

	auto set1 = std::make_shared<DLX_VALUES_SET>();
	set1->insert(1);
	set1->insert(3);
	set1->insert(5);
	solver.addRow(set1);

	auto set2 = std::make_shared<DLX_VALUES_SET>();
	set2->insert(2);
	set2->insert(4);
	solver.addRow(set2);

	auto set3 = std::make_shared<DLX_VALUES_SET>();
	set3->insert(2);
	set3->insert(1);
	set3->insert(3);
	set3->insert(0);
	solver.addRow(set3);

	auto set4 = std::make_shared<DLX_VALUES_SET>();
	set4->insert(6);
	solver.addRow(set4);

	solveAndPrint(solver, elementCount, configuration, optionalCount);
}

void dlxSolverTester::largeScaleRandomWithOptionalTest(DLXSolverTesterConfig* configuration)
{
	cout << "<< Large scale random with optional test >>" << endl;
	cout << "[Note: 10, 11, 12, 13, 14 are mandatory. Test may take a while. " << endl;
	int elementCount = 5;
	int optionalCount = 10;
	int totalUniverseRange = elementCount + optionalCount;
	int numOfSets = 200;

	DLXSolver solver(optionalCount, elementCount);

	for (int setIndex = 0; setIndex < numOfSets; setIndex++)
	{
		auto currSet = std::make_shared<DLX_VALUES_SET>();

		int randomNumOfValues = std::max(3, rand() % optionalCount / 2);

		for (int valIndex = 0; valIndex < randomNumOfValues; valIndex++)
		{
			int nextVal = rand() % optionalCount;
			currSet->insert(nextVal);
		}

		int nextMandatory = (rand() % elementCount) + optionalCount;
		currSet->insert(nextMandatory);
		solver.addRow(currSet);
	}

	solveAndPrint(solver, elementCount, configuration, optionalCount);
}

void dlxSolverTester::particalCoverSuccessTest(DLXSolverTesterConfig* configuration)
{
	cout << "<< Partial cover simple test (1 solution of 2 sets) >>" << endl;
	int elementCount = 5;
	DLXSolver solver(elementCount);

	auto set1 = std::make_shared<DLX_VALUES_SET>();
	set1->insert(1);
	set1->insert(2);
	set1->insert(4);
	solver.addRow(set1);

	auto set2 = std::make_shared<DLX_VALUES_SET>();
	set2->insert(2);
	set2->insert(4);
	set2->insert(0);
	solver.addRow(set2);

	auto set3 = std::make_shared<DLX_VALUES_SET>();
	set3->insert(2);
	set3->insert(1);
	set3->insert(3);
	solver.addRow(set3);

	auto set4 = std::make_shared<DLX_VALUES_SET>();
	set4->insert(0);
	solver.addRow(set4);

	solveAndPrint(solver, elementCount, configuration, 0, true);
}

void dlxSolverTester::noParticalCoverSuccessTest(DLXSolverTesterConfig* configuration)
{
	cout << "<< Partial cover one possible solution test (1 solutions hides 2 solutions) >>" << endl;
	int elementCount = 5;
	DLXSolver solver(elementCount);

	auto set1 = std::make_shared<DLX_VALUES_SET>();
	set1->insert(1);
	set1->insert(2);
	set1->insert(4);
	solver.addRow(set1);

	auto set2 = std::make_shared<DLX_VALUES_SET>();
	set2->insert(2);
	set2->insert(4);
	set2->insert(0);
	solver.addRow(set2);

	auto set3 = std::make_shared<DLX_VALUES_SET>();
	set3->insert(2);
	set3->insert(1);
	set3->insert(3);
	solver.addRow(set3);

	auto set4 = std::make_shared<DLX_VALUES_SET>();
	set4->insert(0);
	set4->insert(1);
	solver.addRow(set4);

	solveAndPrint(solver, elementCount, configuration, 0, true);
}