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

/** Returns true if the mandatory column is true within the matrix, false if it is optional.
*/
bool DLXSolver::isMandatoryColumn(DLXColHeader* column)
{
	// Optional columns are always indexed before the mandatory ones
	return column->colIndex() >= _optionalColsNum;
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

shared_ptr<DLXSolver::DLXColHeader> DLXSolver::chooseNextColumn(bool isAllowPartialCover)
{
	// The column header row contains only DLXColHeader nodes, safe to cast statically
	auto currColHeader = std::static_pointer_cast<DLXColHeader>(_matrixHead->right());

	// Advance the pointer to the first available mandatory column
	while ((currColHeader != _matrixHead) && (!isMandatoryColumn(currColHeader.get())))
	{
		currColHeader = std::static_pointer_cast<DLXColHeader>(currColHeader->right());
	}

	// Treat the edge cases: no columns remain
	if (currColHeader == _matrixHead)
		return NULL;
	else if (currColHeader->right() == _matrixHead) // One column remains - choose it
		return currColHeader;

	int minNumOfElements = _rowNum + 1; // The maximal number of rows is _rowNum, number of rows in the matrix
	shared_ptr<DLXSolver::DLXColHeader> chosenHeader = NULL;

	// This parameter will only be used if partial cover solutions are allowed -
	// If a column with more than 0 rows is available it'll be chosen in stead of a column with 0 rows
	// (but if only columns with 0 rows remain, they will be chosen).
	shared_ptr<DLXSolver::DLXColHeader> chosenHeaderWithZeroRows = NULL;

	// Iterate all remaining column headers until the cyclic loop completes
	for (; currColHeader != _matrixHead; currColHeader = std::static_pointer_cast<DLXColHeader>(currColHeader->right()))
	{
		int nextNumOfElements = currColHeader->numOfElements();

		if (nextNumOfElements == 0) // A column with 0 rows encountered
		{
			if (isAllowPartialCover)
			{ // partial covers are allowed, remember this column but keep looking for other minimal non-zero row columns.
			  // This column will be returned only if other columns are not found.
				chosenHeaderWithZeroRows = currColHeader;
			}
			else
			{ // If only exact covers are allowed, return this column immediately (no better choices are available)
				return currColHeader;
			}
		}
		// New minimal column candidate encountered
		else if (nextNumOfElements < minNumOfElements)
		{
			chosenHeader = currColHeader;
			minNumOfElements = nextNumOfElements;
		}
	}

	// Return the column with the minimal number of rows which is not zero.
	// If such a column doesn't exist, only columns with zero rows remain -- return one of them
	if (chosenHeader == NULL)
		return chosenHeaderWithZeroRows;
	else
		return chosenHeader;
}

void DLXSolver::cover(shared_ptr<DLXColHeader> column)
{
	// Remove the column
	detachNodeFromRow(column);

	// If this is a mandatory column that is removed, update the counter
	if (isMandatoryColumn(column.get()))
		_remainingMandatoryColsNum--;

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

	// If this is a mandatory column that is reattached, update the counter
	if (isMandatoryColumn(column.get()))
		_remainingMandatoryColsNum++;
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
_rowNum(0), _bestPartialCoverFound(SOLVER_NOT_INITIALIZED)
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

/** Performs a recursive step of search -
*  1) Choose a column deterministically.
*  2) Choose each of the rows non-deterministically and perform a cover of intersecting rows / columns.
*  3) Repeat until sucess / failure and backtrack.
*
* Note: isAllowPartialCover is an optional argument used only when exact cover solutions are not available.
* If isAllowPartialCover=true, search will cache the best cover found so far (even if its not exact)
* and add it to the possible solutions (if future search calls find better covers, they override this
* solution).
*/
void DLXSolver::search(vector<DLX_SOLUTION>& solutions, DLX_SOLUTION& currentSolution, bool isAllowPartialCover)
{
	// Limit the number of solutions returned
	// -- The number of exact cover solutions is always limited (even when isAllowPartialCover is on)
	// -- The number of partial cover solutions is only limited if the solver is configured to do so
	if ((solutions.size() >= MAX_NUM_OF_SOLUTIONS) &&
		((!isAllowPartialCover) ||
		((isAllowPartialCover) && (IS_LIMIT_PARTIAL_COVER_SOLUTIONS || isExactCoverFound()))))
	{
		return;
	}

	// Choose the next column to eliminate
	shared_ptr<DLXColHeader> chosenColumn = chooseNextColumn(isAllowPartialCover);

	// No more columns remain, the current solution is successful.
	// Record it and backtrack.
	if (chosenColumn == NULL)
	{
		// First exact cover solution was found, remove partial solutions found so far if there are any
		if (_bestPartialCoverFound > 0)
		{
			solutions.clear();
			_bestPartialCoverFound = 0;
		}
		
		solutions.push_back(currentSolution);

		return;
	}
	else if (chosenColumn->down() == chosenColumn) 
	{ // No rows remain, cannot cover the universe with the choices so far. Backtrack and try other solutions

		// If partial cover is allowed, we check whether the current partial solution is better than the
		// other solutions found so far and record it
		if (isAllowPartialCover)
		{
			// The current solution is a better partial solution than those found so far.
			// Remove the other partial solutions found so far if there are any
			if (_remainingMandatoryColsNum < _bestPartialCoverFound)
			{
				solutions.clear();
				_bestPartialCoverFound = _remainingMandatoryColsNum;
				solutions.push_back(currentSolution);
			}
			// The current solution is of an equal quality to the other partial solutions found so far.
			// Add it to the other partial solutions.
			else if (_remainingMandatoryColsNum == _bestPartialCoverFound)
			{
				solutions.push_back(currentSolution);
			}
		}

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

		// Add the tentative solution part to current solution
		currentSolution.push_back(rowValues);

		// Recurse and continue searching & covering the universe
		search(solutions, currentSolution, isAllowPartialCover);
		
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

/** Solves the cover problem and returns the possible solutions found.
 *  Depending on the parameter given,
 *  this overload may allow return partial cover solutions if an exact cover is not possible.
 */
vector<DLX_SOLUTION> DLXSolver::solve(bool isAllowPartialCover)
{
	vector<DLX_SOLUTION> solutions = vector<DLX_SOLUTION>();

	// If the solver wasn't initialized with a proper universe, quit gracefully with an empty solutions list
	if ((_mandatoryColsNum == 0) || (_rowNum == 0))
		return solutions;

	DLX_SOLUTION currentSolution = DLX_SOLUTION();
	_remainingMandatoryColsNum = _mandatoryColsNum; // All columns remain while we start searching
	_bestPartialCoverFound = _mandatoryColsNum; // The best partial cover will contain less columns as 
												// we gradually cover the unvierse
												// (only used when isAllowPartialCover == true)
	search(solutions, currentSolution, isAllowPartialCover);
	return solutions;
}

/** Solves the cover problem and returns the possible solutions found. */
vector<DLX_SOLUTION> DLXSolver::solve()
{
	// Solve with exact covers only
	return solve(false);
}

/** Returns true is the last "solve" operation found an exact cover.
*  False otherwise.
*/
bool DLXSolver::isExactCoverFound()
{
	return _bestPartialCoverFound == 0;
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