#pragma once

#include <algorithm>
#include "Part.h"
#include "DecomposeResult.h"

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

	/* The hash code for each decomposition solution is made of a multiplication of each part id
	 * (which is a prime number) and its number of occurrences.
	 */
	template <>
	struct hash<DecomposeSolutionKey> {
	public:
		size_t operator()(const DecomposeSolutionKey& valueSet) const
		{
			size_t hashKey = 0;
			
			// Multiply the prime number id of each part orientation and the number of times it appears
			// in the solution
			// i.e: Part I - id 2, Part II - id 3
			// Solution I - hash code of 2*2*3
			// Solution II - hash code of 2*3*3
			for (auto& currIter : *(valueSet.solution().get()))
			{
				PartPtr currPart = currIter.first;
				int currCount = currIter.second;

				hashKey += currPart->id() * currCount;
			}
			
			return hashKey;
		}
	};
}
