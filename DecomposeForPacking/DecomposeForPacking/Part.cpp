#include "Part.h"
#include <set>

// Static variable initialization
unique_ptr<PrimeNumbersGenerator> Part::idAllocator = NULL;

Part::Part(PartOrientationPtr partOrient, bool is3D /*= false*/)
{
	// If the id allocator is accessed for the first time, create it here.
	// The reason this initialization is done here and not at the static variable initialization line
	// is the generator creation involves accessing external files, and doing that during the static
	// initializtion stage (happens before the main() runs) causes uninitialized variables bugs.
	if (idAllocator == NULL)
	{
		idAllocator = PrimeNumbersModule::createGenerator();
	}

	// Assign the next available id (the next prime number the generator creates).
	// The ids are assigned as prime numbers to make the hash function of solutions made of
	// combinations of multiple parts more efficient.
	m_partId = idAllocator->nextPrime();

	m_partOrientations = PartOrientationListPtr(new PartOrientationList());
	partOrient->setPartId(m_partId);
	m_partOrientations->push_back(partOrient);

	extendPartOrientations(is3D);
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

void Part::extendPartOrientations(bool is3D /*= false*/)
{
	// XY rotations
	PartOrientationPtr partOrient = getPartOrientationByIndex(0);
	PartOrientationPtr rotation1 = partOrient->XYRotate();
	PartOrientationPtr rotation2 = rotation1->XYRotate();
	PartOrientationPtr rotation3 = rotation2->XYRotate();

	addUniquePartOrientation(rotation1);
	addUniquePartOrientation(rotation2);
	addUniquePartOrientation(rotation3);

	if (is3D) {
		// Mirror Y Axis to Z Axis and do XZ rotations
		PartOrientationPtr mirrored = partOrient->YZMirror();
		rotation1 = mirrored->ZXRotate();
		rotation2 = rotation1->ZXRotate();
		rotation3 = rotation2->ZXRotate();

		addUniquePartOrientation(mirrored);
		addUniquePartOrientation(rotation1);
		addUniquePartOrientation(rotation2);
		addUniquePartOrientation(rotation3);

		// Mirror X Axis to Z Axis and do YZ rotations
		mirrored = partOrient->XZMirror();
		rotation1 = mirrored->ZYRotate();
		rotation2 = rotation1->ZYRotate();
		rotation3 = rotation2->ZYRotate();

		addUniquePartOrientation(mirrored);
		addUniquePartOrientation(rotation1);
		addUniquePartOrientation(rotation2);
		addUniquePartOrientation(rotation3);
	}
}

void Part::addUniquePartOrientation(PartOrientationPtr partOrient)
{
	for each(const PartOrientationPtr partOrientItem in *m_partOrientations) {
		if (!(*partOrient != *partOrientItem)) {
			return;
		}
	}

	partOrient->setPartId(m_partId);
	m_partOrientations->push_back(partOrient);
}
