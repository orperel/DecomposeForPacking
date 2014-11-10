#pragma once

#include <iostream>
#include <string>
#include "World.h"
#include "Part.h"
#include "Decompose.h"
#include "WorldBuilder.h"
#include "CImg.h"

using namespace cimg_library;
using namespace std;

int main(int argc, char *argv[]) {
	WorldPtr world = WorldBuilder::fromImage("../../obj.bmp");

	PartPtr part(new Part(10));

	int lastIndex = part->addPointToRight(0);
	part->addPointToRight(lastIndex);

	PartList partList;
	partList.push_back(part);

	Decompose decompose(world, partList);

	decompose.decompose();

	//shared_ptr<CImg<unsigned char>> img = WorldBuilder::toImage(world);

	//new CImgDisplay(*img);

	cout << "Test proj" << endl;
	
	int x;

	cin >> x;
}