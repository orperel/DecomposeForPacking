#pragma once
class OpenGLRenderContext
{
public:
	OpenGLRenderContext();
	virtual ~OpenGLRenderContext();

	void drawPart();

private:
	float[] _pointsVBO;
	float[] _linesVBO;
	float[] _trianglesVBO;
	float[] _trianglesStripVBO;
};

