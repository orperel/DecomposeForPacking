#pragma once

#include "World.h"
#include "DecomposeResult.h"

class SameDecompose
{
public:
	SameDecompose(WorldPtr world1, WorldPtr world2);

	virtual ~SameDecompose();

	virtual std::shared_ptr<vector<PartLocationListPtr>> run();

private:
	WorldPtr m_world1;
	WorldPtr m_world2;
};

