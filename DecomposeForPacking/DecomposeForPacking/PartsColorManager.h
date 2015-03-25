#pragma once

#include <unordered_map>
#include <iterator>

#include "DecomposeResult.h"
#include "DecomposeAndPack.h"

using std::unordered_map;
using std::tuple;
using std::iterator;
using RGB_COLOR = tuple<float, float, float>;

enum class ColorMode
{
	ORIGINAL, // Color is assigned for each part orientation type
	BY_INSTANCE, // Color is assigned for each part orientation instance
	CONSISTENT // Color is assigned for each part orientation and remains the same for packing
};

/* Pick colors for parts */
class PartsColorManager
{
public:
	PartsColorManager(FinalDecomposeResults decomposeResult, FinalPackResults packResult);
	~PartsColorManager();

	RGB_COLOR getColor(PartOrientation& partOrientation);
	void changeMode(ColorMode mode);
	void resetColorsToSolution(int solutionIndex, bool isDecompose); // Reset when showing a new decompose solution
	void regenerateColorsCache();
private:
	ColorMode _mode;
	FinalDecomposeResults _decomposeResults; // Sorted by index
	FinalPackResults _packResults; // Sorted by index, match decompose results

	unordered_map< int, RGB_COLOR > _partOrientationToColor; // Color for each part orientation
	vector<RGB_COLOR> _colorsCache;
	int _currentColorIndex;

	// For consistent mode
	unordered_map< int, shared_ptr<vector<RGB_COLOR>> > _partToColor; // For each part, save a list of colors of the orientations
														  // according to decompose
	unordered_map< int, vector<RGB_COLOR>::iterator > _iteratedPartToColor; // Current iterator for each part
																			// over _partToColor

	RGB_COLOR generateRandomColor();
	void resetIteratedColorData();
	RGB_COLOR getColorForPartOrientation(int partOrientationID); // Generates if needed
	bool _isCurrentSolutionDecompose; // True - decompose, False - Packing
};

