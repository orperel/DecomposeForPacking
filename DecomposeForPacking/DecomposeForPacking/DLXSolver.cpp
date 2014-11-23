#include "DLXSolver.h"
#include <iostream>

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

void DLXSolver::detachNodeFromCol(shared_ptr<DLXDataNode> node)
{
	node->up()->setDown(node->down());
	node->down()->setUp(node->up());

	node->head()->decNumOfElements();
}

void DLXSolver::reattachNodeToCol(shared_ptr<DLXDataNode> node)
{
	node->up()->setDown(node);
	node->down()->setUp(node);
	node->head()->incNumOfElements();
}

shared_ptr<DLXSolver::DLXColHeader> DLXSolver::chooseNextColumn()
{
	// The column header row contains only DLXColHeader nodes, safe to cast statically
	auto currColHeader = std::static_pointer_cast<DLXColHeader>(_matrixHead->right());

	// Advance the pointer to the first available mandatory column
	while ((currColHeader != _matrixHead) && (currColHeader->colIndex() < _optionalColsNum))
	{
		currColHeader = std::static_pointer_cast<DLXColHeader>(currColHeader->right());
	}

	// Treat the edge cases: no columns remain
	if (currColHeader == _matrixHead)
		return NULL;
	else if (currColHeader->right() == _matrixHead) // One column remains - choose it
		return currColHeader;

	int minNumOfElements = currColHeader->numOfElements();
	auto chosenHeader = currColHeader;

	// Iterate all remaining column headers until the cyclic loop completes
	for (; currColHeader->right() != _matrixHead; currColHeader = std::static_pointer_cast<DLXColHeader>(currColHeader->right()))
	{
		auto nextColHeader = std::static_pointer_cast<DLXColHeader>(currColHeader->right());
		int nextNumOfElements = nextColHeader->numOfElements();

		// New minimal column candidate encountered
		if (nextNumOfElements < minNumOfElements)
		{
			chosenHeader = nextColHeader;
			minNumOfElements = nextNumOfElements;
		}
	}

	return chosenHeader;
}

void DLXSolver::cover(shared_ptr<DLXColHeader> column)
{
	// Remove the column
	detachNodeFromRow(column);

	// Remove each row that contains a value for this column
	for (auto rowNode = column->down(); rowNode != column; rowNode = rowNode->down())
	{
		// To remove a row - we iterate each of the nodes in the row and detach them from their columns
		// (the links within the detached row remain intact, to be able to reattach it when backtracking)
		for (auto horzNode = rowNode->right(); horzNode != rowNode; horzNode = horzNode->right())
		{
			detachNodeFromCol(std::static_pointer_cast<DLXDataNode>(horzNode));
		}
	}
}

void DLXSolver::uncover(shared_ptr<DLXColHeader> column)
{
	// Reattach each row that contains a value for this column
	for (auto rowNode = column->up(); rowNode != column; rowNode = rowNode->up())
	{
		// To reattach a row - we iterate each of the nodes in the row and attach them back to their columns
		for (auto horzNode = rowNode->left(); horzNode != rowNode; horzNode = horzNode->left())
		{
			reattachNodeToCol(std::static_pointer_cast<DLXDataNode>(horzNode));
		}
	}

	// Reattach the column back to the columns row
	reattachNodeToRow(column);
}

/** Creates a new "Exact Cover Problem" solver, using "Full Cover" mode.
 *  Only solutions that cover the entire universe are outputted.
 *  The universe is 0..numberOfColumns.
 */
DLXSolver::DLXSolver(int numberOfColumns) : DLXSolver(0, numberOfColumns)
{
}

