#pragma once

#include "OpenGLLibs.h" // Must be first to enable OpenGL import
#include "OpenGLRenderer.h"
#include "OpenGLShaderProgram.h"
#include "OpenGL3DDefaultProgramSetup.h"
#include "Arcball.h"

enum VIEW_STATE
{
	UNINITIALIZED,
	READY, // No mouse pressed
};

class OpenGL3DRenderer : public OpenGLRenderer
{
public:
	OpenGL3DRenderer();
	virtual ~OpenGL3DRenderer();
	virtual void setup() override;
	virtual void renderFrame() override;

protected:
	OpenGLBatch _triangles3DBatch;

	// Generates the model view projection matrix
	shared_ptr<MATRIX_4X4> generateModelMatrix(int windowWidth, int windowHeight);
	shared_ptr<MATRIX_4X4> generateViewMatrix(int windowWidth, int windowHeight);
	shared_ptr<MATRIX_4X4> generateProjectionMatrix(int windowWidth, int windowHeight);
	shared_ptr<MATRIX_4X4> generateMVPMatrix(int windowWidth, int windowHeight);

	// Handle light source
	shared_ptr<VECTOR_3D> generateLightSource();

	// Buffer the geometry to the GPU
	virtual void bufferData(shared_ptr<OpenGLRenderContext> context) override;

	virtual void determineWindowSize() override;

private:
	unique_ptr<OpenGLShaderProgram> _default3DProgram;
	unique_ptr<OpenGL3DDefaultProgramSetup> _default3DProgramSetup;

	// Camera control
	VIEW_STATE _viewState;
	static shared_ptr<Arcball> _arcball;
	shared_ptr<MATRIX_4X4> _lastViewMatrix;
	double _lastFrameRenderTime;
	float _zoom;


	void renderBatch(GLenum geometryType, OpenGLBatch& batch);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
};

