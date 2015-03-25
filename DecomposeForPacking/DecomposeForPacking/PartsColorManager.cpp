#include "PartsColorManager.h"
#include <time.h>
#include <algorithm>

PartsColorManager::PartsColorManager(FinalDecomposeResults decomposeResult, FinalPackResults packResult):
_decomposeResults(decomposeResult), _packResults(packResult), _mode(ColorMode::CONSISTENT), _currentColorIndex(0)
{
	// Generate common predefined colors
	srand(time(NULL));

	_partOrientationToColor[0] = std::make_tuple(255, 0, 0);
	_partOrientationToColor[1] = std::make_tuple(0, 0, 255);
	_partOrientationToColor[2] = std::make_tuple(0, 255, 0);
	_partOrientationToColor[7] = std::make_tuple(255, 255, 0);
	_partOrientationToColor[4] = std::make_tuple(255, 0, 255);
	_partOrientationToColor[5] = std::make_tuple(0, 255, 255);
	_partOrientationToColor[6] = std::make_tuple(205, 50, 155);
	_partOrientationToColor[3] = std::make_tuple(185, 139, 0);
	_partOrientationToColor[8] = std::make_tuple(149, 55, 55);
	_partOrientationToColor[9] = std::make_tuple(255, 0, 128);
	_partOrientationToColor[10] = std::make_tuple(74, 69, 42);
	_partOrientationToColor[11] = std::make_tuple(100, 150, 42);
	_partOrientationToColor[12] = std::make_tuple(30, 169, 250);
	_partOrientationToColor[13] = std::make_tuple(0, 140, 120);
	_partOrientationToColor[14] = std::make_tuple(160, 48, 160);

	for (int index = 0; index < 15; index++)
	{
		_colorsCache.push_back(_partOrientationToColor[index]);
	}
}

PartsColorManager::~PartsColorManager()
{
}

RGB_COLOR PartsColorManager::generateRandomColor()
{
	if (_currentColorIndex >= _colorsCache.size())
	{
		float r, g, b;
		r = (rand() % 255);
		g = (rand() % 255);
		b = (rand() % 255);
		RGB_COLOR generatedColor = std::make_tuple(r, g, b);
		_colorsCache.push_back(generatedColor);
	}

	return _colorsCache.at(_currentColorIndex++);
}

void PartsColorManager::regenerateColorsCache()
{
	_partOrientationToColor.clear();

	// Reset colors cache to 15 common colors in random order
	RGB_COLOR commonColors[15];

	for (int index = 0; index < 15; index++)
	{
		commonColors[index] = _colorsCache.at(index);
	}

	_colorsCache.clear();

	for (int index = 0; index < 15; index++)
	{
		_colorsCache.push_back(commonColors[index]);
	}

	std::random_shuffle(_colorsCache.begin(), _colorsCache.end());
}

RGB_COLOR PartsColorManager::getColorForPartOrientation(int partOrientationID)
{
	// No color for the current part orientation -- so generate
	if (_partOrientationToColor.find(partOrientationID) == _partOrientationToColor.end())
	{
		_partOrientationToColor[partOrientationID] = generateRandomColor();
	}

	return _partOrientationToColor[partOrientationID];
}

RGB_COLOR PartsColorManager::getColor(PartOrientation& partOrientation)
{
	// Packing
	switch (_mode)
	{
		case (ColorMode::ORIGINAL):
		{
			return getColorForPartOrientation(partOrientation.getId());
			break;
		}
		case (ColorMode::BY_INSTANCE):
		{
			int partOrientationID = partOrientation.getPartId();
			auto iter = _iteratedPartToColor[partOrientationID];
			RGB_COLOR color = *iter;
			_iteratedPartToColor[partOrientationID]++; // Advance iterator
			return color;
			break;
		}
		case (ColorMode::CONSISTENT):
		{
			// Decompose
			if (_isCurrentSolutionDecompose)
			{
				return getColorForPartOrientation(partOrientation.getId());
			}
			else
			{
				int partID = partOrientation.getPartId();
				auto iter = _iteratedPartToColor[partID];
				RGB_COLOR color = *iter;
				_iteratedPartToColor[partID]++; // Advance iterator
				return color;
			}
			break;
		}
		default:
		{ // Should never happen
			return std::make_tuple(255, 255, 255);
		}
	}
}

void PartsColorManager::changeMode(ColorMode mode)
{
	_mode = mode;
}

void PartsColorManager::resetColorsToSolution(int solutionIndex, bool isCurrentSolutionDecompose)
{
	_currentColorIndex = 0;
	_isCurrentSolutionDecompose = isCurrentSolutionDecompose;
	vector<tuple<PartOrientationPtr, Point>> decomposeSolutionParts = *(_decomposeResults->at(solutionIndex));

	_partToColor.clear();

	for (const auto& partTuple : decomposeSolutionParts)
	{
		PartOrientationPtr orientationPtr = std::get<0>(partTuple);

		RGB_COLOR orientationColor;

		// Mode Consistent (_partToColor stores colors of all orientations)
		if (_mode == ColorMode::CONSISTENT)
		{
			orientationColor = getColorForPartOrientation(orientationPtr->getId());
		}
		else
		{
			orientationColor = generateRandomColor();
		}

		// Create vector if needed
		if (_partToColor.find(orientationPtr->getPartId()) == _partToColor.end())
		{
			_partToColor[orientationPtr->getPartId()] = std::make_shared<vector<RGB_COLOR>>();
		}

		_partToColor[orientationPtr->getPartId()]->push_back(orientationColor);
	}

	resetIteratedColorData();
}

void PartsColorManager::resetIteratedColorData()
{
	_iteratedPartToColor.clear();

	// Initialize iterators for all part colors (Consistent mode)
	for (const auto& partData : _partToColor)
	{
		int partID = std::get<0>(partData);
		shared_ptr<vector<RGB_COLOR>> partColors = std::get<1>(partData);
		_iteratedPartToColor[partID] = partColors->begin();
	}
}