/** Creates a new "Exact Cover Problem" solver, using "Partial Cover" mode.
*  Only solutions that cover the mandatory columns are outputted.
*  Note when adding rows - which value indices fall under the mandatory columns and which
*  under the optional ones.
*  The universe to cover is indices [numberOfOptionalCols..numberOfOptionalCols + numberOfMandatoryCols - 1].
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
void DLXSolver::addRow(shared_ptr<DLX_VALUES_SET> row)
{
	if (row->empty())
		return;

	DLX_VALUES_SET_ITERATOR iter = row->begin();
	int firstValue = *iter;
	auto firstNode = std::make_shared<DLXDataNode>(_rowNum, firstValue); // rowIndex = _rowNum, _colIndex = firstValue
	addNodeToColumn(firstNode);
	iter++;
	auto prevNode = firstNode;

	// Parse each value in the row's value set and create a node out of it.
	// Nodes are placed immediately in the sparse matrix, attaching them to adjacent row and column links.
	for (; iter != row->end(); iter++)
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

void DLXSolver::search(vector<DLX_SOLUTION>& solutions, DLX_SOLUTION& currentSolution)
{
	// Limit the number of solutions returned
	if (solutions.size() >= MAX_NUM_OF_SOLUTIONS)
		return;

	// Choose the next column to eliminate
	shared_ptr<DLXColHeader> chosenColumn = chooseNextColumn();

	// No more columns remain, the current solution is successful.
	// Record it and backtrack.
	if (chosenColumn == NULL)
	{
		solutions.push_back(currentSolution);
		return;
	}
	else if (chosenColumn->down() == chosenColumn) 
	{ // No rows remain, cannot cover the universe with the choices so far. Backtrack and try other solutions
		return;
	}
	
	// Remove the chosen column and the rows associated with it
	cover(chosenColumn);

	// Test each of the rows non-deterministically as partial solution candidates
	for (auto rowNode = chosenColumn->down(); rowNode != chosenColumn; rowNode = rowNode->down())
	{
		DLX_VALUES_SET rowValues;
		rowValues.insert(chosenColumn->colIndex()); // Construct the set of values representing the row

		// Go over the elements of the row: cover them, and add them to the rowValues set
		for (auto horzNode = rowNode->right(); horzNode != rowNode; horzNode = horzNode->right())
		{
			auto dataNode = std::static_pointer_cast<DLXDataNode>(horzNode);
			rowValues.insert(dataNode->colIndex()); // Construct the set of values representing the row
			cover(dataNode->head()); // Remove the node's column and rows associated with it
		}

		currentSolution.push_back(rowValues); // Add the tentative solution part to current solution
		search(solutions, currentSolution); // Recurse and continue searching & covering the universe
		
		// When the search above completes, we're ready to backtrack and try other solutions.
		// Undo the last step to backtrack:
		currentSolution.pop_back(); // Removes the last element - which is the solution part candidate

		for (auto horzNode = rowNode->left(); horzNode != rowNode; horzNode = horzNode->left())
		{
			auto dataNode = std::static_pointer_cast<DLXDataNode>(horzNode);
			uncover(dataNode->head()); // Recover the node's column and rows associated with it
		}
	}

	uncover(chosenColumn);
}

/** Solves the cover problem and returns the possible solutions found. */
vector<DLX_SOLUTION> DLXSolver::solve()
{
	vector<DLX_SOLUTION> solutions = vector<DLX_SOLUTION>();

	// If the solver wasn't initialized with a proper universe, quit gracefully with an empty solutions list
	if (_mandatoryColsNum == 0)
		return solutions;

	DLX_SOLUTION currentSolution = DLX_SOLUTION();
	search(solutions, currentSolution);
	return solutions;
}

std::ostream& DLXSolver::DLXNode::print(std::ostream& os) const
{
	os  << "[upColIndex=" << _up->colIndex() << "], "
		<< "[downColIndex=" << _down->colIndex() << "], "
		<< "[leftColIndex=" << _left->colIndex() << "], "
		<< "[rightColIndex=" << _right->colIndex() << "], ";
	return os;
}

std::ostream& DLXSolver::DLXColHeader::print(std::ostream& os) const 
{
	os << "DLXColHeader: ";
	DLXNode::print(os);
	os << "[setCount=" << _setCount << "]" << std::endl;
	return os;
}

std::ostream& DLXSolver::DLXDataNode::print(std::ostream& os) const
{
	os << "DLXDataNode: ";
	DLXNode::print(os);
	os << "[rowIndex=" << _rowIndex << "]" << std::endl;
	return os;
}

std::ostream& operator<<(std::ostream& stream, const DLXSolver& solver) {

	stream << "Dancing links solver data: " << std::endl;

	// Print the matrix's nodes info from top left to right bottom, column by column
	for (auto colNode = solver._matrixHead->right(); colNode != solver._matrixHead; colNode = colNode->right())
	{
		colNode->print(stream);

		for (auto rowNode = colNode->down(); rowNode != colNode; rowNode = rowNode->down())
		{
			rowNode->print(stream);
		}
	}

	return stream;
}