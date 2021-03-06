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
	#include "GL2DDisplayHelper.h"
	#include "GL3DDisplayHelper.h"
	#include "PrimeNumbersModule.h"

	using namespace cimg_library;
	using namespace std;
#endif
#include "DisplayHelper.h"
#include "DecomposeAndPack.h"
#include "SameDecompose.h"


#ifdef RUN_TESTS
	void runTests()
	{
		dlxSolverTester tester;
		tester.runTests();
	}
#else
	void runProd()
	{
		// 3D Object
		//ObjMeshPtr cube(new ObjMesh(L"../../cube.obj", 3));
		//ObjMeshPtr teapot(new ObjMesh(L"../../teapot.obj"));
		//ObjMeshPtr lowTeapot(new ObjMesh(L"../../lowpolyTeapot.obj"));
		//ObjMeshPtr knot(new ObjMesh(L"../../knot.obj", 10));
		//ObjMeshPtr sample(new ObjMesh(L"../../sample.obj", 4));
		ObjMeshPtr sample2(new ObjMesh(L"../../sample2.obj", 6));
		//ObjMeshPtr lamp(new ObjMesh(L"../../lamp.obj", 11));
		//ObjMeshPtr sample3(new ObjMesh(L"../../sample3.obj", 9));
		WorldPtr world = WorldBuilder::fromMesh(sample2);

		// Other stuff
		//std::string path = "../../tet.bmp";
		//std::string path = "../../pretzel.bmp";
		//std::string path = "../../obj4.bmp";
		//std::string path = "../../duck.bmp";


		//// Example #1
		//std::string path = "../../pptSample.bmp";
		//std::shared_ptr<CImg<int>> orig(new CImg<int>(path.c_str()));
		//WorldPtr world = WorldBuilder::fromImage(orig, 4);

		// Example #2
		//std::string path = "../../pptSample2.bmp";
		//std::shared_ptr<CImg<int>> orig(new CImg<int>(path.c_str()));
		//WorldPtr world = WorldBuilder::fromImage(orig, 1);

		DecomposeAndPack dp(world);
		DecomposeAndPackResult decomposeResult = dp.run();

		// Display 2d results
		if (world->getDepth() == 1)
		{
			GL2DDisplayHelper displayHelper;
			displayHelper.displayResults(world, decomposeResult);
		}
		else // Display 3d results
		{
			GL3DDisplayHelper displayHelper;
			displayHelper.displayResults(world, decomposeResult);
		}

		// Same Decompose
		//std::string h_img = "../../h.bmp";
		//std::string house_img = "../../house.bmp";
		//std::shared_ptr<CImg<int>> orig1(new CImg<int>(h_img.c_str()));
		//WorldPtr h = WorldBuilder::fromImage(orig1, 1);
		//std::shared_ptr<CImg<int>> orig2(new CImg<int>(house_img.c_str()));
		//WorldPtr house = WorldBuilder::fromImage(orig2, 1);
		//SameDecompose sd(house, h);
		//std::shared_ptr<vector<PartLocationListPtr>> sameDecomposeResult = sd.run();

		// Display - deprecated
		//shared_ptr<vector<shared_ptr<CImgDisplay>>> displayVector(new vector<shared_ptr<CImgDisplay>>());
		//displayVector->push_back(DisplayHelper::showWorld(world));
		//shared_ptr<vector<shared_ptr<CImgDisplay>>> displayVector2 = DisplayHelper::showResult(world, std::get<0>(res), 3);
		//shared_ptr<vector<shared_ptr<CImgDisplay>>> displayVector3 = DisplayHelper::showResult(world, std::get<1>(res), 3);

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