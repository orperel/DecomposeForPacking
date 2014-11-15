#include "Part.h"

Part::Part(PartOrientationPtr partOrientation)
{
	m_partOrientations = PartOrientationListPtr(new PartOrientationList());
	m_partOrientations->push_back(partOrientation);

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
	extendHorizonalOrientation();
	extendVerticalOreintation();
}

void Part::extendVerticalOreintation()
{
	PartOrientationPtr partOrient = getPartOrientationByIndex(0);
	PartOrientationPtr newPartOrient(new PartOrientation());

	//for each (const Point& point in partOrient->getPointList()) {
	//	//newPartOrient->
	//}
}

void Part::extendHorizonalOrientation()
{

}
