#include "Part.h"

Part::Part(PartOrientationPtr partOrient)
{
	m_partOrientations = PartOrientationListPtr(new PartOrientationList());

	m_partOrientations->push_back(partOrient);

	extendPartOrientations();
}

Part::~Part()
{

}

PartOrientationListPtr Part::getPartOrientations()
{
	return m_partOrientations;
}

PartOrientationPtr Part::getPartOrientationByIndex(int index)
{
	return m_partOrientations->at(index);
}

void Part::extendPartOrientations()
{
	PartOrientationPtr partOrient = getPartOrientationByIndex(0);

	if (partOrient->isSymmetrical()) {
		return;
	}

	PartOrientationPtr rotatedOrientation = partOrient->rotate();
	m_partOrientations->push_back(rotatedOrientation);

	PartOrientationPtr mirroredVerticalOrientation = partOrient->verticalMirror();
	PartOrientationPtr mirroredHorizonalOrientation = partOrient->horizonalMirror();
	PartOrientationPtr rotatedMirroredOrientation = rotatedOrientation->verticalMirror();

	if (!partOrient->isVerticalSymmetrical(mirroredVerticalOrientation) && !partOrient->isHorizonalSymmetrical(mirroredHorizonalOrientation)) {

		m_partOrientations->push_back(mirroredVerticalOrientation);
		m_partOrientations->push_back(rotatedMirroredOrientation);
	}
}
