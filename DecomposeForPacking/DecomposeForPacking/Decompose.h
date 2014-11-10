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
	Decompose(WorldPtr world, PartList partList);
	virtual ~Decompose();

	void decompose();

private:
	WorldPtr m_world;
	PartList m_partList;
	SetsToPartMapPtr m_locationSetToPart;
};

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