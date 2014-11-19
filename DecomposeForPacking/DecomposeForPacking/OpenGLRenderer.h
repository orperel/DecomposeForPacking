#pragma once

class OpenGLRenderer
{
public:
	OpenGLRenderer();
	virtual ~OpenGLRenderer();

	int initRenderingLoop();

	void renderFrame();
};