#include "DLXSolver.h"

void DLXSolver::createColumnHeaders(int numOfColumns)
{
	_matrixHead = DLXColHeader::createSentinelHeader();
	shared_ptr<DLXColHeader> currNode = _matrixHead;

	// Create and link each column header to the previous one
	for (int colIndex = 0; colIndex < numOfColumns; colIndex++)
	{
		shared_ptr<DLXColHeader> newColHeader = std::make_shared<DLXColHeader>(colIndex);
		currNode->setRight(newColHeader);
		newColHeader->setLeft(currNode);
		newColHeader->setUp(newColHeader);
		newColHeader->setDown(newColHeader);
		currNode = newColHeader;

		// Add column-header to fast-access index mapping
		_colHeadersMapping[colIndex] = currNode;
	}

	// Complete cyclic linkings
	currNode->setRight(_matrixHead);
	_matrixHead->setLeft(currNode);
}

void DLXSolver::addNodeToColumn(shared_ptr<DLXDataNode> node)
{
	// Use fast-access index to fetch the approporiate column header node
	auto colHeader = _colHeadersMapping.at(node->colIndex());
	node->setHead(colHeader);
	int targetRowIndex = node->rowIndex();
	shared_ptr<DLXNode> currIterNode = colHeader->down(); // Points to the first data node in the column

	// Advance over the column's links until a link with a higher row index is encountered, or we reach the end on the column
	while ((currIterNode != colHeader) && (std::static_pointer_cast<DLXDataNode>(currIterNode)->rowIndex() < targetRowIndex))
	{
		currIterNode = currIterNode->down();
	}

	// currIterNode now points to the node that should be next to our newly inserted node
	// Attach the node before it
	auto prevNode = currIterNode->up();
	node->setUp(prevNode);
	node->setDown(currIterNode);
	prevNode->setDown(node);
	currIterNode->setUp(node);

	colHeader->incNumOfElements();
}

void DLXSolver::detachNodeFromRow(shared_ptr<DLXNode> node)
{
	node->left()->setRight(node->right());
	node->right()->setLeft(node->left());
}

void DLXSolver::reattachNodeToRow(shared_ptr<DLXNode> node)
{
	node->left()->setRight(node);
	node->right()->setLeft(node);
}

void DLXSolver::detachNodeFromCol(shared_ptr<DLXNode> node)
{
	node->up()->setDown(node->down());
	node->down()->setUp(node->up());
}

void DLXSolver::reattachNodeToCol(shared_ptr<DLXNode> node)
{
	node->up()->setDown(node);
	node->down()->setUp(node);
}

void DLXSolver::detachRow()
{

}

void DLXSolver::detachColumn()
{

}

shared_ptr<DLXSolver::DLXColHeader> DLXSolver::chooseNextColumn()
{

}

void DLXSolver::cover(shared_ptr<DLXColHeader>)
{

}

void DLXSolver::uncover(shared_ptr<DLXColHeader>)
{

}

/** Creates a new "Exact Cover Problem" solver, using "Full Cover" mode.
 *  Only solutions that cover the entire universe are outputted.
 *  The universe is 0..numberOfColumns.
 */
DLXSolver::DLXSolver(int numberOfColumns) : DLXSolver(0, _mandatoryColsNum)
{
}

/** Creates a new "Exact Cover Problem" solver, using "Partial Cover" mode.
*  Only solutions that cover the mandatory columns are outputted.
*  Note when adding rows - which value indices fall under the mandatory columns and which
*  under the optional ones.
*  The uninverse to cover is indices [numberOfOptionalCols..numberOfOptionalCols + numberOfMandatoryCols - 1].
*/
DLXSolver::DLXSolver(int numberOfOptionalCols, int numberOfMandatoryCols):
_mandatoryColsNum(numberOfMandatoryCols),
_optionalColsNum(numberOfOptionalCols),
_rowNum(0)
{
	createColumnHeaders(numberOfOptionalCols + numberOfMandatoryCols);
}

/** Dtor to release resources allocated by the solver.. */
DLXSolver::~DLXSolver()
{
	// Resources automatically released by shared pointers
}

/** Add a new set of values to test against as part of the solution.
*  Values are indexed from 0 to N the following way:
*  Full cover mode -
*  [0 .. numberOfColumns - 1]
*  Partial cover mode - (optional columns indexed first, mandatory columns following immediatly afterwards) -
*  [0 .. numberOfOptionalColumns - 1, numberOfOptionalColumns, numberOfOptionalColumns + 1 .. numberOfOptionalColumns + numberOfMandatoryColumns]
*/
void DLXSolver::addRow(DLX_VALUES_SET row)
{
	if (row.empty())
		return;

	DLX_VALUES_SET_ITERATOR iter = row.begin();
	int firstValue = *iter;
	shared_ptr<DLXNode> firstNode = std::make_shared<DLXDataNode>(_rowNum, firstValue); // rowIndex = _rowNum, _colIndex = firstValue
	iter++;
	auto prevNode = firstNode;

	// Parse each value in the row's value set and create a node out of it.
	// Nodes are placed immediately in the sparse matrix, attaching them to adjacent row and column links.
	for (; iter != row.end(); iter++)
	{
		int value = *iter;
		auto dataNode = std::make_shared<DLXDataNode>(_rowNum, value); // rowIndex = _rowNum, _colIndex = value
		prevNode->setRight(dataNode);
		dataNode->setLeft(prevNode);
		addNodeToColumn(dataNode);
		prevNode = dataNode;
	}

	// Maintain cyclic pointers on row links
	prevNode->setRight(firstNode);
	firstNode->setLeft(prevNode);

	_rowNum++;
}

/** Solves the cover problem and returns the possible solutions found. */
vector<DLX_SOLUTION> DLXSolver::solve()
{
	return vector<DLX_SOLUTION>();
}