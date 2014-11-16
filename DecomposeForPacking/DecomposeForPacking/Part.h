#pragma once

#include <vector>
#include "PartOrientation.h"

class Part {
public:
	Part(PartOrientationPtr partOrient);
	~Part();

	PartOrientationListPtr getPartOrientations();

	PartOrientationPtr getPartOrientationByIndex(int index);

private:
	void extendPartOrientations();

	PartOrientationListPtr m_partOrientations;
};

typedef std::shared_ptr<Part> PartPtr;
typedef std::vector<PartPtr> PartList;
typedef std::shared_ptr<PartList> PartListPtr;