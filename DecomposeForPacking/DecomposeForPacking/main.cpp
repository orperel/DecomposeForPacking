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
#include "DecomposeAndPack.h"


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

		std::string path = "../../tet.bmp";
		//std::string path = "../../pretzel.bmp";
		//std::string path = "../../obj4.bmp";
		std::shared_ptr<CImg<int>> orig(new CImg<int>(path.c_str()));
		WorldPtr world = WorldBuilder::fromImage(orig, 10);

		DecomposeAndPack dp(world);
		DecomposeAndPackResult res = dp.run();

		// Display - will be replaced with Or code...
		shared_ptr<vector<shared_ptr<CImgDisplay>>> displayVector(new vector<shared_ptr<CImgDisplay>>());
		displayVector->push_back(DisplayHelper::showWorld(world));
		shared_ptr<vector<shared_ptr<CImgDisplay>>> displayVector2 = DisplayHelper::showResult(world, std::get<0>(res), 3);
		shared_ptr<vector<shared_ptr<CImgDisplay>>> displayVector3 = DisplayHelper::showResult(world, std::get<1>(res), 3);

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