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

	using namespace cimg_library;
	using namespace std;
#endif

#ifdef RUN_TESTS
	void runTests()
	{
		dlxSolverTester tester;
		tester.runTests();
	}
#else
	void runProd()
	{
		WorldPtr world = WorldBuilder::fromImage("../../obj.bmp");

	PartListPtr partList = PartBuilder::buildStandartPartPack(10);

	Decompose decomposer(world, partList);

	decomposer.decompose();

	shared_ptr<CImg<unsigned char>> img = PartBuilder::toImage((*partList)[0]);

	new CImgDisplay(*img);

	shared_ptr<CImg<unsigned char>> img2 = WorldBuilder::toImage(world);

	new CImgDisplay(*img2);

		cout << "Test proj" << endl;

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