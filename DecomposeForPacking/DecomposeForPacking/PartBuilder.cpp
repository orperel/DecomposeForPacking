#include "PartBuilder.h"

const int STANDART_T_PART_WIDTH = 1;
const int STANDART_CORNER_PART_WIDTH = 2;
const int STANDART_ZIGZAG_PART_WIDTH = 2;
const int STANDART_LONG_PART_WIDTH = 3;

std::shared_ptr<CImg<unsigned char>> PartBuilder::toImage(PartPtr part)
{
	int width = 0;
	int height = 0;

	//for each (const Point& point in *part->getPartOrientationByIndex(2)->getPointList()) {
	//	if (point.getX() > width) width = point.getX();
	//	if (point.getY() > height) height = point.getY();
	//}

	width = 1000;
	height = 600;

	std::shared_ptr<CImg<unsigned char>> img(new CImg<unsigned char>(width + 1, height + 1));

	for each (const Point& point in *part->getPartOrientationByIndex(3)->getPointList()) {
		(*img->data(point.getX() + 500, point.getY() + 300)) = 0;
	}

	return img;
}

PartPtr PartBuilder::buildLongPart(int partWidth, int pixelSize /*= 1*/, bool is3D /*= false*/)
{
	PartOrientationPtr partOrient(new PartOrientation(pixelSize));

	int lastIndex = 0;
	for (int i = 0; i < partWidth - 1; i++) {
		lastIndex = partOrient->addPointToRight(lastIndex);
	}

	PartPtr part(new Part(partOrient, is3D));
	return part;
}

PartListPtr PartBuilder::buildStandartPartPack(int pixelSize /*= 1*/, bool is3D /*= false*/)
{
	PartListPtr partList(new PartList());

	partList->push_back(buildLongPart(STANDART_LONG_PART_WIDTH, pixelSize, is3D));
	partList->push_back(buildLongPart(2, pixelSize, is3D));
	partList->push_back(buildZigzagPart(STANDART_ZIGZAG_PART_WIDTH, pixelSize, is3D));
	partList->push_back(buildCornerPart(STANDART_CORNER_PART_WIDTH, pixelSize, is3D));	
	partList->push_back(buildTPart(STANDART_T_PART_WIDTH, pixelSize, is3D));

	// Display Part - TODO: should be deleted
	//shared_ptr<CImg<unsigned char>> img = PartBuilder::toImage((*partList)[0]);
	//new CImgDisplay(*img);

	return partList;
}

PartPtr PartBuilder::buildCornerPart(int partWidth, int pixelSize /*= 1*/, bool is3D /*= false*/)
{
	PartOrientationPtr partOrient(new PartOrientation(pixelSize));
	
	int lastIndex = 0;
	for (int i = 0; i < partWidth - 1; i++) {
		lastIndex = partOrient->addPointToRight(lastIndex);
	}

	lastIndex = 0;
	for (int i = 0; i < partWidth - 1; i++) {
		lastIndex = partOrient->addPointBelow(lastIndex);
	}

	PartPtr part(new Part(partOrient, is3D));
	return part;
}

PartPtr PartBuilder::buildZigzagPart(int partWidth, int pixelSize /*= 1*/, bool is3D /*= false*/)
{
	PartOrientationPtr partOrient(new PartOrientation(pixelSize));

	int lastIndex = 0;
	for (int i = 0; i < partWidth - 1; i++) {
		lastIndex = partOrient->addPointToRight(lastIndex);
	}

	lastIndex = partOrient->addPointBelow(lastIndex);
	for (int i = 0; i < partWidth - 1; i++) {
		lastIndex = partOrient->addPointToRight(lastIndex);
	}

	PartPtr part(new Part(partOrient, is3D));
	return part;
}

PartPtr PartBuilder::buildTPart(int partWidth, int pixelSize /*= 1*/, bool is3D /*= false*/)
{
	PartOrientationPtr partOrient(new PartOrientation(pixelSize));

	int lastIndex = 0;
	
	for (int i = 0; i < partWidth; i++) {
		lastIndex = partOrient->addPointToRight(lastIndex);
	}

	int middleIndex = lastIndex;

	for (int i = 0; i < partWidth; i++) {
		lastIndex = partOrient->addPointToRight(lastIndex);
	}

	lastIndex = middleIndex;

	for (int i = 0; i < partWidth; i++) {
		lastIndex = partOrient->addPointBelow(lastIndex);
	}

	PartPtr part(new Part(partOrient, is3D));
	return part;
}