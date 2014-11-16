#pragma once

#include <algorithm>
#include <functional>
#include <tuple>
#include "AlgoXResult.h"
#include "PackSolutionMetaData.h"

using std::shared_ptr;

typedef int DECOMPOSE_SOULTION_INDEX; // Index of a certain decompose solution
typedef tuple<PartLocationList, PartLocationList, PartsCount> DAPSolution; // Decompose and pack solution

/**
 *	Contains multiple solution results of the packing process.
 *	Each packing solution
 */
class PackResult
{
public:
	class PackResultIterator
	{
		public:
			PackResultIterator(vector<tuple<PartLocationList, DECOMPOSE_SOULTION_INDEX>>& packedResults);
			virtual ~PackResultIterator();
			const bool hasNext();
			unique_ptr<DAPSolution> nextSolution();
		private:
			vector<tuple<PartLocationList, DECOMPOSE_SOULTION_INDEX>>::iterator _innerIterator;
			vector<tuple<PartLocationList, DECOMPOSE_SOULTION_INDEX>>::iterator _iteratorEnd;
	};

	PackResult(shared_ptr<AlgoXResult> decomposeResult);
	virtual ~PackResult();

	/** Adds a new packing solution and links it to the corresponding decomposition solution. */
	void addPackingSolution(int decomposeSolutionIndex, PartLocationList packedParts);

	/** Sorts the packing results by the given criteria. */
	void sortByCriteria(std::function <bool(const tuple<PartLocationList, DECOMPOSE_SOULTION_INDEX>&,
											const tuple<PartLocationList, DECOMPOSE_SOULTION_INDEX>&)> criteria);
	
	unique_ptr<PackResult::PackResultIterator> iterator();

private:

	/**
	 * Grades are given according to the following criteria (priority descending):
	 * 1) Solution is shaped as a box +5 points.
	 * 2) Solution with least parts gets +5 points.
	 * 4) Solution with smaller total size +3 points.
	 * 3) Solution contains holes -3 points.
	 */
	bool gradeByShapeAndNumOfParts(const tuple<PartLocationList, DECOMPOSE_SOULTION_INDEX>& solutionA,
								   const tuple<PartLocationList, DECOMPOSE_SOULTION_INDEX>& solutionB);

	/** Contains all the solutions of the decomposition process */
	shared_ptr<AlgoXResult> _decomposition;

	/** Maps the packing solution to their corresponding decompositions */
	vector<tuple<PartLocationList, DECOMPOSE_SOULTION_INDEX>> _packedPartsToDecompose;
};