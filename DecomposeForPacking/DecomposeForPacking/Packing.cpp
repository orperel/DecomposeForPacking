#include "Packing.h"
#include <vector>

Packing::Packing(WorldPtr box, vector<PartsCount> partsToPack) : _box(box), _partsToPack(partsToPack)
{

}

Packing::~Packing()
{

}

PackResult Packing::pack()
{
	PackResult results(nullptr);

	return results;
}