#pragma once

#include <iostream>
#include "World.h"
#include "CImg.h"

using namespace std;
using namespace cimg_library;

//#define cimg_use_png

WorldPtr imageToWorld() {
	CImg<unsigned char> src("../../tetris.bmp");

	int width = src.width();
	int height = src.height();
	int depth = src.depth();
	int channels = src.spectrum();

	cout << "Size of the image: " << width << "x" << height << "\n";
	cout << "Image depth: " << depth << "\n"; //typically equal to 1 when considering usual 2d images
	cout << "Number of channels: " << channels << "\n"; //3 for RGB-coded color images

	int bla =  src.front();

	//for each (int pixel in src)
	//{
	//	cout << pixel << " ";
	//}

	new CImgDisplay(src);

	return WorldPtr(new World());
}

int main(int argc, char *argv[]) {

	imageToWorld();

	cout << "Test proj" << endl;
	
	int* arr = new int[5];
	arr[2] = 5;
	cout << arr[2];
	delete[] arr;
	
	int x;

	cin >> x;
}