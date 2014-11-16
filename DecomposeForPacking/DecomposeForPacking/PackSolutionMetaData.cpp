#include "PackSolutionMetaData.h"

PackSolutionMetaData::PackSolutionMetaData(tuple<PartLocationList, DECOMPOSE_SOULTION_INDEX>& packSolution)
{

}

PackSolutionMetaData::~PackSolutionMetaData()
{

}

const int PackSolutionMetaData::packageSize() const
{

}

const int PackSolutionMetaData::numberOfParts() const
{

}

void PackSolutionMetaData::generateMetaData(tuple<PartLocationList, DECOMPOSE_SOULTION_INDEX>& packSolution)
{
	calculateBoundingBox(packSolution);
}

void PackSolutionMetaData::calculateBoundingBox(tuple<PartLocationList, DECOMPOSE_SOULTION_INDEX>& packSolution)
{
	packSolution->
}

void PackSolutionMetaData::countNumberOfParts()
{

}