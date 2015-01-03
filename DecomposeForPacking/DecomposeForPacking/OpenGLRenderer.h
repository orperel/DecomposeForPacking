#pragma once

#include "OpenGLLibs.h" // Must be first to enable OpenGL import
#include "OpenGLRenderContext.h"
#include <memory>
#include <vector>

#include "OpenGLShaderProgram.h"
#include "OpenGL2DDefaultProgramSetup.h"
#include "OpenGLBatch.h"

#include "IInputListener.h"

using std::unique_ptr;
using std::vector;

class OpenGLRenderer
{
public:
	OpenGLRenderer();
	virtual ~OpenGLRenderer();

	int initRenderingLoop();

	void setup();
	void commitContext(shared_ptr<OpenGLRenderContext> context);
	void renderBatch(GLenum geometryType, OpenGLBatch& batch);
	void renderFrame();

	void addInputListener(IInputListener* listener);

	const bool isReady() const { return _isReady; }
private:
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void updateScene(shared_ptr<OpenGLRenderContext> context, GLFWwindow* window);
	shared_ptr<MATRIX_4X4> generateMVPMatrix(); // Generates the model view projection matrix

	unique_ptr<OpenGLShaderProgram> _default2DProgram;
	unique_ptr<OpenGL2DDefaultProgramSetup> _default2DProgramSetup;
	OpenGLBatch _pointsBatch;
	OpenGLBatch _linesBatch;
	OpenGLBatch _boldLinesBatch;
	OpenGLBatch _trianglesBatch;
	shared_ptr<OpenGLRenderContext> _renderContext;
	bool _isReady;
	bool _isDirty;

	// Keys callback
	static vector<IInputListener*> _inputListeners;
};