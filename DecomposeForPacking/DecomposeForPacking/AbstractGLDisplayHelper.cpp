#include "AbstractGLDisplayHelper.h"
#include <thread>
#include "DFPLogger.h"
#include <string>

AbstractGLDisplayHelper::AbstractGLDisplayHelper() : _isCallbackReceived(false)
{

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
	_renderer->initRenderingLoop(_renderContext);
	DFPLogger::getInstance().log("Quitting OpenGL render thread", DFPLogger::LogLevel::DEBUG_MODE);
}

void AbstractGLDisplayHelper::displayResults(WorldPtr world, DecomposeAndPackResult dapResults)
{
	FinalDecomposeResults decomposeResult = std::get<0>(dapResults);
	FinalPackResults packResult = std::get<1>(dapResults);
	_colorManager = std::make_shared<PartsColorManager>(decomposeResult, packResult);
	displayDecomposePackResults(world, decomposeResult, packResult);
}

void AbstractGLDisplayHelper::initRenderingContext(WorldPtr world)
{
	int height = world->getHeight();
	int width = world->getWidth();
	int depth = world->getDepth();
	int pixelResolution = world->getPixelResolution();
	_renderContext = std::make_shared<OpenGLRenderContext>(width, height, depth, pixelResolution);
}

void AbstractGLDisplayHelper::displayDecomposePackResults(WorldPtr world, FinalDecomposeResults decomposeResult, FinalPackResults packResult)
{
	initRenderingContext(world);
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

				isCurrentDisplayDecompose = !isCurrentDisplayDecompose;
			}
			else if (_lastCallbackKey == KEYBOARD_KEY::KEY_RIGHT)
			{
				if (!isCurrentDisplayDecompose)
				{
					decomposeResultsIndex++;
				}

				isCurrentDisplayDecompose = !isCurrentDisplayDecompose;
			}
			else if (_lastCallbackKey == KEYBOARD_KEY::KEY_1)
			{
				_colorManager->changeMode(ColorMode::CONSISTENT);
			}
			else if (_lastCallbackKey == KEYBOARD_KEY::KEY_2)
			{
				_colorManager->changeMode(ColorMode::BY_INSTANCE);
			}
			else if (_lastCallbackKey == KEYBOARD_KEY::KEY_3)
			{
				_colorManager->changeMode(ColorMode::ORIGINAL);
			}
			else if (_lastCallbackKey == KEYBOARD_KEY::KEY_R)
			{
				_colorManager->regenerateColorsCache();
			}
			
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
				_colorManager->resetColorsToSolution(decomposeResultsIndex, true);
				paintSingleSolution(world, decomposeResult->at(decomposeResultsIndex));

				string solutionIndex = std::to_string(decomposeResultsIndex + 1);
				string description = "Decompose solution #" + solutionIndex;
				_renderContext->setContentDescription(description);
			}
			else
			{
				_colorManager->resetColorsToSolution(decomposeResultsIndex, false);
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