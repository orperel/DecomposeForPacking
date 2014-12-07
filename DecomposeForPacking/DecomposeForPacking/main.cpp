#pragma once

// Uncomment this line to run tester instead
//#define RUN_TESTS 

#ifdef RUN_TESTS
	#include "dlxSolverTester.h"
#else
	#include <iostream>
	#include <string>
	#include <math.h>
	#include "World.h"
	#include "Part.h"
	#include "Decompose.h"
	#include "WorldBuilder.h"
	#include "CImg.h"
	#include "PartBuilder.h"
	#include "DecomposeResult.h"
	#include "Packing.h"
	#include "ObjMesh.h"

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
		//// 3D Object
		//ObjMeshPtr cube(new ObjMesh(L"../../cube.obj"));
		//ObjMeshPtr teapot(new ObjMesh(L"../../teapot.obj"));
		//WorldPtr world = WorldBuilder::fromMesh(teapot);

		shared_ptr<vector<shared_ptr<CImgDisplay>>> displayVector(new vector<shared_ptr<CImgDisplay>>());

		std::string path = "../../tet.bmp";
		//std::string path = "../../pretzel.bmp";
		//std::string path = "../../obj4.bmp";
		std::shared_ptr<CImg<int>> orig(new CImg<int>(path.c_str()));
		WorldPtr world = WorldBuilder::fromImage(orig, 10);

		displayVector->push_back(DisplayHelper::showWorld(world));

		PartListPtr partList = PartBuilder::buildStandartPartPack(1);

		Decompose decomposer(world, partList);

		cout << "Starting decomposing..." << endl;

		shared_ptr<DecomposeResult> decomposeResult = decomposer.decompose();

		cout << "Finished decomposing..." << endl;

		// Display Part - should be deleted
		//shared_ptr<CImg<unsigned char>> img = PartBuilder::toImage((*partList)[0]);
		//new CImgDisplay(*img);

		shared_ptr<vector<shared_ptr<CImgDisplay>>> displayVector2 = DisplayHelper::showResult(world, decomposeResult->getListOfPartLocationLists(), 3);

		int width, height;

		shared_ptr<PackResult> packResult;

		cout << "Starting packing..." << endl;

		do {
			width = ceil(sqrt(world->getNumberOfPoints()));
			height = width;
			WorldPtr box = WorldBuilder::buildBox(width, height);
			Packing packer(box, decomposeResult);
			packResult = packer.pack();

			if (packResult->hasSolution()) {
				cout << "Found Solution!!!" << endl;
				break;
			}

			width = ceil(width + 0.1*width);
			height = width;
		} while (world->getWidth() >= width && world->getHeight() >= height);

		cout << "Finished packing..." << endl;

		shared_ptr<vector<shared_ptr<CImgDisplay>>> displayVector3 = DisplayHelper::showResult(world, packResult->getPackPerDecomposeList(), 3);

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