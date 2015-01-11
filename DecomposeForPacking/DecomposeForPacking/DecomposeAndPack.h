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
												shared_ptr<DecomposeResult> totalDecomposeResults);

	static bool sortIntDesc(int i, int j) { return (i>j); }

	// Member of the world
	WorldPtr m_world;
};

