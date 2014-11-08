#pragma once

#include <vector>
#include <set>
#include <unordered_map>
#include <memory>

using std::vector;
using std::set;
using std::unordered_map;
using std::shared_ptr;

typedef set<int> DLX_VALUES_SET;
typedef DLX_VALUES_SET::iterator DLX_VALUES_SET_ITERATOR;
typedef vector<DLX_VALUES_SET> DLX_SOLUTION;

/**
* A solver implementation for the Exact Cover Problem, using "Algorithm X".
* This solver is a slightly modified version of the backtracking "Dancing Links"
* implementation originally suggested by Donald Knuth.
* The solver allows 2 modes:
* 1) Classic Full Cover solution - by outputing only solutions that cover the entire given universe of values.
* 2) Partial Cover solution - by outputing solutions that cover only mandatory values.
*    Other values contained in the input sets will be used for covering, but the algorithm does not insist
*    on covering these values for the solution to count (they still cannot collide, however - meaning
*    the same value cannot be picked twice from 2 different sets).
*
* In any case, a valid solution will cover each value at most once.
* The implementation uses a sparse matrix to represent the given input:
* Columns - each column represents a possible value in the universe.
*			 The solver will aim to pick sets of values that cover all these values.
*			 For Full Cover mode - all values in the universe will be covered.
*			 For Partial Cover mode - only the mandatory columns will be required to be covered.
*			 Other values contained in the sets may be used to cover as well, but a valid solution may
*           leave some of these columns uncovered.
*			 i.e: Columns for the numbers 1, 2, 3, ... , 10.
* Rows -    each row represents a single set of values.
*		     The solver outputs combinations of sets of values that cover the universe.
*			 i.e: Rows for the sets: {1, 4}, {2, 4, 7}, {1, 3, 9}, ...
*/
class DLXSolver
{
private:
	// Forward declarations & name abbreviations
	class DLXNode;
	class DLXColHeader;
	class DLXDataNode;
	using DLXNode = DLXSolver::DLXNode;
	using DLXColHeader = DLXSolver::DLXColHeader;
	using DLXDataNode = DLXSolver::DLXDataNode;

public:
	/** Creates a new "Exact Cover Problem" solver, using "Full Cover" mode.
	 *  Only solutions that cover the entire universe are outputted.
	 *  The universe is 0..numberOfColumns.
	 */
	DLXSolver(int numberOfColumns);

	/** Creates a new "Exact Cover Problem" solver, using "Partial Cover" mode.
 	 *  Only solutions that cover the mandatory columns are outputted.
	 *  Note when adding rows - which value indices fall under the mandatory columns and which
	 *  under the optional ones.
	 *  The uninverse to cover is indices [numberOfOptionalCols..numberOfOptionalCols + numberOfMandatoryCols - 1].
	 */
	DLXSolver(int numberOfOptionalCols, int numberOfMandatoryCols);

	/** Dtor to release resources allocated by the solver.. */
	virtual ~DLXSolver();

	/** Add a new set of values to test against as part of the solution.
	 *  Values are indexed from 0 to N the following way:
	 *  Full cover mode -
	 *  [0 .. numberOfColumns - 1]
	 *  Partial cover mode - (optional columns indexed first, mandatory columns following immediatly afterwards) -
	 *  [0 .. numberOfOptionalColumns - 1, numberOfOptionalColumns, numberOfOptionalColumns + 1 .. numberOfOptionalColumns + numberOfMandatoryColumns]
	 */
	void addRow(DLX_VALUES_SET row);

	/** Solves the cover problem and returns the possible solutions found. */
	vector<DLX_SOLUTION> solve();


private:
	// Data members:

	/** Contains the current number of rows in the solver's matrix.
	 *  Each row represents a set of values used to cover the "universe".
	 */
	int _rowNum;

	/** Represents the number of mandatory columns in the solver's matrix.
	 *  Each mandatory column represents a value in the "universe" that must be covered
	 *  in the final solution as well.
	 */
	int _mandatoryColsNum;

	/** Represents the number of optional columns in the solver's matrix.
	 *  An optional column represents values that may get covered because they're included with
	 *  mandatory values in the sets picked. However, the solution doesn't insist on covering these values.
	 */
	int _optionalColsNum;

	/** A pointer to the sentinel head of the column headers of the matrix.
	 *  The sentinel head must always exist and is used to access the rest of the matrix.
	 */
	shared_ptr<DLXColHeader> _matrixHead;
	
	/** Allows fast access to the column headers by index using a hash-table */
	unordered_map<int, shared_ptr<DLXColHeader>> _colHeadersMapping;

	// Logic Methods:

