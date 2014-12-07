#pragma once

#include <tuple>
#include <functional>
#include "DecomposeResult.h"
#include "PackSolutionMetaData.h"

typedef tuple<PartLocationListPtr, PartLocationListPtr, PartsCountPtr> DAPSolution;	// Decompose and pack solution
typedef tuple<PartLocationListPtr, DECOMPOSE_SOULTION_INDEX> PackToDecomposeTuple;

/**	Contains multiple solution results of the packing process. */
class PackResult
{
public:

	class PackResultIterator
	{
		public:
			PackResultIterator(PackResult& packResult);
			virtual ~PackResultIterator();
			const bool hasNext();
			unique_ptr<DAPSolution> nextSolution();

		private:
			PackResult& _packResult;
			vector<PackToDecomposeTuple>::iterator _innerIterator;
			vector<PackToDecomposeTuple>::iterator _iteratorEnd;
	};


	PackResult(std::shared_ptr<vector<PartLocationListPtr>> packPerDecompose);
	virtual ~PackResult();

	std::shared_ptr<vector<PartLocationListPtr>> getPackPerDecomposeList();

	
	/** Adds a new packing solution and links it to the corresponding decomposition solution. */
	void addPackingSolution(int decomposeSolutionIndex, PartLocationListPtr packedParts);

	/** Sorts the packing results by the given criteria. */
	void sortByCriteria(std::function <bool(const PackToDecomposeTuple&, const PackToDecomposeTuple&)> criteria);
	
	unique_ptr<PackResult::PackResultIterator> iterator();

private:
	std::shared_ptr<vector<PartLocationListPtr>> _packPerDecompose;
	
	
	bool gradeByShapeAndNumOfParts(const PackToDecomposeTuple& solutionA, const PackToDecomposeTuple& solutionB);

	/** Contains all the solutions of the decomposition process */
	shared_ptr<DecomposeResult> _decomposition;

	/** Maps the packing solution to their corresponding decompositions */
	vector<PackToDecomposeTuple> _packedPartsToDecompose;
};