#include "PackSolutionMetaData.h"

PackSolutionMetaData::PackSolutionMetaData(tuple<PartLocationList, DECOMPOSE_SOULTION_INDEX>& packSolution)
{

}

PackSolutionMetaData::~PackSolutionMetaData()
{

}

const int PackSolutionMetaData::packageSize() const
{
	return 0; // TODO
}

const int PackSolutionMetaData::numberOfParts() const
{
	return 0; // TODO
}

void PackSolutionMetaData::generateMetaData(tuple<PartLocationList, DECOMPOSE_SOULTION_INDEX>& packSolution)
{
	calculateBoundingBox(packSolution);
}

void PackSolutionMetaData::calculateBoundingBox(tuple<PartLocationList, DECOMPOSE_SOULTION_INDEX>& packSolution)
{
	
}

void PackSolutionMetaData::countNumberOfParts()
{

}