#pragma once

#include <algorithm>
#include <functional>
#include <tuple>
#include <memory>
#include "DecomposeResult.h"
#include "PackSolutionMetaData.h"

using std::shared_ptr;

typedef int DECOMPOSE_SOULTION_INDEX; // Index of a certain decompose solution
typedef tuple<PartLocationListPtr, PartLocationListPtr, PartsCountPtr> DAPSolution; // Decompose and pack solution
typedef tuple<PartLocationListPtr, DECOMPOSE_SOULTION_INDEX> PackToDecomposeTuple;

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
			PackResultIterator(PackResult& packedResults);
			virtual ~PackResultIterator();
			const bool hasNext();
			unique_ptr<DAPSolution> nextSolution();
		private:
			PackResult& _packedResults;
			vector<PackToDecomposeTuple>::iterator _innerIterator;
			vector<PackToDecomposeTuple>::iterator _iteratorEnd;
	};

	PackResult(shared_ptr<DecomposeResult> decomposeResult);
	virtual ~PackResult();

	/** Adds a new packing solution and links it to the corresponding decomposition solution. */
	void addPackingSolution(int decomposeSolutionIndex, PartLocationListPtr packedParts);

	/** Sorts the packing results by the given criteria. */
	void sortByCriteria(std::function <bool(const PackToDecomposeTuple&, const PackToDecomposeTuple&)> criteria);
	
	unique_ptr<PackResult::PackResultIterator> iterator();

private:

	/**
	 * Grades are given according to the following criteria (priority descending):
	 * 1) Solution is shaped as a box +5 points.
	 * 2) Solution with least parts gets +5 points.
	 * 4) Solution with smaller total size +3 points.
	 * 3) Solution contains holes -3 points.
	 */
	bool gradeByShapeAndNumOfParts(const PackToDecomposeTuple& solutionA, const PackToDecomposeTuple& solutionB);

	/** Contains all the solutions of the decomposition process */
	shared_ptr<DecomposeResult> _decomposition;

	/** Maps the packing solution to their corresponding decompositions */
	vector<PackToDecomposeTuple> _packedPartsToDecompose;
};