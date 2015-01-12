#pragma once

#include <memory>
#include <iostream>
#include <vector>
#include <tuple>

using std::vector;
using std::tuple;
using std::cout;
using std::endl;

void printVectorOfInt(std::shared_ptr<vector<int>> vec) {
	cout << "[";
	for (unsigned i = 0; i < vec->size(); i++) {
		cout << (*vec)[i];
		if (i != (vec->size() - 1)) { cout << ", "; }
	}
	cout << "]" << endl;
}

void printVectorOfTuples(std::shared_ptr<vector<tuple<float, int>>> vec) {
	cout << "[";
	for (unsigned i = 0; i < vec->size(); i++) {
		float first = std::get<0>((*vec)[i]);
		int second = std::get<1>((*vec)[i]);
		cout << "<" << first << "," << second;
		if (i != (vec->size() - 1)) { cout << ">, "; }
		else { cout << ">"; }
	}
	cout << "]" << endl;
}