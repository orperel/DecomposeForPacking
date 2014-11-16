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

	if (!partOrient->isVerticalSymmetrical() && !partOrient->isHorizonalSymmetrical()) {
		PartOrientationPtr mirroredOrientation = partOrient->verticalMirror();

		PartOrientationPtr rotatedMirroredOrientation = rotatedOrientation->verticalMirror();

		m_partOrientations->push_back(mirroredOrientation);
		m_partOrientations->push_back(rotatedMirroredOrientation);
	}
}
