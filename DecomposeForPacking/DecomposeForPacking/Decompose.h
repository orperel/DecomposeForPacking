#pragma once

#include <unordered_map>
#include "World.h"
#include "Part.h"
#include "DLXSolver.h"

typedef unordered_map<DLX_VALUES_SET, PartPtr> SetsToPartMap;
typedef std::shared_ptr<SetsToPartMap> SetsToPartMapPtr;

class Decompose
{
public:
	Decompose(WorldPtr world, PartListPtr partList);
	virtual ~Decompose();

	void decompose();
	
	//To remove
	void printSolutions(std::vector<DLX_SOLUTION> solutions);
private:
	WorldPtr m_world;
	PartListPtr m_partList;
	// Map from location sets to their part (location set is a set of indices of the part location, instead of points)
	SetsToPartMapPtr m_locationSetToPart;
};

/* Defines a hash function for the SetsToPartMap (XOR between the indices in the set). */
namespace std {
	template <>
	class hash<DLX_VALUES_SET> {
	public:
		size_t operator()(const DLX_VALUES_SET& valueSet) const
		{
			int result = 0;
			for each (const int index in valueSet) {
				result = result ^ index;
			}
			return result;
		}
	};
}