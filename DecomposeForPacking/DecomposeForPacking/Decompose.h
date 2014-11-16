#pragma once

#include <unordered_map>
#include "DLXSolver.h"
#include "World.h"
#include "Part.h"
#include "DecomposeResult.h"

typedef unordered_map<DLX_VALUES_SET, PartPtr> SetToPartMap;
typedef std::shared_ptr<SetToPartMap> SetToPartMapPtr;
typedef unordered_map<DLX_VALUES_SET, tuple<PartOrientationPtr, Point>> SetToOrientationMap;
typedef std::shared_ptr<SetToOrientationMap> SetToOrientationMapPtr;

class Decompose
{
public:
	Decompose(WorldPtr world, PartListPtr partList);
	virtual ~Decompose();

	DecomposeResult decompose();

private:
	WorldPtr m_world;
	PartListPtr m_partList;
	// Map from location sets to their parts (location set is a set of indices of the part location, instead of points)
	SetToPartMapPtr m_locationSetToPart;
	// Map from location sets to their part orientations
	SetToOrientationMapPtr m_locationSetToOrient;
};

/* Defines a hash function for the SetToPartMap and SetToOrientationMap (XOR between the indices in the set). */
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