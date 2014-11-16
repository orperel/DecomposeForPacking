#include "Packing.h"
#include <vector>

Packing::Packing(WorldPtr box, vector<PartsCount> partsToPack)
{

}

Packing::~Packing()
{

}

AlgoXResult Packing::pack()
{
	vector<DLX_SOLUTION> results;
	return AlgoXResult(NULL, results);
}