#include "AbstractGLDisplayHelper.h"
#include <thread>
#include "DFPLogger.h"
#include <time.h>
#include <string>

AbstractGLDisplayHelper::AbstractGLDisplayHelper() : _isCallbackReceived(false)
{
	srand(time(NULL));

	_partOrientationToColor[0] = std::make_tuple(255, 0, 0);
	_partOrientationToColor[1] = std::make_tuple(0, 0, 255);
	_partOrientationToColor[2] = std::make_tuple(0, 255, 0);
	_partOrientationToColor[7] = std::make_tuple(255, 255, 0);
	_partOrientationToColor[4] = std::make_tuple(255, 0, 255);
	_partOrientationToColor[5] = std::make_tuple(0, 255, 255);
	_partOrientationToColor[6] = std::make_tuple(205, 50, 155);
	_partOrientationToColor[3] = std::make_tuple(255, 192, 0);
	_partOrientationToColor[8] = std::make_tuple(149, 55, 55);
	_partOrientationToColor[9] = std::make_tuple(255, 0, 128);
	_partOrientationToColor[10] = std::make_tuple(74, 69, 42);
	_partOrientationToColor[11] = std::make_tuple(100, 150, 42);
	_partOrientationToColor[12] = std::make_tuple(30, 169, 250);
	_partOrientationToColor[13] = std::make_tuple(0, 140, 120);
	_partOrientationToColor[14] = std::make_tuple(200, 69, 200);
}


AbstractGLDisplayHelper::~AbstractGLDisplayHelper()
{
}

void AbstractGLDisplayHelper::onKeyPressed(KEYBOARD_KEY key)
{
	_isCallbackReceived = true;
	_lastCallbackKey = key;
}

void AbstractGLDisplayHelper::initRenderingLoop()
{
	DFPLogger::getInstance().log("Starting OpenGL render thread", DFPLogger::LogLevel::DEBUG_MODE);
	_renderer->initRenderingLoop();
	DFPLogger::getInstance().log("Quitting OpenGL render thread", DFPLogger::LogLevel::DEBUG_MODE);
}

void AbstractGLDisplayHelper::displayResults(WorldPtr world, DecomposeAndPackResult dapResults)
{
	FinalDecomposeResults decomposeResult = std::get<0>(dapResults);
	FinalPackResults packResult = std::get<1>(dapResults);
	displayDecomposePackResults(world, decomposeResult, packResult);
}

void AbstractGLDisplayHelper::displayDecomposePackResults(WorldPtr world, FinalDecomposeResults decomposeResult, FinalPackResults packResult)
{
	std::thread openGLThread(&AbstractGLDisplayHelper::initRenderingLoop, this);
	paintWorld(world);

	while (!_renderer->isReady())
	{
		; // Sleep TODO
	}

	_renderer->commitContext(_renderContext);
	_renderer->addInputListener(this);

	int decomposeResultsIndex = -1;
	bool isCurrentDisplayDecompose = false;

	while (true)
	{
		; // Sleep TODO

		if (_isCallbackReceived)
		{
			if (_lastCallbackKey == KEYBOARD_KEY::KEY_LEFT)
			{
				if (isCurrentDisplayDecompose)
				{
					decomposeResultsIndex--;
				}
			}
			if (_lastCallbackKey == KEYBOARD_KEY::KEY_RIGHT)
			{
				if (!isCurrentDisplayDecompose)
				{
					decomposeResultsIndex++;
				}
			}
			
			isCurrentDisplayDecompose = !isCurrentDisplayDecompose;

			if (decomposeResultsIndex <= -1)
			{
				decomposeResultsIndex = -1;
				isCurrentDisplayDecompose = false;
			}
			else if (decomposeResultsIndex > packResult->size() - 1) {
				decomposeResultsIndex--;
				isCurrentDisplayDecompose = false;
			}

			_renderContext->clearDataBuffers();

			if ((decomposeResultsIndex == -1) || (decomposeResult->empty()))
			{
				paintWorld(world);
				string description = "World (object mask view)";
				description += decomposeResult->empty() ? ": No solutions" : "";
				_renderContext->setContentDescription(description);
			}
			else if (isCurrentDisplayDecompose)
			{
				paintSingleSolution(world, decomposeResult->at(decomposeResultsIndex));

				string solutionIndex = std::to_string(decomposeResultsIndex + 1);
				string description = "Decompose solution #" + solutionIndex;
				_renderContext->setContentDescription(description);
			}
			else
			{
				paintSingleSolution(world, packResult->at(decomposeResultsIndex));

				string solutionIndex = std::to_string(decomposeResultsIndex + 1);
				string description = "Packing solution #" + solutionIndex;
				_renderContext->setContentDescription(description);
			}

			_renderer->commitContext(_renderContext);

			_isCallbackReceived = false;
		}
	}

	DFPLogger::getInstance().log("Main thread quitting", DFPLogger::LogLevel::DEBUG_MODE);
}