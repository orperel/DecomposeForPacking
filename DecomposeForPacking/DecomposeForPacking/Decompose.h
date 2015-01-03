#pragma once

#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include "DLXSolver.h"
#include "World.h"
#include "Part.h"
#include "DecomposeResult.h"

// Map from location sets to their parts (location set is a set of indices of the part location, instead of points)
typedef unordered_map<DLX_VALUES_SET, PartPtr> SetToPartMap;
typedef std::shared_ptr<SetToPartMap> SetToPartMapPtr;
// Map from location sets to their part orientations
typedef unordered_map<DLX_VALUES_SET, tuple<PartOrientationPtr, Point>> SetToOrientationMap;
typedef std::shared_ptr<SetToOrientationMap> SetToOrientationMapPtr;

using std::unordered_set;

// Represents a key for a single decompose solutions.
// The keys are used to identify duplicate decomposition solutions that use the same parts
// (they should be ommitted due to identical packing solution that will follow).
class DecomposeSolutionKey
{
public:
	DecomposeSolutionKey(PartsCountPtr solution) : m_solutionPtr(solution) {};
	const PartsCountPtr solution() const { return m_solutionPtr; }

	/* Operator== override */
	bool operator==(const DecomposeSolutionKey& other) const
	{
		PartsCountPtr otherPartsCount = other.m_solutionPtr;

		if (this->m_solutionPtr->size() != otherPartsCount->size())
			return false;

		// Compare each of the parts
		for (auto& currIter : *(this->m_solutionPtr.get()))
		{
			PartPtr currPart = currIter.first;
			int currCount = currIter.second;

			// Use lambda expression to find the part in the other list (it is encapsulated in a shared_ptr,
			// therefore we cannot use the default find function).
			auto it = std::find_if(otherPartsCount->begin(),
								   otherPartsCount->end(),
								   [&](std::pair<PartPtr, int> const& partEntryToSearch) {
										// Depend on Part's operator==
										return (*partEntryToSearch.first == *currPart &&
												partEntryToSearch.second == currCount);
									});

			// Part entry doesn't exist in other solution
			if (it == otherPartsCount->end())
			{
				return false;
			}
		}

		return true; // Solutions are identical in parts
	}

private:
	PartsCountPtr m_solutionPtr;
};

namespace std {
	template <>
	struct hash<DecomposeSolutionKey> {
	public:
		size_t operator()(const DecomposeSolutionKey& valueSet) const
		{
			return 3;
		}
	};
}


/** Manages the decomposition process, that is calling for the visitor and the DLX solver, and arranging the results
	in a DecomposeResult object. */
class Decompose
{
public:
	/** Constructs a new decompose object from a world and part list. */
	Decompose(WorldPtr world, PartListPtr partList);
	
	virtual ~Decompose();
	
	/** Implements the class purposes and returns the decomposition result. */
	std::shared_ptr<DecomposeResult> decompose();

private:
	WorldPtr m_world;	// The world to be decomposed
	PartListPtr m_partList;	// The part list participate in the decomposition
	SetToPartMapPtr m_locationSetToPart;	// Map from location sets to parts (location set is a set of indices of the part location, instead of points)
	SetToOrientationMapPtr m_locationSetToOrient;	// Map from location sets to part orientations

	// Filters decompose solutions that contain exactly the same parts (but in different locations or orientations),
	// eventually they will return the same packing results
	unordered_set<DecomposeSolutionKey> m_duplicatesFilter;
};

/* Defines a hash function for the locationSetToPart and locationSetToOrient (XOR between the indices in the set). */
namespace std {
	template <>
	struct hash<DLX_VALUES_SET> {
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