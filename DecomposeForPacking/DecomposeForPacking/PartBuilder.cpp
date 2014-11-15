#include "PartBuilder.h"

const int STANDART_CORNER_PART_WIDTH = 2;
const int STANDART_ZIGZAG_PART_WIDTH = 2;
const int STANDART_LONG_PART_WIDTH = 3;

std::shared_ptr<CImg<unsigned char>> PartBuilder::toImage(PartPtr part)
{
	int width = 0;
	int height = 0;

	for each (const Point& point in part->getPointList()) {
		if (point.getX() > width) width = point.getX();
		if (point.getY() > height) height = point.getY();
	}

	std::shared_ptr<CImg<unsigned char>> img(new CImg<unsigned char>(width + 1, height + 1));

	for each (const Point& point in part->getPointList()) {
		(*img->data(point.getX(), point.getY())) = 0;
	}

	return img;
}

PartPtr PartBuilder::buildLongPart(int partWidth, int pixelSize /*= 1*/)
{
	PartPtr part(new Part(pixelSize));

	int lastIndex = 0;
	for (int i = 0; i < partWidth - 1; i++) {
		lastIndex = part->addPointToRight(lastIndex);
	}

	return part;
}

PartListPtr PartBuilder::buildStandartPartPack(int pixelSize /*= 1*/)
{
	PartListPtr partList(new PartList());

	//partList->push_back(buildZigzagPart(STANDART_ZIGZAG_PART_WIDTH, pixelSize));
	//partList->push_back(buildCornerPart(STANDART_CORNER_PART_WIDTH, pixelSize));
	partList->push_back(buildLongPart(1, pixelSize));
	//partList->push_back(buildLongPart(2, pixelSize));

	return partList;
}

PartPtr PartBuilder::buildCornerPart(int partWidth, int pixelSize /*= 1*/)
{
	PartPtr part(new Part(pixelSize));

	int lastIndex = 0;
	for (int i = 0; i < partWidth - 1; i++) {
		lastIndex = part->addPointToRight(lastIndex);
	}

	lastIndex = 0;
	for (int i = 0; i < partWidth - 1; i++) {
		lastIndex = part->addPointBelow(lastIndex);
	}

	return part;
}

PartPtr PartBuilder::buildZigzagPart(int partWidth, int pixelSize /*= 1*/)
{
	PartPtr part(new Part(pixelSize));

	int lastIndex = 0;
	for (int i = 0; i < partWidth - 1; i++) {
		lastIndex = part->addPointToRight(lastIndex);
	}

	lastIndex = part->addPointBelow(lastIndex);
	for (int i = 0; i < partWidth - 1; i++) {
		lastIndex = part->addPointToRight(lastIndex);
	}

	return part;
}
