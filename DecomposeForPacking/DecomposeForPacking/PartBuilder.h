#pragma once

#include <string>
#include <memory>
#include "CImg.h"
#include "Part.h"

using namespace cimg_library;

class PartBuilder
{
public:
	static PartPtr buildLongPart(int partWidth, int pixelSize = 1);

	static PartPtr buildCornerPart(int partWidth, int pixelSize = 1);
	
	static PartPtr buildZigzagPart(int partWidth, int pixelSize = 1);
	
	static PartPtr buildTPart(int partWidth, int pixelSize = 1);

	static PartPtr buildTPart(int partWidth, int pixelSize = 1);

	static PartListPtr buildStandartPartPack(int pixelSize = 1);

	static std::shared_ptr<CImg<unsigned char>> toImage(PartPtr part);
};

