#include "DecomposeAndPack.h"

#include "CImg.h"
#include "DisplayHelper.h"
#include "PartBuilder.h"
#include "Decompose.h"
#include "PackResult.h"
#include "WorldBuilder.h"
#include "Packing.h"
#include "StringUtils.h"

#include <algorithm>

using namespace std;

const int DECOMPOSE_NUMBER_OF_ITERATIONS = 3;

DecomposeAndPack::DecomposeAndPack(WorldPtr world) : m_world(world), m_resultsNumOfParts(std::make_shared<vector<int>>()),
	m_resultsBoundingBox(std::make_shared<vector<int>>())
{
}


DecomposeAndPack::~DecomposeAndPack()
{
}

DecomposeAndPackResult DecomposeAndPack::run()
{
	shared_ptr<DecomposeResult> decomposeResult = decompose();
	shared_ptr<PackResult> packResult = pack(decomposeResult);

	FinalDecomposeResults decomposePartLocationLists = decomposeResult->getListOfPartLocationLists();
	FinalPackResults packPartLocationLists = packResult->getPackPerDecomposeList();

	std::shared_ptr<vector<GradeIndex>> resultsByGrade = getResultsByGrade();
	// Printings
	//cout << "Bounding box:" << endl;
	//printVectorOfInt(m_resultsBoundingBox);
	//cout << endl << "Number of parts:" << endl;
	//printVectorOfInt(m_resultsNumOfParts);
	//cout << endl << "Grades:" << endl;
	//printVectorOfTuples(resultsByGrade);
	//cout << endl;

	FinalDecomposeResults finalDecomposeResult = std::make_shared<vector<PartLocationListPtr>>();
	FinalPackResults finalPackResult = std::make_shared<vector<PartLocationListPtr>>();
	for each (const GradeIndex& gradeIndex in *resultsByGrade) {
		int index = std::get<1>(gradeIndex);
		finalDecomposeResult->push_back((*decomposePartLocationLists)[index]);
		finalPackResult->push_back((*packPartLocationLists)[index]);
	}

	return std::make_tuple(finalDecomposeResult, finalPackResult);
}

shared_ptr<DecomposeResult> DecomposeAndPack::extendDecompose(WorldPtr world, PartListPtr partList,
	PartsCountPtr partsCount, PartLocationListPtr partLocationList, shared_ptr<DecomposeResult> totalDecomposeResults) {

	PointListPtr newPointList(new PointList(*world->getPointList()));
	std::vector<int> pointIndexesToDelete;

	for each (const tuple<PartOrientationPtr, Point>& partOrientTuple in *partLocationList) { // For every part
		PartOrientationPtr partOrient = get<0>(partOrientTuple);
		Point basePoint = get<1>(partOrientTuple);

		for each (const Point& pointOffset in *partOrient->getPointList()) { // For each point of the part
			Point point(pointOffset.getX() + basePoint.getX(), pointOffset.getY() + basePoint.getY(), 0, BLACK);
			pointIndexesToDelete.push_back(world->getIndexFromPoint(point));
		}
	}

	std::sort(pointIndexesToDelete.begin(), pointIndexesToDelete.end(), sortIntDesc);

	for each (const int index in pointIndexesToDelete) {
		newPointList->erase(newPointList->begin() + index);
	}

	WorldPtr newWorld(new World(newPointList, world->getWidth(), world->getHeight(), world->getDepth(), world->getPixelResolution()));

	// The new world is empty, an exact decomposition was found.
	if ((newWorld->getHeight() == 0) || (newWorld->getWidth() == 0))
	{
		return NULL;
	}

	// Else continue decomposing the remaining blocks in the world
	Decompose decomposer(newWorld, partList);
	shared_ptr<DecomposeResult> decomposeResult = decomposer.decompose();
	decomposeResult->extend(partsCount, partLocationList);

	totalDecomposeResults->add(decomposeResult);

	return decomposeResult;
}

