#include "Part.h"

// Static variable initialization
unique_ptr<PrimeNumbersGenerator> Part::idAllocator = PrimeNumbersModule::createGenerator();

Part::Part(PartOrientationPtr partOrient)
{
	m_partOrientations = PartOrientationListPtr(new PartOrientationList());

	m_partOrientations->push_back(partOrient);

	extendPartOrientations();

	// Assign the next available id (the next prime number the generator creates).
	// The ids are assigned as prime numbers to make the hash function of solutions made of
	// combinations of multiple parts more efficient.
	m_partId = 0; //idAllocator->nextPrime();
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

	bool bla1 = partOrient->isVerticalSymmetrical(mirroredVerticalOrientation);
	bool bla2 = partOrient->isHorizonalSymmetrical(mirroredHorizonalOrientation);

	if (!partOrient->isVerticalSymmetrical(mirroredVerticalOrientation) && !partOrient->isHorizonalSymmetrical(mirroredHorizonalOrientation)) {

		m_partOrientations->push_back(mirroredVerticalOrientation);
		m_partOrientations->push_back(rotatedMirroredOrientation);
	}
}
