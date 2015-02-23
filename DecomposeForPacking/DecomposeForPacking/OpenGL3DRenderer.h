#pragma once

#include "OpenGLLibs.h" // Must be first to enable OpenGL import
#include "OpenGLRenderer.h"
#include "OpenGLShaderProgram.h"
#include "OpenGL3DDefaultProgramSetup.h"

enum VIEW_STATE
{
	UNINITIALIZED,
	STANDBY_MODE, // No mouse pressed
	ROTATE_MODE // Mouse pressed
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

private:
	unique_ptr<OpenGLShaderProgram> _default3DProgram;
	unique_ptr<OpenGL3DDefaultProgramSetup> _default3DProgramSetup;

	void OpenGL3DRenderer::renderBatch(GLenum geometryType, OpenGLBatch& batch);

	// Camera control
	VIEW_STATE _viewState;
	shared_ptr<MATRIX_4X4> _lastViewMatrix;
	shared_ptr<MATRIX_4X4> _lastModelMatrix;
	shared_ptr<glm::vec3> _lastCameraPosition;
	shared_ptr<glm::mat4> _rotationMatrix;
	double _lastFrameRenderTime;
	double _pressPositionX;
	double _pressPositionY;
	shared_ptr<glm::vec3> _pressCameraPosition;
};

