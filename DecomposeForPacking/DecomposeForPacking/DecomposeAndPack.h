#pragma once

#include "World.h"
#include "DecomposeResult.h"
#include "PackResult.h"

#include <tuple>
#include <vector>
#include <memory>

typedef std::tuple<std::shared_ptr<std::vector<PartLocationListPtr>>, std::shared_ptr<std::vector<PartLocationListPtr>>> DecomposeAndPackResult;

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

