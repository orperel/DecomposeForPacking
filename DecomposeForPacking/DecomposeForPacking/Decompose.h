#pragma once

#include <unordered_map>
#include "World.h"
#include "Part.h"
#include "DLXSolver.h"

typedef unordered_map<DLX_VALUES_SET, PartOrientationPtr> SetsToOrientationMap;
typedef std::shared_ptr<SetsToOrientationMap> SetsToOrientationMapPtr;

class Decompose
{
public:
	Decompose(WorldPtr world, PartListPtr partList);
	virtual ~Decompose();

	vector<DLX_SOLUTION> decompose();

private:
	WorldPtr m_world;
	PartListPtr m_partList;
	// Map from location sets to their part orientation (location set is a set of indices of the part location, instead of points)
	SetsToOrientationMapPtr m_locationSetToOrient;
};

/* Defines a hash function for the SetsToOrientationMap (XOR between the indices in the set). */
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