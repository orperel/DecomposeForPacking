#pragma once

#include <memory>
#include <vector>
#include <set>
#include <unordered_map>

using std::vector;
using std::set;
using std::unordered_map;
using std::shared_ptr;
using std::unique_ptr;

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
	 *  Note: Rows added are not verified in order to remove performance overhead.
	 *	Make sure that:
	 *	1) Rows added do not contain values below 0 or equal / above the column count (mandatory + optional).
	 *  2) For partial cover mode, each row must contain at least one mandatory value.
	 */
	void addRow(shared_ptr<DLX_VALUES_SET> row);

	/** Solves the cover problem and returns the possible solutions found. */
	vector<DLX_SOLUTION> solve();

	/* Operator Overloading - string representation of DLXSolver.
	 * The entire matrix data will be printed.
	 * WARNING: For big matrices this output can be very big.
	 */
	friend std::ostream& operator<< (std::ostream& stream, const DLXSolver& solver);

private:
	// Consts:

	/** Limits the number of solutions returned by DLXSolver, to avoid long running times
	 *  when many solutions are available. DlXSolver will choose only the first MAX_NUM_OF_SOLUTIONS
	 *  solutions encountered.
	 */
	static const int MAX_NUM_OF_SOLUTIONS = 10;
	
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
	inline void detachNodeFromCol(shared_ptr<DLXDataNode> node);

	/** Reattaches the node back to the column according to the node's up / down pointers. */
	inline void reattachNodeToCol(shared_ptr<DLXDataNode> node);

	/** Chooses the next column to be removed from the matrix.
	 *  The algorithm uses the heuristic - "choose the column with the least elements" (a value that appears in 
	 *  less sets is less likely to cause massive branching).
	 *  If no columns remain in the matrix, NULL is returned.
	 *  For partial cover mode, only mandatory columns are chosen.
	 *  If no mandatory columns remain in the matrix, NULL is returned.
	 */
	shared_ptr<DLXColHeader> chooseNextColumn();

	/** Removes:
	 *  1) The given column 
	 *  2) All rows that contain values for this column in the sparse matrix.
	 */
	void cover(shared_ptr<DLXColHeader> column);

	/** Reattaches the given column, and all rows that contain values for this column back to the sparse matrix.
	 *  The method relies on the links within the columns and the removed rows being intact.
	 */
	void uncover(shared_ptr<DLXColHeader> column);

	/** Performs a recursive step of search -
	 *  1) Choose a column deterministically.
	 *  2) Choose each of the rows non-deterministically and perform a cover of intersecting rows / columns.
	 *  3) Repeat until sucess / failure and backtrack.
	 */
	void search(vector<DLX_SOLUTION>& solutions, DLX_SOLUTION& currentSolution);

	// Inner classes

	/** A general base representation for all nodes in the matrix.
	 */
	class DLXNode
	{
	protected:
		int _colIndex; // Index of the column (from 0 to number of columns - 1)

		// Point to adjacent nodes to maintain a sparse matrix structure
		shared_ptr<DLXNode> _up;
		shared_ptr<DLXNode> _down;
		shared_ptr<DLXNode> _right;
		shared_ptr<DLXNode> _left;
	public:
		DLXNode(int colIndex) : _colIndex(colIndex) {}
		virtual ~DLXNode() = 0 {} ; // Mark class as abstract

		// Getters & Setters
		const int& colIndex() const { return _colIndex; }  // Current column index (the value depicted by this column)
		const shared_ptr<DLXNode>& up() const { return _up; }
		void setUp(const shared_ptr<DLXNode>& node){ _up = node; }
		const shared_ptr<DLXNode>& down() const { return _down; }
		void setDown(const shared_ptr<DLXNode>& node){ _down = node; }
		const shared_ptr<DLXNode>& right() const { return _right; }
		void setRight(const shared_ptr<DLXNode>& node){ _right = node; }
		const shared_ptr<DLXNode>& left() const { return _left; }
		void setLeft(const shared_ptr<DLXNode>& node){ _left = node; }

		virtual std::ostream& print(std::ostream& os) const; // String representation for the node
	};

	/** A column header node, in the matrix.
	 *  Column headers define the begining of each column in the matrix.
	 *  They point circularily to the last element in the column as well on par with the Dancing Links paradigm
	 * (as well as to the previous and next column headers).
	 */
	class DLXColHeader: public DLXNode
	{
	private:
		static const int SENTINEL_INDEX = -1; // Index of the sentinel header, the root that points to all column headers
		int _setCount; // The number of sets that contain the value represented by this column
		
	public:
		DLXColHeader(int colIndex) : DLXNode(colIndex), _setCount(0) {}
		virtual ~DLXColHeader() = default;
		static shared_ptr<DLXColHeader> createSentinelHeader() { return std::make_shared<DLXColHeader>(SENTINEL_INDEX); }
		bool isSentinel() { return _colIndex == SENTINEL_INDEX; }

		// Getters & Setters
		const int& numOfElements() const { return _setCount; } // Number of sets containing the value depicted by this column
		void incNumOfElements(){ _setCount++; } // Increases the number of elements in the column by 1
		void decNumOfElements(){ _setCount--; } // Decreases the number of elements in the column by 1

		// Operator Overloading
		virtual std::ostream& print(std::ostream& os) const; // Hierarchial string representation
	};

	/** A data node representing a single element in the matrix.
	 *  Data nodes represent a value (the column) that exists in a certain set (the row).
	 *  They point circularily to all 4 directions on par with the Dancing Links paradigm.
	 */
	class DLXDataNode: public DLXNode
	{
	private:
		// Position index in the matrix
		int _rowIndex;

		// Pointer to the head of the column, containing meta-data relevant for the column of the node
		shared_ptr<DLXColHeader> _head;
	public:
		DLXDataNode(int rowIndex, int colIndex) : DLXNode(colIndex), _rowIndex(rowIndex) {}
		virtual ~DLXDataNode() = default;

		// Getters & Setters
		const int& rowIndex() const { return _rowIndex; }
		const shared_ptr<DLXColHeader>& head() const { return _head; }
		void setHead(const shared_ptr<DLXColHeader>& node){ _head = node; }

		virtual std::ostream& print(std::ostream& os) const; // Hierarchial string representation
	};
};

