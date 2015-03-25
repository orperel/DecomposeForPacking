#pragma once

#include "OpenGLLibs.h" // Must be first to enable OpenGL import
#include "ExpandableBuffer.h"
#include <string>

using std::string;

/** ------------------------------------------------------------------------ */
/** Buffer structs should match their counter-part attributes in the shaders */
/** ------------------------------------------------------------------------ */
struct Colored2DVertexEntry
{
	GLfloat x, y;
	GLfloat colorR, colorG, colorB, colorA;
};

struct Colored3DVertexEntry
{
	GLfloat x, y, z;
	GLfloat nx, ny, nz; // Joint surface normals sum - normalized
	GLfloat colorR, colorG, colorB, colorA;
};

/* The "painting board" used by the application to specify which info should be drawn by OpenGL
 */
class OpenGLRenderContext
{	
public:
	OpenGLRenderContext(int width, int height, int pixelResolution);
	OpenGLRenderContext(int width, int height, int depth, int pixelResolution);
	virtual ~OpenGLRenderContext();

	void clearDataBuffers();

	void writePoint(float x, float y);
	void writeLine(float xStart, float yStart, float xEnd, float yEnd);
	void writeBoldLine(float v1X, float v1Y, float v2X, float v2Y);
	void write2DTriangle(float v1X, float v1Y,
		                 float v2X, float v2Y,
						 float v3X, float v3Y, 
						 float r, float g, float b, float a);
	void write3DTriangle(Colored3DVertexEntry& v1,
						 Colored3DVertexEntry& v2,
						 Colored3DVertexEntry& v3);

	ExpandableBuffer<Colored2DVertexEntry>& points() { return _pointsVBO; };
	ExpandableBuffer<Colored2DVertexEntry>& lines() { return _linesVBO; };
	ExpandableBuffer<Colored2DVertexEntry>& boldLines() { return _boldLinesVBO; };
	ExpandableBuffer<Colored2DVertexEntry>& triangles2D() { return _triangles2DVBO; };
	ExpandableBuffer<Colored3DVertexEntry>& triangles3D() { return _triangles3DVBO; };
	const int width() const { return _maxX - _minX; };
	const int height() const { return _maxY - _minY; };
	const int depth() const { return _maxZ - _minZ; };
	const int worldWidth() const { return _worldWidth; };
	const int worldHeight() const { return _worldHeight; };
	const int worldDepth() const { return _worldDepth; };
	const int pixelResolution() const { return _pixelResolution; };
	const void setContentDescription(string& description) { _contentDescription = description; };
	const string contentDescription() const { return _contentDescription; };
private: 
	ExpandableBuffer<Colored2DVertexEntry> _pointsVBO;
	ExpandableBuffer<Colored2DVertexEntry> _linesVBO;
	ExpandableBuffer<Colored2DVertexEntry> _boldLinesVBO;
	ExpandableBuffer<Colored2DVertexEntry> _triangles2DVBO;
	ExpandableBuffer<Colored3DVertexEntry> _triangles3DVBO;

	int _pixelResolution;
	string _contentDescription;
	int _minX, _minY, _minZ, _maxX, _maxY, _maxZ;
	int _worldWidth, _worldHeight, _worldDepth;
};

