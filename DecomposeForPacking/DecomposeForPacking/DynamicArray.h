#pragma once

#include <vector>
#include <memory>

using namespace std;

template <typename T>
class DynamicArray
{
public:
	DynamicArray(){};

	DynamicArray(int rows, int cols) : dArray(rows, vector<T>(cols)), m_rows(rows), m_cols(cols) {}

	vector<T> & operator[](int i) {
		return dArray[i];
	}

	const vector<T> & operator[] (int i) const {
		return dArray[i];
	}

	void resize(int rows, int cols) {
		dArray.resize(rows);
		for (int i = 0; i < rows; ++i) dArray[i].resize(cols);
	}

	int getRows() {
		return m_rows;
	}

	int getCols() {
		return m_cols;
	}
private:
	vector<vector<T> > dArray;

	int m_rows;
	int m_cols;
};

typedef std::shared_ptr<DynamicArray<bool>> BooleanDynamicArrayPtr;