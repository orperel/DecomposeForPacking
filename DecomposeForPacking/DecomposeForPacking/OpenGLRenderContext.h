#pragma once

#include "OpenGLLibs.h" // Must be first to enable OpenGL import
#include "ExpandableBuffer.h"
#include <string>

using std::string;

/** ------------------------------------------------------------------------ */
/** Buffer structs should match their counter-part attributes in the shaders */
/** ------------------------------------------------------------------------ */
struct ColoredVertexEntry
{
	GLfloat x, y;
	GLfloat colorR, colorG, colorB, colorA;
};

/* The "painting board" used by the application to specify which info should be drawn by OpenGL
 */
class OpenGLRenderContext
{	
public:
	OpenGLRenderContext(int width, int height, int pixelResolution);
	virtual ~OpenGLRenderContext();

	void clearDataBuffers();

	void writePoint(float x, float y);
	void writeLine(float xStart, float yStart, float xEnd, float yEnd);
	void writeBoldLine(float v1X, float v1Y, float v2X, float v2Y);
	void writeTriangle(float v1X, float v1Y, float v2X, float v2Y, float v3X, float v3Y, float r, float g, float b, float a);

	ExpandableBuffer<ColoredVertexEntry>& points() { return _pointsVBO; };
	ExpandableBuffer<ColoredVertexEntry>& lines() { return _linesVBO; };
	ExpandableBuffer<ColoredVertexEntry>& boldLines() { return _boldLinesVBO; };
	ExpandableBuffer<ColoredVertexEntry>& triangles() { return _trianglesVBO; };
	const int width() const { return _width; };
	const int height() const { return _height; };
	const int pixelResolution() const { return _pixelResolution; };
	const void setContentDescription(string& description) { _contentDescription = description; };
	const string contentDescription() const { return _contentDescription; };
private: 
	ExpandableBuffer<ColoredVertexEntry> _pointsVBO;
	ExpandableBuffer<ColoredVertexEntry> _linesVBO;
	ExpandableBuffer<ColoredVertexEntry> _boldLinesVBO;
	ExpandableBuffer<ColoredVertexEntry> _trianglesVBO;

	int _width;
	int _height;
	int _pixelResolution;
	string _contentDescription;
};

