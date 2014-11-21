#include "WorldBuilder.h"
#include "World.h"
#include <iostream>

using namespace std;

const int WHITE = 255;

WorldPtr WorldBuilder::fromImage(shared_ptr<CImg<int>> img, int ratio /*= 1*/)
{
	WorldPosition worldPosition = getWorldPosition(img);

	PointListPtr pointList(new PointList());

	for (int i = worldPosition.min_X; i <= worldPosition.max_X; i+=ratio) {
		for (int j = worldPosition.min_Y; j <= worldPosition.max_Y; j+=ratio) {
			if (isSquareNotEmpty(img, Point(i, j), ratio)) {
				(*pointList).push_back(Point((i - worldPosition.min_X) / ratio, (j - worldPosition.min_Y) / ratio));
			}
		}
	}

	return WorldPtr(new World(pointList, worldPosition.max_X - worldPosition.min_X + 1, worldPosition.max_Y - worldPosition.min_Y + 1));
}

shared_ptr<CImg<unsigned char>> WorldBuilder::toImage(WorldPtr world)
{
	shared_ptr<CImg<unsigned char>> img(new CImg<unsigned char>(world->getWidth(), world->getHeight(), 1, 3));

	for each (const Point& point in *world->getPointList()) {
		(*img)(point.getX(), point.getY()) = point.getColor();
	}

	return img;

}

WorldPosition WorldBuilder::getWorldPosition(std::shared_ptr<CImg<int>> img)
{
	int width = img->width();
	int height = img->height();

	//int depth = img.depth();
	//int channels = img.spectrum();

	//cout << "Size of the image: " << width << "x" << height << "\n";
	//cout << "Image depth: " << depth << "\n"; //typically equal to 1 when considering usual 2d images
	//cout << "Number of channels: " << channels << "\n"; //3 for RGB-coded color images

	WorldPosition wp;

	wp.min_X = -1;
	wp.max_X = -1;
	wp.min_Y = -1;
	wp.max_Y = -1;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			int pixel = (int)img->atXY(i, j);

			if (WHITE != pixel) {
				if (-1 == wp.min_X) {
					wp.min_X = i;
				}
				if (-1 == wp.min_Y) {
					wp.min_Y = j;
				}

				if (i < wp.min_X) {
					wp.min_X = i;
				}
				if (j < wp.min_Y) {
					wp.min_Y = j;
				}

				if (i > wp.max_X) {
					wp.max_X = i;
				}
				if (j > wp.max_Y) {
					wp.max_Y = j;
				}
			}
		}
	}

	return wp;
}

bool WorldBuilder::isSquareNotEmpty(std::shared_ptr<CImg<int>> img, Point point, int size)
{
	for (int i = point.getX(); i < point.getX() + size; i++) {
		for (int j = point.getY(); j < point.getY() + size; j++) {
			int pixel = (int)img->atXY(i, j);

			if (WHITE != pixel) {
				return true;
			}
		}
	}

	return false;
}
