#include "OpenGLRenderContext.h"

OpenGLRenderContext::OpenGLRenderContext(int width, int height, int pixelResolution) :
OpenGLRenderContext(width, height, 0, pixelResolution)
{
}

OpenGLRenderContext::OpenGLRenderContext(int width, int height, int depth, int pixelResolution) :
_width(width), _height(height), _depth(depth), _pixelResolution(pixelResolution)
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
}

void OpenGLRenderContext::write3DTriangle(Colored3DVertexEntry& v1,
										  Colored3DVertexEntry& v2,
										  Colored3DVertexEntry& v3)
{
	_triangles3DVBO.write(v1);
	_triangles3DVBO.write(v2);
	_triangles3DVBO.write(v3);
}