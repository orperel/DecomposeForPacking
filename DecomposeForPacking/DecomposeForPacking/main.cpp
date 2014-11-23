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

		//std::string path = "../../duck.bmp";
		std::string path = "../../duck2.bmp";
		//std::string path = "../../obj.bmp";
		std::shared_ptr<CImg<int>> orig(new CImg<int>(path.c_str()));
		WorldPtr world = WorldBuilder::fromImage(orig, 25);

		displayVector->push_back(DisplayHelper::showWorld(world));
		//displayVector->push_back(std::shared_ptr<CImgDisplay>(new CImgDisplay(*orig)));

		PartListPtr partList = PartBuilder::buildStandartPartPack(1);

		Decompose decomposer(world, partList);

		cout << "Starting decomposing..." << endl;

		shared_ptr<DecomposeResult> decomposeResult = decomposer.decompose();

		cout << "Finished decomposing..." << endl;

		// Display Part - should be deleted
		//shared_ptr<CImg<unsigned char>> img = PartBuilder::toImage((*partList)[0]);
		//new CImgDisplay(*img);

		shared_ptr<vector<shared_ptr<CImgDisplay>>> displayVector2 = DisplayHelper::showDecomposeResult(world, *decomposeResult, 3);

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