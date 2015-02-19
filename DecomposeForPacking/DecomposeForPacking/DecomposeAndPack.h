#pragma once

#include "World.h"
#include "DecomposeResult.h"
#include "PackResult.h"

#include <tuple>
#include <vector>
#include <memory>

typedef std::shared_ptr<std::vector<PartLocationListPtr>> FinalDecomposeResults;
typedef std::shared_ptr<std::vector<PartLocationListPtr>> FinalPackResults;
typedef std::tuple<FinalDecomposeResults, FinalPackResults> DecomposeAndPackResult;

typedef tuple<float, int> GradeIndex;	// Tuple of solution grade and its index in the results vector

/* 
	Decompose a world and than pack it in the smallest box it can.
*/
class DecomposeAndPack
{
public:
	/*
	 *	C'tor - Gets a pointer to a world 
	 */
	DecomposeAndPack(WorldPtr world);

	/*
	*	Default D'tor
	*/
	virtual ~DecomposeAndPack();

	/*
	*	Runs the DecomposeAndPack logic.
	*	returns DecomposeAndPackResult - tuple of two std::shared_ptr<std::vector<PartLocationListPtr>>
	*/
	virtual DecomposeAndPackResult run();

	/* Returns indices of the results vector ordered by grade.
	*  The grade is composed of 60% bounding box and 40% number of parts, relatively to the minimal values. Meaning that the
	*  solution with the minimal bounding box will get 60 points for it, and the solution with the minimal number of parts will
	*  get 40 points. The percentages are configurable (in DFPConfiguration).
	*/
	std::shared_ptr<vector<GradeIndex>> getResultsByGrade();

private:
	/*
	 *	Runs the decompose logic.
	 *	returns DecomposeResult
	 */
	shared_ptr<DecomposeResult> decompose();

	/*
	*	Runs the pack logic.
	*	returns PackResult
	*/
	shared_ptr<PackResult> pack(shared_ptr<DecomposeResult> decomposeResult);

	shared_ptr<DecomposeResult> extendDecompose(WorldPtr world, PartListPtr partList,
												PartsCountPtr partsCount, PartLocationListPtr partLocationList,
												shared_ptr<DecomposeResult> totalDecomposeResults, bool isPartial);

	static bool sortIntDesc(int i, int j) { return (i>j); }

	// Member of the world
	WorldPtr m_world;

	std::shared_ptr<vector<int>> m_resultsNumOfParts;	// The number of parts of each decompose and pack result
	std::shared_ptr<vector<int>> m_resultsBoundingBox;	// The bounding box size of each packing result
};

