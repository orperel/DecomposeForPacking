#pragma once

#include <vector>
#include "PartOrientation.h"

class Part {
public:
	Part(PartOrientationPtr partOrientation);
	~Part();

	PartOrientationListPtr getPartOrientations();

	PartOrientationPtr getPartOrientationByIndex(int index);

private:
	void extendPartOrientations();
	void extendVerticalOreintation();
	void extendHorizonalOrientation();

	PartOrientationListPtr m_partOrientations;
};

typedef std::shared_ptr<Part> PartPtr;
typedef std::vector<PartPtr> PartList;
typedef std::shared_ptr<PartList> PartListPtr;