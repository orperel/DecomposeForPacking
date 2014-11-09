#pragma once

#include <iostream>
#include "World.h"
#include "CImg.h"
#include "DynamicArray.h"
#include <string>
#include "Part.h"

using namespace std;
using namespace cimg_library;

const int BLACK = 255;

WorldPtr imageToWorld(std::string path) {
	CImg<unsigned char> img(path.c_str());

	//	int width = src.width();
	//	int height = src.height();
	//	int depth = src.depth();
	//	int channels = src.spectrum();
	//
	//	cout << "Size of the image: " << width << "x" << height << "\n";
	//	cout << "Image depth: " << depth << "\n"; //typically equal to 1 when considering usual 2d images
	//	cout << "Number of channels: " << channels << "\n"; //3 for RGB-coded color images
	//

	int width = img.width();
	int height = img.height();

	int min_X = -1;
	int max_X = -1;
	int min_Y = -1;
	int max_Y = -1;
	int numberOfPoints = 0;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			int pixel = (int)img.atXY(i, j);

			if (BLACK != pixel) {
				numberOfPoints++;

				if (-1 == min_X) {
					min_X = i;
				} else {
					if (i > max_X) {
						max_X = i;
					}
				}

				if (-1 == min_Y) {
					min_Y = j;
				} else {
					if (j > max_Y) {
						max_Y = j;
					}
				}
			}
		}
	}

	cout << width << " - " << height << endl;
	cout << min_X << " - " << max_X << endl << min_Y << " - " << max_Y << endl;

	BooleanDynamicArrayPtr mat(new DynamicArray<bool>(max_X - min_X + 1, max_Y - min_Y + 1));
	CImg<unsigned char> bla(max_X - min_X + 1, max_Y - min_Y + 1);


	for (int i = min_X; i <= max_X; i++) {
		for (int j = min_Y; j <= max_Y; j++) {
			int pixel = (int)img.atXY(i, j);

			(*mat)[i - min_X][j - min_Y] = (BLACK != pixel);
			(*bla.data(i - min_X, j - min_Y)) = pixel;
		}
	}

	//new CImgDisplay(bla);

	return WorldPtr(new World(mat, numberOfPoints));
}

int main(int argc, char *argv[]) {
	WorldPtr world = imageToWorld("../../tetris.bmp");

	PartPtr part(new Part());

	int lastIndex = part->addPointToRight(0);
	part->addPointToRight(lastIndex);

	cout << "Test proj" << endl;
	
	int x;

	cin >> x;
}