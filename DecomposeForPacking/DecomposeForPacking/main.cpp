#pragma once

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



int main(int argc, char *argv[]) {
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