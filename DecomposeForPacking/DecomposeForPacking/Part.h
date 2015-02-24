#pragma once

#include <vector>
#include "PartOrientation.h"
#include "PrimeNumbersModule.h"

class Part {
public:
	Part(PartOrientationPtr partOrient, bool is3D = false);
	~Part();

	PartOrientationListPtr getPartOrientations();

	PartOrientationPtr getPartOrientationByIndex(int index);

	const int id() const { return m_partId; }

	/* Operator== override */
	bool operator==(const Part& other) const
	{
		return this->m_partId == other.m_partId;
	}

private:
	void extendPartOrientations(bool is3D = false);

	void addUniquePartOrientation(PartOrientationPtr partOrient);

	PartOrientationListPtr m_partOrientations;
	int m_partId;

	// Assigns a unique prime number sequentive id to each created part.
	static unique_ptr<PrimeNumbersGenerator> idAllocator;
};

typedef std::shared_ptr<Part> PartPtr;
typedef std::vector<PartPtr> PartList;
typedef std::shared_ptr<PartList> PartListPtr;