	/** Creates the matrix head and the column headers of the sparse matrix and links them to each other */
	inline void createColumnHeaders(int numOfColumns);

	/** Adds and links the node to other nodes in its approporiate column (according to the node's column index) */
	inline void addNodeToColumn(shared_ptr<DLXDataNode> node);

	/** Detaches the node from the row, removing the links from the links adjcant to it to the removed node.
	 *  The removed node's link pointers are retained, however, making a reattachment possible when needed for backtracking.
	 */
	inline void detachNodeFromRow(shared_ptr<DLXNode> node);

	/** Reattaches the node back to the row according to the node's left / right pointers. */
	inline void reattachNodeToRow(shared_ptr<DLXNode> node);

	/** Detaches the node from the column, removing the links from the links adjcant to it to the removed node.
	 *  The removed node's link pointers are retained, however, making a reattachment possible when needed for backtracking.
	 */
	inline void detachNodeFromCol(shared_ptr<DLXNode> node);

	/** Reattaches the node back to the column according to the node's up / down pointers. */
	inline void reattachNodeToCol(shared_ptr<DLXNode> node);

	inline void detachRow();
	inline void detachColumn();

	/** Chooses the next column to be removed from the matrix.
	 *  The algorithm uses the heuristic - "choose the column with the least elements" (a value that appears in 
	 *  less sets is less likely to cause massive branching).
	 */
	shared_ptr<DLXColHeader> chooseNextColumn();

	/** Removes the given column, and all rows that contain values for this column from the sparse matrix
	 */
	void cover(shared_ptr<DLXColHeader>);

	/** Reattaches the given column, and all rows that contain values for this column back to the sparse matrix.
	 */
	void uncover(shared_ptr<DLXColHeader>);

	// Inner classes
	class DLXNode
	{
	protected:
		// Point to adjacent nodes to maintain a sparse matrix structure
		shared_ptr<DLXNode> _up;
		shared_ptr<DLXNode> _down;
		shared_ptr<DLXNode> _right;
		shared_ptr<DLXNode> _left;
	public:
		virtual ~DLXNode() = 0 {} ; // Mark class as abstract

		// Getters & Setters
		const shared_ptr<DLXNode>& up() const { return _up; }
		void setUp(const shared_ptr<DLXNode>& node){ _up = node; }
		const shared_ptr<DLXNode>& down() const { return _down; }
		void setDown(const shared_ptr<DLXNode>& node){ _down = node; }
		const shared_ptr<DLXNode>& right() const { return _right; }
		void setRight(const shared_ptr<DLXNode>& node){ _right = node; }
		const shared_ptr<DLXNode>& left() const { return _left; }
		void setLeft(const shared_ptr<DLXNode>& node){ _left = node; }
	};

	class DLXColHeader: public DLXNode
	{
	private:
		static const int SENTINEL_INDEX = -1; // Index of the sentinel header, the root that points to all headers
		int _colIndex; // Index of the column (from 0 to number of columns - 1)
		int _setCount; // The number of sets that contain the value represented by this column

	public:
		DLXColHeader(int colIndex) : _colIndex(colIndex) {}
		virtual ~DLXColHeader() = default;
		static shared_ptr<DLXColHeader> createSentinelHeader() { return std::make_shared<DLXColHeader>(SENTINEL_INDEX); }
		bool isSentinel() { return _colIndex == SENTINEL_INDEX; }

		// Getters & Setters
		const int& colIndex() const { return _colIndex; }  // Current column index (the value depicted by this column)
		const int& numOfElements() const { return _setCount; } // Number of sets containing the value depicted by this column
		void incNumOfElements(){ _setCount++; } // Increases the number of elements in the column by 1
		void decNumOfElements(){ _setCount--; } // Decreases the number of elements in the column by 1

		// Operator Overloading
		std::ostream& print(std::ostream& os) const; // TODO: Overload for string representation
	};

	class DLXDataNode: public DLXNode
	{
	private:
		// Position indices in the matrix
		int _rowIndex;
		int _colIndex;

		// Pointer to the head of the column, containing meta-data relevant for the column of the node
		shared_ptr<DLXColHeader> _head;
	public:
		DLXDataNode(int rowIndex, int colIndex): _rowIndex(rowIndex), _colIndex(colIndex) {}
		virtual ~DLXDataNode() = default;

		// Getters & Setters
		const int& rowIndex() const { return _rowIndex; }
		const int& colIndex() const { return _colIndex; }
		const shared_ptr<DLXColHeader>& head() const { return _head; }
		void setHead(const shared_ptr<DLXColHeader>& node){ _head = node; }

		// Operator Overloading
		std::ostream& print(std::ostream& os) const; // TODO: Overload for string representation
	};
};

