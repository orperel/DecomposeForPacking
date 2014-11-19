#pragma once

// Uncomment this line to run tester instead
//#define RUN_TESTS 

#ifdef RUN_TESTS
	#include "dlxSolverTester.h"
#else
	#include <iostream>
	#include <string>
	#include "World.h"
	#include "Part.h"
	#include "Decompose.h"
	#include "WorldBuilder.h"
	#include "CImg.h"
	#include "PartBuilder.h"
	#include "DecomposeResult.h"

	using namespace cimg_library;
	using namespace std;
#endif
#include "DisplayHelper.h"


#ifdef RUN_TESTS
	void runTests()
	{
		dlxSolverTester tester;
		tester.runTests();
	}
#else
	void runProd()
	{
		shared_ptr<vector<shared_ptr<CImgDisplay>>> displayVector(new vector<shared_ptr<CImgDisplay>>());

		WorldPtr world = WorldBuilder::fromImage("../../obj.bmp");

		PartListPtr partList = PartBuilder::buildStandartPartPack(10);

		Decompose decomposer(world, partList);

		cout << "Starting to decompose..." << endl;

		DecomposeResult decomposeResult = decomposer.decompose();

		displayVector->push_back(DisplayHelper::showWorld(world));

		//shared_ptr<CImg<unsigned char>> img = PartBuilder::toImage((*partList)[0]);

		//new CImgDisplay(*img);

		shared_ptr<vector<shared_ptr<CImgDisplay>>> displayVector2 = DisplayHelper::showDecomposeResult(world, decomposeResult, 10);

		int x;

		cin >> x;
	}
#endif

int main(int argc, char *argv[]) {

#ifdef RUN_TESTS
	runTests();
#else
	runProd();
#endif
}