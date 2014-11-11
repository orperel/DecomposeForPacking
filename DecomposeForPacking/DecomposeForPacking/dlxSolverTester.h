#pragma once

#include <memory>
#include <vector>
#include "DLXSolver.h"
#include <tuple>
#include <chrono>

typedef std::tuple<shared_ptr<vector<DLX_SOLUTION>>, std::chrono::milliseconds, bool> MEASURED_RESULT;

struct DLXSolverTesterConfig
{
	bool printSolutions;
	bool measureTime;
	bool verifySolution;
	bool printSolverData;
};

class dlxSolverTester
{
public:
	dlxSolverTester();
	~dlxSolverTester();

	void runTests();
private:
	// Tests
	void simpleTest(DLXSolverTesterConfig* configuration);
	void doubleCoverTest(DLXSolverTesterConfig* configuration);
	void wikipediaCoverTest(DLXSolverTesterConfig* configuration);
	void largeScaleRandomTest(DLXSolverTesterConfig* configuration);
	void emptyRowsTest(DLXSolverTesterConfig* configuration);
	void emptyColsTest(DLXSolverTesterConfig* configuration);
	void singleSetTest(DLXSolverTesterConfig* configuration);
	void singleSetImpossibleCoverTest(DLXSolverTesterConfig* configuration);
	void doubleSetImpossibleCoverTest(DLXSolverTesterConfig* configuration);
	void oneValueEachSetCoverTest(DLXSolverTesterConfig* configuration);
	void identicalSetsCoverTest(DLXSolverTesterConfig* configuration);
	void simpleOptionalCoverTest(DLXSolverTesterConfig* configuration);
	void oneMandatoryRestOptionalCoverTest(DLXSolverTesterConfig* configuration);
	void oneOptionalRestMandatoryCoverTest(DLXSolverTesterConfig* configuration);
	void onlyOptionalsCoverTest(DLXSolverTesterConfig* configuration);
	void largeScaleRandomWithOptionalTest(DLXSolverTesterConfig* configuration);

	// Test control methods
	void solveAndPrint(DLXSolver& solver, int universeSize, DLXSolverTesterConfig* configuration, int optionalColumns = 0);
	MEASURED_RESULT runSolverWithMeasures(DLXSolver& solver);
	void printMeasuredResultsAndSolutions(MEASURED_RESULT measures, int universeSize, DLXSolverTesterConfig* configuration, int optionalColumns = 0);
	void verifyUniverseCovered(std::vector<DLX_SOLUTION> solutions, int mandatoryCols, int optionalCols = 0);
	void printSolutions(std::vector<DLX_SOLUTION> solutions);
};

