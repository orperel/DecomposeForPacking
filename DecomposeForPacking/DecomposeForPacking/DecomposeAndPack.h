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

class DecomposeAndPack
{
public:
	DecomposeAndPack(WorldPtr world);
	virtual ~DecomposeAndPack();

	virtual DecomposeAndPackResult run();

private:
	shared_ptr<DecomposeResult> decompose();
	shared_ptr<PackResult> pack(shared_ptr<DecomposeResult> decomposeResult);

	WorldPtr m_world;
};

