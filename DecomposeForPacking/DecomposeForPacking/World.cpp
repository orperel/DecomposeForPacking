#include "World.h"
#include "CImg.h"
#include <iostream>

using namespace cimg_library;
using namespace std;

#define cimg_use_png


World::World()
{
	CImg<unsigned char> src("C:/Users/Amit/Desktop/tetris.bmp");

	int width = src.width();
	int height = src.height();
	int depth = src.depth();
	int channels = src.spectrum();

	cout << "Size of the image: " << width << "x" << height << "\n";
	cout << "Image depth: " << depth << "\n"; //typically equal to 1 when considering usual 2d images
	cout << "Number of channels: " << channels << "\n"; //3 for RGB-coded color images

	int bla =  src.front();

	for each (int pixel in src)
	{
		cout << pixel << " ";
	}

	
}


World::~World()
{
}
