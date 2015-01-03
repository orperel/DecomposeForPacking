#include "Packing.h"
#include "PackingPartFitVisitor.h"
#include "Point.h"

using std::get;

/** Constructs a new packing object from a box and decomposition result.
	Extracts the parts count list and the parts count by size from this decomposition. */
Packing::Packing(WorldPtr box, std::shared_ptr<DecomposeResult> decomposeResult) : _box(box),	
	_partsCountList(decomposeResult->getPartsCountList()), _partsCountBySize(decomposeResult->getPartsCountBySize()),
	_locationSetToPart(new SetToPartMap()),	_locationSetToOrient(new SetToOrientationMap())
{
}

Packing::~Packing()
{
}

/** Implements the class purposes and returns the packing result.
 *	The results are ordered according to the decompose result, one solution for each decomposition.
 *  This solution is with the minimal bounding box, i.e. the best solution.
 */
std::shared_ptr<PackResult> Packing::pack()
{
	// Creates the result vector of the packing, of the type list of PartLocationList
	std::shared_ptr<vector<PartLocationListPtr>> packPerDecompose = std::make_shared<vector<PartLocationListPtr>>();

	for (size_t i = 0; (i < CANDIDATES_FOR_PACKING) && (i < _partsCountList->size()); i++) {
		int currDecompositionSize = std::get<0>(_partsCountBySize->at(i));
		int currDecompositionIndex = std::get<1>(_partsCountBySize->at(i));
		// Creates DLXSolver for the packing ("currDecompositionSize" are the mandatory fields)
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

		std::shared_ptr<vector<PartLocationListPtr>> listOfPartLocationLists = std::make_shared<vector<PartLocationListPtr>>();
		if (!solutions.empty()) {
			for each (const DLX_SOLUTION& solution in solutions) {
				PartLocationListPtr partLocationList = std::make_shared<PartLocationList>();
				// Computes part location list
				for each (const DLX_VALUES_SET& locationSet in solution) {
					// Pushes-back tuple of part orientation and its origin point to the vector
					partLocationList->push_back(_locationSetToOrient->at(locationSet));
				}
				listOfPartLocationLists->push_back(partLocationList);
			}

			std::shared_ptr<vector<int>> boundingBoxes = getBoundingBoxes(listOfPartLocationLists);
			int minBoundingBox = (*boundingBoxes)[0];
			int indexOfMin = 0;
			for (int i = 1; i < boundingBoxes->size(); i++) {
				if ((*boundingBoxes)[i] < minBoundingBox) {
					minBoundingBox = (*boundingBoxes)[i];
					indexOfMin = i;
				}
			}
			
			packPerDecompose->push_back((*listOfPartLocationLists)[indexOfMin]);
		}
		else {
			packPerDecompose->push_back(std::make_shared<PartLocationList>());
		}
	}
	
	PackResult packResult = PackResult(packPerDecompose);
	return std::make_shared<PackResult>(packPerDecompose);
}

/** Returns a vector of the bounding box sizes of all solutions, for one decomposition. */
std::shared_ptr<vector<int>> Packing::getBoundingBoxes(std::shared_ptr<vector<PartLocationListPtr>> listOfPartLocationLists) {
	std::shared_ptr<vector<int>> boundingBoxes = std::make_shared<vector<int>>();

	for each (const PartLocationListPtr& partLocationList in *listOfPartLocationLists) {
		int minHorizontal = std::numeric_limits<int>::max();
		int maxHorizontal = 0;
		int minVertical = std::numeric_limits<int>::max();
		int maxVertical = 0;

		for each (const tuple<PartOrientationPtr, Point>& orientOrigin in *partLocationList) {		
			for each (Point point in *(std::get<0>(orientOrigin)->getPointList())) {
				Point relatedPoint = point + (std::get<1>(orientOrigin));
				int x = relatedPoint.getX();
				int y = relatedPoint.getY();
				if (x < minHorizontal) {
					minHorizontal = x;
				}
				if (x > maxHorizontal) {
					maxHorizontal = x;
				}
				if (y < minVertical) {
					minVertical = y;
				}
				if (y > maxVertical) {
					maxVertical = y;
				}
			}
		}

		boundingBoxes->push_back((maxHorizontal - minHorizontal)*(maxVertical - minVertical));
	}

	return boundingBoxes;
}