#pragma once

#include "OpenGLRenderer.h"
#include "OpenGLShaderProgram.h"
#include "OpenGL2DDefaultProgramSetup.h"

class OpenGL2DRenderer : public OpenGLRenderer
{
public:
	OpenGL2DRenderer();
	virtual ~OpenGL2DRenderer();

	virtual void setup() override;
	virtual void renderFrame() override;
protected:
	OpenGLBatch _pointsBatch;
	OpenGLBatch _linesBatch;
	OpenGLBatch _boldLinesBatch;
	OpenGLBatch _triangles2DBatch;

	// Generates the model view projection matrix
	shared_ptr<MATRIX_4X4> generateMVPMatrix(int windowWidth, int windowHeight);

	// Buffer the geometry to the GPU
	virtual void bufferData(shared_ptr<OpenGLRenderContext> context) override;

private:
	unique_ptr<OpenGLShaderProgram> _default2DProgram;
	unique_ptr<OpenGL2DDefaultProgramSetup> _default2DProgramSetup;

	void renderBatch(GLenum geometryType, OpenGLBatch& batch);
};

