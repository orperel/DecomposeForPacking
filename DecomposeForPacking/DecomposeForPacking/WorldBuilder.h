#pragma once

#include <string>
#include <memory>
#include "IWorldVisitor.h"
#include "CImg.h"
#include "World.h"
#include "ObjMesh.h"

using namespace cimg_library;

struct WorldPosition {
	int min_X;
	int max_X;
	int min_Y;
	int max_Y;
};

class WorldBuilder
{
public:
	static WorldPtr fromImage(std::shared_ptr<CImg<int>> img, int ratio = 1);

	static WorldPtr WorldBuilder::fromMesh(ObjMeshPtr mesh);

	static std::shared_ptr<CImg<unsigned char>> toImage(WorldPtr world);

private:
	static WorldPosition getWorldPosition(std::shared_ptr<CImg<int>> img);

	static bool isSquareNotEmpty(std::shared_ptr<CImg<int>> img, Point point, int size);
};

