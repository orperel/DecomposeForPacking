#include "GLDisplayHelper.h"
#include <thread>
#include "DFPLogger.h"
#include <time.h>
#include <string>

static const float RESOLUTION = 2;
static const float SQUARE_SIZE = 1;

GLDisplayHelper::GLDisplayHelper() : _isCallbackReceived(false)
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


GLDisplayHelper::~GLDisplayHelper()
{
}

void GLDisplayHelper::onKeyPressed(KEYBOARD_KEY key)
{
	_isCallbackReceived = true;
	_lastCallbackKey = key;
}

GLDisplayHelper::WorldDisplayVisitor::WorldDisplayVisitor(shared_ptr<OpenGLRenderContext> renderContext) :
_renderContext(renderContext)
{
}

void paintSquare(WorldPtr world, shared_ptr<OpenGLRenderContext> renderContext,
				 float x, float y, float r, float g, float b, float a)
{
	// TODO: Fill
}

void GLDisplayHelper::WorldDisplayVisitor::visit(World& world, Point point)
{
	// Paint a square for each unmasked pixel in the world
	float v1x = point.getX();
	float v1y = world.getHeight() - point.getY(); // OpenGL Y axis is inverted
	float v2x = v1x + SQUARE_SIZE;
	float v2y = v1y;
	float v3x = v1x;
	float v3y = v1y + SQUARE_SIZE;
	float v4x = v1x + SQUARE_SIZE;
	float v4y = v1y + SQUARE_SIZE;

	float r, g, b, a;
	a = 0.7f;
	r = 128;
	r /= 255.0;
	g = 128;
	g /= 255.0;
	b = 128;
	b /= 255.0;

	_renderContext->writeTriangle(v1x, v1y, v2x, v2y, v3x, v3y, r, g, b, a);
	_renderContext->writeTriangle(v2x, v2y, v3x, v3y, v4x, v4y, r, g, b, a);

	_renderContext->writeLine(v1x, v1y, v2x, v2y);
	_renderContext->writeLine(v2x, v2y, v4x, v4y);
	_renderContext->writeLine(v3x, v3y, v4x, v4y);
	_renderContext->writeLine(v3x, v3y, v1x, v1y);
}

void GLDisplayHelper::initRenderingLoop()
{
	DFPLogger::getInstance().log("Starting OpenGL render thread", DFPLogger::LogLevel::DEBUG_MODE);
	_renderer.initRenderingLoop();
	DFPLogger::getInstance().log("Quitting OpenGL render thread", DFPLogger::LogLevel::DEBUG_MODE);
}

void GLDisplayHelper::paintWorld(WorldPtr world)
{
	int height = world->getHeight();
	int width = world->getWidth();
	int pixelResolution = world->getPixelResolution();
	_renderContext = std::make_shared<OpenGLRenderContext>(width, height, pixelResolution);

	// Paint the world pixels to the context
	auto worldPainter = std::make_shared<WorldDisplayVisitor>(_renderContext);
	world->accept(worldPainter);
}

