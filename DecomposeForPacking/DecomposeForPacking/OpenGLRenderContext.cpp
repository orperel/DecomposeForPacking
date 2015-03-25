#include "OpenGLRenderContext.h"
#include <algorithm>

static const int MIN_INIT = 99999999;
static const int MAX_INIT = -99999999;

OpenGLRenderContext::OpenGLRenderContext(int width, int height, int pixelResolution) :
OpenGLRenderContext(width, height, 0, pixelResolution)
{
}

OpenGLRenderContext::OpenGLRenderContext(int width, int height, int depth, int pixelResolution) :
_minX(MIN_INIT), _minY(MIN_INIT), _minZ(MIN_INIT),
_maxX(MAX_INIT), _maxY(MAX_INIT), _maxZ(MAX_INIT), _pixelResolution(pixelResolution),
_worldWidth(width), _worldHeight(height), _worldDepth(depth)
{
}


OpenGLRenderContext::~OpenGLRenderContext()
{
}

void OpenGLRenderContext::clearDataBuffers()
{
	_pointsVBO.reset();
	_linesVBO.reset();
	_boldLinesVBO.reset();
	_triangles2DVBO.reset();
	_triangles3DVBO.reset();

	_minX = MIN_INIT;
	_minY = MIN_INIT;
	_minZ = MIN_INIT;
	_maxX = MAX_INIT;
	_maxY = MAX_INIT;
	_maxZ = MAX_INIT;
}

void OpenGLRenderContext::writePoint(float x, float y)
{
	Colored2DVertexEntry bufferEntry;
	bufferEntry.x = x;
	bufferEntry.y = y;
	bufferEntry.colorR = 1.0;
	bufferEntry.colorG = 1.0;
	bufferEntry.colorB = 1.0;
	bufferEntry.colorA = 1.0;
	_pointsVBO.write(bufferEntry);

	_minX = std::min(_minX, int(x));
	_maxX = std::max(_maxX, int(x));
	_minY = std::min(_minY, int(y));
	_maxY = std::max(_maxY, int(y));
}

void OpenGLRenderContext::writeLine(float v1X, float v1Y, float v2X, float v2Y)
{
	Colored2DVertexEntry bufferEntry1;
	bufferEntry1.x = v1X;
	bufferEntry1.y = v1Y;
	bufferEntry1.colorR = 0.0;
	bufferEntry1.colorG = 0.0;
	bufferEntry1.colorB = 0.0;
	bufferEntry1.colorA = 1.0;

	Colored2DVertexEntry bufferEntry2;
	bufferEntry2.x = v2X;
	bufferEntry2.y = v2Y;
	bufferEntry2.colorR = 0.0;
	bufferEntry2.colorG = 0.0;
	bufferEntry2.colorB = 0.0;
	bufferEntry2.colorA = 1.0;

	_linesVBO.write(bufferEntry1);
	_linesVBO.write(bufferEntry2);

	_minX = std::min(_minX, int(v1X));
	_maxX = std::max(_maxX, int(v1X));
	_minY = std::min(_minY, int(v1Y));
	_maxY = std::max(_maxY, int(v1Y));
	_minX = std::min(_minX, int(v2X));
	_maxX = std::max(_maxX, int(v2X));
	_minY = std::min(_minY, int(v2Y));
	_maxY = std::max(_maxY, int(v2Y));
}

