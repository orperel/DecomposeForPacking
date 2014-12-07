#include "Packing.h"
#include "PackingPartFitVisitor.h"

using std::get;

Packing::Packing(WorldPtr box, std::shared_ptr<DecomposeResult> decomposeResult) : _box(box),	
	_partsCountList(decomposeResult->getPartsCountList()), _partsCountBySize(decomposeResult->getPartsCountBySize()),
	_locationSetToPart(new SetToPartMap()),	_locationSetToOrient(new SetToOrientationMap())
{
}

Packing::~Packing()
{
}

std::shared_ptr<PackResult> Packing::pack()
{
	// Creates the result vector of the packing, of the type list of PartLocationList
	std::shared_ptr<vector<PartLocationListPtr>> packPerDecompose = std::make_shared<vector<PartLocationListPtr>>();

	for (size_t i = 0; (i < CANDIDATES_FOR_PACKING) && (i < _partsCountList->size()); i++) {
		int currDecompositionSize = std::get<0>(_partsCountBySize->at(i));
		int currDecompositionIndex = std::get<1>(_partsCountBySize->at(i));
		// Creates DLXSolver for the packing
		shared_ptr<DLXSolver> dlxSolver(new DLXSolver(_box->getNumberOfPoints(), currDecompositionSize));

		// For each part in the part list creates a new visitor of the world. Runs world.accept() on the visitor
		PartsCountPtr currPartsCount = _partsCountList->at(currDecompositionIndex);
		int currPartId = _box->getNumberOfPoints();
		for (auto& iterator = currPartsCount->begin(); iterator != currPartsCount->end(); ++iterator) {
			IWorldVisitorPtr visitor(new PackingPartFitVisitor(iterator->first, currPartId, iterator->second, dlxSolver,
				_locationSetToPart, _locationSetToOrient));
			_box->accept(visitor);
			currPartId += iterator->second;
		}

		auto solutions = dlxSolver->solve();	// Runs solver

		PartLocationListPtr partLocationList = std::make_shared<PartLocationList>();
		if (!solutions.empty()) {
			auto solution = solutions[0];
			// Computes part location list
			for each (const DLX_VALUES_SET& locationSet in solution) {
				// Pushes-back tuple of part orientation and its origin point to the vector
				partLocationList->push_back(_locationSetToOrient->at(locationSet));
			}
		}
		packPerDecompose->push_back(partLocationList);
	}
	
	PackResult packResult = PackResult(packPerDecompose);
	return std::make_shared<PackResult>(packPerDecompose);
}