void GLDisplayHelper::paintDecomposeResults(WorldPtr world, PartLocationListPtr decomposeParts)
{
	for (tuple<PartOrientationPtr, Point>& partLocation : *decomposeParts)
	{
		PartOrientationPtr currentOrientation = std::get<0>(partLocation);
		Point anchorPartPoint = std::get<1>(partLocation);

		int orientationId = currentOrientation->getId();
		if (_partOrientationToColor.find(orientationId) == _partOrientationToColor.end())
		{
			float r, g, b;
			r = (rand() % 255);
			g = (rand() % 255);
			b = (rand() % 255);
			RGB_COLOR color = std::make_tuple(r, g, b);
			_partOrientationToColor[orientationId] = color;
		}

		RGB_COLOR orientationColor = _partOrientationToColor.at(currentOrientation->getId());
		float r = std::get<0>(orientationColor);
		float g = std::get<1>(orientationColor);
		float b = std::get<2>(orientationColor);
		float a;
		r /= 255.0;
		g /= 255.0;
		b /= 255.0;
		a = 1.0f;

		for (Point& relativePartCoordinate : *(currentOrientation->getPointList()))
		{
			Point point = anchorPartPoint + relativePartCoordinate;

			// Paint a square for each unmasked pixel in the world
			float v1x = point.getX();
			float v1y = world->getHeight() - point.getY(); // OpenGL Y axis is inverted
			float v2x = v1x + SQUARE_SIZE;
			float v2y = v1y;
			float v3x = v1x;
			float v3y = v1y + SQUARE_SIZE;
			float v4x = v1x + SQUARE_SIZE;
			float v4y = v1y + SQUARE_SIZE;

			_renderContext->writeTriangle(v1x, v1y, v2x, v2y, v3x, v3y, r, g, b, a);
			_renderContext->writeTriangle(v2x, v2y, v3x, v3y, v4x, v4y, r, g, b, a);

			if (currentOrientation->isContainsPointAbovePoint(relativePartCoordinate))
				_renderContext->writeLine(v1x, v1y, v2x, v2y);
			else
				_renderContext->writeBoldLine(v1x, v1y, v2x, v2y);
			
			if (currentOrientation->isContainsPointRightOfPoint(relativePartCoordinate))
				_renderContext->writeLine(v2x, v2y, v4x, v4y);
			else
				_renderContext->writeBoldLine(v2x, v2y, v4x, v4y);
			
			if (currentOrientation->isContainsPointBelowPoint(relativePartCoordinate))
				_renderContext->writeLine(v3x, v3y, v4x, v4y);
			else
				_renderContext->writeBoldLine(v3x, v3y, v4x, v4y);

			if (currentOrientation->isContainsPointLeftPoint(relativePartCoordinate))
				_renderContext->writeLine(v3x, v3y, v1x, v1y);
			else
				_renderContext->writeBoldLine(v3x, v3y, v1x, v1y);
		}
	}
}

void GLDisplayHelper::displayResults(WorldPtr world, DecomposeAndPackResult dapResults)
{
	FinalDecomposeResults decomposeResult = std::get<0>(dapResults);
	displayDecomposeResults(world, decomposeResult);
}

void GLDisplayHelper::displayDecomposeResults(WorldPtr world, FinalDecomposeResults decomposeResult)
{
	std::thread openGLThread(&GLDisplayHelper::initRenderingLoop, this);
	paintWorld(world);

	while (!_renderer.isReady())
	{
		; // Sleep TODO
	}

	_renderer.commitContext(_renderContext);
	_renderer.addInputListener(this);

	int decomposeResultsIndex = -1;

	while (true)
	{
		; // Sleep TODO

		if (_isCallbackReceived)
		{
			if (_lastCallbackKey == KEYBOARD_KEY::KEY_LEFT)
				decomposeResultsIndex--;
			if (_lastCallbackKey == KEYBOARD_KEY::KEY_RIGHT)
				decomposeResultsIndex++;

			if (decomposeResultsIndex <= -1)
				decomposeResultsIndex = -1;
			else if (decomposeResultsIndex > decomposeResult->size() - 1)
				decomposeResultsIndex--;

			_renderContext->clearDataBuffers();

			if (decomposeResultsIndex == -1)
			{
				paintWorld(world);
				string description = "World (object mask view)";
				_renderContext->setContentDescription(description);
			}
			else
			{
				paintDecomposeResults(world,
					decomposeResult->at(decomposeResultsIndex));

				string solutionIndex = std::to_string(decomposeResultsIndex + 1);
				string description = "Decompose solution #" + solutionIndex;
				_renderContext->setContentDescription(description);
			}

			_renderer.commitContext(_renderContext);

			_isCallbackReceived = false;
		}
	}

	DFPLogger::getInstance().log("Main thread quitting", DFPLogger::LogLevel::DEBUG_MODE);
}