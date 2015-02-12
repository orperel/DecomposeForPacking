#include "GL2DDisplayHelper.h"
#include "OpenGL2DRenderer.h"

static const float SQUARE_SIZE = 1;

GL2DDisplayHelper::GL2DDisplayHelper() : AbstractGLDisplayHelper()
{
	_renderer = unique_ptr<OpenGL2DRenderer>(new OpenGL2DRenderer());
}


GL2DDisplayHelper::~GL2DDisplayHelper()
{
}

GL2DDisplayHelper::WorldDisplayVisitor::WorldDisplayVisitor(shared_ptr<OpenGLRenderContext> renderContext) :
_renderContext(renderContext)
{
}

void GL2DDisplayHelper::WorldDisplayVisitor::visit(World& world, Point point)
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

	_renderContext->write2DTriangle(v1x, v1y, v2x, v2y, v3x, v3y, r, g, b, a);
	_renderContext->write2DTriangle(v2x, v2y, v3x, v3y, v4x, v4y, r, g, b, a);

	_renderContext->writeLine(v1x, v1y, v2x, v2y);
	_renderContext->writeLine(v2x, v2y, v4x, v4y);
	_renderContext->writeLine(v3x, v3y, v4x, v4y);
	_renderContext->writeLine(v3x, v3y, v1x, v1y);
}

void GL2DDisplayHelper::paintSquare(WorldPtr world, float x, float y, float r, float g, float b, float a)
{
	// TODO: Fill
}

void GL2DDisplayHelper::paintWorld(WorldPtr world)
{
	int height = world->getHeight();
	int width = world->getWidth();
	int pixelResolution = world->getPixelResolution();
	_renderContext = std::make_shared<OpenGLRenderContext>(width, height, pixelResolution);

	// Paint the world pixels to the context
	auto worldPainter = std::make_shared<WorldDisplayVisitor>(_renderContext);
	world->accept(worldPainter);
}

void GL2DDisplayHelper::paintSingleSolution(WorldPtr world, PartLocationListPtr parts)
{
	for (tuple<PartOrientationPtr, Point>& partLocation : *parts)
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

			_renderContext->write2DTriangle(v1x, v1y, v2x, v2y, v3x, v3y, r, g, b, a);
			_renderContext->write2DTriangle(v2x, v2y, v3x, v3y, v4x, v4y, r, g, b, a);

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