#pragma once

#include <string>
#include <memory>
#include "IWorldVisitor.h"
#include "CImg.h"
#include "World.h"

using namespace cimg_library;

class WorldBuilder
{
public:
	static WorldPtr fromImage(std::string path);

	static std::shared_ptr<CImg<unsigned char>> toImage(WorldPtr world);
};

