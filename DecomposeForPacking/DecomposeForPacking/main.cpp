#pragma once

#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {

	cout << "Test proj" << endl;
	
	int* arr = new int[5];
	arr[2] = 5;
	cout << arr[2];
	delete[] arr;
	
	int x;

	cin >> x;
}