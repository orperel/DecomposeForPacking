#pragma once

#include "OpenGLLibs.h" // Must be first to enable OpenGL import
#include "OpenGLRenderContext.h"
#include <memory>
#include <vector>

#include "OpenGLBatch.h"
#include "IInputListener.h"

using std::unique_ptr;
using std::vector;

/** Rendering engine abstract code common for 2d & 3d */
class OpenGLRenderer
{
public:
	OpenGLRenderer();
	virtual ~OpenGLRenderer();

	int initRenderingLoop();

	// Load shaders and initialize batches according to 2d or 3d
	virtual void setup() = 0;
	// Render the GPU buffered geometry for the current frame
	virtual void renderFrame() = 0;
	void commitContext(shared_ptr<OpenGLRenderContext> context);

	void addInputListener(IInputListener* listener);

	const bool isReady() const { return _isReady; }

protected:
	// Buffer the geometry to the GPU
	virtual void bufferData(shared_ptr<OpenGLRenderContext> context) = 0;

	shared_ptr<OpenGLRenderContext> _renderContext;
	int _windowWidth;
	int _windowHeight;

private:
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void updateScene(shared_ptr<OpenGLRenderContext> context, GLFWwindow* window);

	bool _isReady;
	bool _isDirty;

	// Keys callback
	static vector<IInputListener*> _inputListeners;
};