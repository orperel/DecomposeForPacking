#pragma once

#include <iostream>
#include "World.h"

using namespace std;

int main(int argc, char *argv[]) {

	World world;

	cout << "Test proj" << endl;
	
	int* arr = new int[5];
	arr[2] = 5;
	cout << arr[2];
	delete[] arr;
	
	int x;

	cin >> x;
}