void OpenGLRenderContext::writeBoldLine(float v1X, float v1Y, float v2X, float v2Y)
{
	Colored2DVertexEntry bufferEntry1;
	bufferEntry1.x = v1X;
	bufferEntry1.y = v1Y;
	bufferEntry1.colorR = 0.0;
	bufferEntry1.colorG = 0.0;
	bufferEntry1.colorB = 0.0;
	bufferEntry1.colorA = 1.0;

	Colored2DVertexEntry bufferEntry2;
	bufferEntry2.x = v2X;
	bufferEntry2.y = v2Y;
	bufferEntry2.colorR = 0.0;
	bufferEntry2.colorG = 0.0;
	bufferEntry2.colorB = 0.0;
	bufferEntry2.colorA = 1.0;

	_boldLinesVBO.write(bufferEntry1);
	_boldLinesVBO.write(bufferEntry2);

	_minX = std::min(_minX, int(v1X));
	_maxX = std::max(_maxX, int(v1X));
	_minY = std::min(_minY, int(v1Y));
	_maxY = std::max(_maxY, int(v1Y));
	_minX = std::min(_minX, int(v2X));
	_maxX = std::max(_maxX, int(v2X));
	_minY = std::min(_minY, int(v2Y));
	_maxY = std::max(_maxY, int(v2Y));
}

void OpenGLRenderContext::write2DTriangle(float v1X, float v1Y, float v2X, float v2Y, float v3X, float v3Y,
										  float r, float g, float b, float a)
{
	Colored2DVertexEntry bufferEntry1;
	bufferEntry1.x = v1X;
	bufferEntry1.y = v1Y;
	bufferEntry1.colorR = r;
	bufferEntry1.colorG = g;
	bufferEntry1.colorB = b;
	bufferEntry1.colorA = a;

	Colored2DVertexEntry bufferEntry2;
	bufferEntry2.x = v2X;
	bufferEntry2.y = v2Y;
	bufferEntry2.colorR = r;
	bufferEntry2.colorG = g;
	bufferEntry2.colorB = b;
	bufferEntry2.colorA = a;

	Colored2DVertexEntry bufferEntry3;
	bufferEntry3.x = v3X;
	bufferEntry3.y = v3Y;
	bufferEntry3.colorR = r;
	bufferEntry3.colorG = g;
	bufferEntry3.colorB = b;
	bufferEntry3.colorA = a;

	_triangles2DVBO.write(bufferEntry1);
	_triangles2DVBO.write(bufferEntry2);
	_triangles2DVBO.write(bufferEntry3);

	_minX = std::min(_minX, int(v1X));
	_maxX = std::max(_maxX, int(v1X));
	_minY = std::min(_minY, int(v1Y));
	_maxY = std::max(_maxY, int(v1Y));
	_minX = std::min(_minX, int(v2X));
	_maxX = std::max(_maxX, int(v2X));
	_minY = std::min(_minY, int(v2Y));
	_maxY = std::max(_maxY, int(v2Y));
	_minX = std::min(_minX, int(v3X));
	_maxX = std::max(_maxX, int(v3X));
	_minY = std::min(_minY, int(v3Y));
	_maxY = std::max(_maxY, int(v3Y));
}

void OpenGLRenderContext::write3DTriangle(Colored3DVertexEntry& v1,
										  Colored3DVertexEntry& v2,
										  Colored3DVertexEntry& v3)
{
	_triangles3DVBO.write(v1);
	_triangles3DVBO.write(v2);
	_triangles3DVBO.write(v3);

	_minX = std::min(_minX, int(v1.x));
	_maxX = std::max(_maxX, int(v1.x));
	_minY = std::min(_minY, int(v1.y));
	_maxY = std::max(_maxY, int(v1.y));
	_minZ = std::min(_minZ, int(v1.z));
	_maxZ = std::max(_maxZ, int(v1.z));
	_minX = std::min(_minX, int(v2.x));
	_maxX = std::max(_maxX, int(v2.x));
	_minY = std::min(_minY, int(v2.y));
	_maxY = std::max(_maxY, int(v2.y));
	_minZ = std::min(_minZ, int(v2.z));
	_maxZ = std::max(_maxZ, int(v2.z));
	_minX = std::min(_minX, int(v3.x));
	_maxX = std::max(_maxX, int(v3.x));
	_minY = std::min(_minY, int(v3.y));
	_maxY = std::max(_maxY, int(v3.y));
	_minZ = std::min(_minZ, int(v3.z));
	_maxZ = std::max(_maxZ, int(v3.z));
}