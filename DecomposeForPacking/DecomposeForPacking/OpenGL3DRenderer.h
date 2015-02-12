#pragma once
#include "OpenGLRenderer.h"
#include "OpenGLShaderProgram.h"
#include "OpenGL3DDefaultProgramSetup.h"

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
	shared_ptr<MATRIX_4X4> generateMVPMatrix(int windowWidth, int windowHeight);

	// Buffer the geometry to the GPU
	virtual void bufferData(shared_ptr<OpenGLRenderContext> context) override;

private:
	unique_ptr<OpenGLShaderProgram> _default3DProgram;
	unique_ptr<OpenGL3DDefaultProgramSetup> _default3DProgramSetup;

	void OpenGL3DRenderer::renderBatch(GLenum geometryType, OpenGLBatch& batch);
};