shared_ptr<DecomposeResult> DecomposeAndPack::decompose()
{
	cout << "Starting decomposing..." << endl;

	// Code for big parts solution, we are not using this code right now, so it's commented out
	//int numberOfIteration = 1;
	//float partSizePercentOfWorld = 0;

	//int minEdge = round(min(m_world->getWidth(), m_world->getHeight()) * partSizePercentOfWorld / 100);

	//shared_ptr<DecomposeResult> decomposeResult;

	//for (int i = 0; i < numberOfIteration; i++) {
	//	int partSize = minEdge - round(minEdge*i / numberOfIteration);

	//	if (0 == partSize) {
	//		partSize = 1;
	//	}

	//	shared_ptr<DecomposeResult> newDecomposeResult(new DecomposeResult());

	//	PartListPtr partList = PartBuilder::buildStandartPartPack(partSize);

	//	if (NULL == decomposeResult) {
	//		Decompose decomposer(m_world, partList);
	//		decomposeResult = decomposer.decompose();
	//	}
	//	else {
	//		for (int j = 0; j < decomposeResult->getListOfPartLocationLists()->size(); j++) {
	//			shared_ptr<DecomposeResult> nextDecomposition = extendDecompose(m_world, partList,
	//				(*decomposeResult->getPartsCountList())[j],
	//				(*decomposeResult->getListOfPartLocationLists())[j],
	//				newDecomposeResult);

	//			// Exact decomposition was found, quit
	//			if (nextDecomposition == NULL)
	//				break;
	//		}

	//		decomposeResult = newDecomposeResult;
	//	}

	//	if (1 == partSize) {
	//		break;
	//	}
	//}

	
	// Regular decomposing without the big parts solution, should be deleted after adding big parts
	PartListPtr partList = PartBuilder::buildStandartPartPack();
	Decompose decomposer(m_world, partList);
	shared_ptr<DecomposeResult> decomposeResult = decomposer.decompose();

	cout << "Finished decomposing..." << endl;

	return decomposeResult;
}

shared_ptr<PackResult> DecomposeAndPack::pack(shared_ptr<DecomposeResult> decomposeResult)
{
	int width, height;

	shared_ptr<PackResult> packResult;

	cout << "Starting packing..." << endl;

	do {
		width = ceil(sqrt(m_world->getNumberOfPoints()));
		height = width;
		WorldPtr box = WorldBuilder::buildBox(width, height);
		Packing packer(box, decomposeResult);
		packResult = packer.pack();

		if (packResult->hasSolution()) {
			cout << "Found Solution!!!" << endl;
			m_resultsNumOfParts = packer.publicSolutionsNumOfParts();
			m_resultsBoundingBox = packer.getResultsBoundingBox();
			break;
		}

		width = ceil(width + 0.1*width);
		height = width;
	} while (m_world->getWidth() >= width && m_world->getHeight() >= height);

	cout << "Finished packing..." << endl;

	return packResult;
}

/** A lambda expression implements the grade criteria for sorting the results vector. */
bool wayToSort(const GradeIndex& x, const GradeIndex& y) { return (std::get<0>(x) > std::get<0>(y)); }

/** Returns indices of the results vector ordered by grade.
 *  Grade: ....
 */
std::shared_ptr<vector<GradeIndex>> DecomposeAndPack::getResultsByGrade()
{
	std::shared_ptr<vector<GradeIndex>> resultsByGrade = std::make_shared<vector<GradeIndex>>();

	// Computes the minimal bounding box of all results
	int minBoundingBox = (*m_resultsBoundingBox)[0];
	for (int i = 1; i < m_resultsBoundingBox->size(); i++) {
		if ((*m_resultsBoundingBox)[i] < minBoundingBox) {
			minBoundingBox = (*m_resultsBoundingBox)[i];
		}
	}

	// Computes the minimal number of parts of all results
	int minNumberOfParts = (*m_resultsNumOfParts)[0];
	for (int i = 1; i < m_resultsNumOfParts->size(); i++) {
		if ((*m_resultsNumOfParts)[i] < minNumberOfParts) {
			minNumberOfParts = (*m_resultsNumOfParts)[i];
		}
	}

	// Computes grades
	for (int index = 0; index < m_resultsBoundingBox->size(); index++) {
		float boundingBoxGrade = ((1.0f*minBoundingBox) / (1.0f*((*m_resultsBoundingBox)[index]))) * BOUNDING_BOX_WEIGHT;
		float numOfPartsGrade = ((1.0f*minNumberOfParts) / (1.0f*((*m_resultsNumOfParts)[index]))) * NUM_OF_PARTS_WEIGHT;
		resultsByGrade->push_back(GradeIndex((boundingBoxGrade + numOfPartsGrade), index));
	}

	std::sort(resultsByGrade->begin(), resultsByGrade->end(), wayToSort);	// Sorts by grade

	return resultsByGrade;
}