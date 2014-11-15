#include "Part.h"

Part::Part(PartOrientationPtr partOrientation)
{
	m_partOrientations = PartOrientationListPtr(new PartOrientationList());
	m_partOrientations->push_back(partOrientation);
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

}
