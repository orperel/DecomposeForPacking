#include "OpenGL3DRenderer.h"

#include "ShaderFactory.h"
#include "OpenGLShader.h"
#include "DFPLogger.h"
#include "DFPConfiguration.h"

OpenGL3DRenderer::OpenGL3DRenderer() : OpenGLRenderer()
{
}

OpenGL3DRenderer::~OpenGL3DRenderer()
{
}

#pragma region - Rendering Logic

void OpenGL3DRenderer::setup()
{
	// Load shaders
	unique_ptr<OpenGLShader> vertexShader = ShaderFactory::loadShaderFromFile(GL_VERTEX_SHADER,
		DEFAULT_3D_VERTSHADER_PATH);

	unique_ptr<OpenGLShader> fragmentShader = ShaderFactory::loadShaderFromFile(GL_FRAGMENT_SHADER,
		DEFAULT_3D_FRAGSHADER_PATH);

	_default3DProgram = unique_ptr<OpenGLShaderProgram>(new OpenGLShaderProgram(std::move(vertexShader), std::move(fragmentShader)));
	_default3DProgramSetup = unique_ptr<OpenGL3DDefaultProgramSetup>(new OpenGL3DDefaultProgramSetup());
	_default3DProgramSetup->createAttributes(*(_default3DProgram.get()));
	_default3DProgram->link();
	_default3DProgramSetup->createUniforms(*(_default3DProgram.get()));


	// Generate VertexArrayBuffers and VertexBufferObjects in OpenGL
	_triangles3DBatch.generate();
}

shared_ptr<MATRIX_4X4> OpenGL3DRenderer::generateMVPMatrix(int windowWidth, int windowHeight)
{
	// TODO: Replace with 3d projection
	float scale = 0.5;
	float screen_width = _renderContext->width();
	float screen_height = _renderContext->height();
	glm::mat4 projection = glm::ortho(0.0f, 1.0f*screen_height / scale, 0.0f, 1.0f*screen_width / scale);
	return std::make_shared<MATRIX_4X4>(projection);
}

void OpenGL3DRenderer::bufferData(shared_ptr<OpenGLRenderContext> context)
{
	// Happens asynchronically to renderFrame --
	// Data is buffered to openGL buffers, so the next render will capture this new data.
	// This actually implements double buffering as well.
	_triangles3DBatch.bind();
	_triangles3DBatch.bufferData(context->triangles2D());
}

void OpenGL3DRenderer::renderBatch(GLenum geometryType, OpenGLBatch& batch)
{
	batch.bind();

	shared_ptr<MATRIX_4X4> mvpMatrix = generateMVPMatrix(_windowWidth, _windowHeight);
	_default3DProgramSetup->setUniformValues(*mvpMatrix);
	_default3DProgramSetup->setup();
	glDrawArrays(geometryType, 0, batch.numOfVertices());

	batch.unbind();
	_default3DProgramSetup->tearDown();
}

void OpenGL3DRenderer::renderFrame()
{
	_default3DProgram->use();

	if (_triangles3DBatch.sizeInBytes() > 0)
		renderBatch(GL_TRIANGLES, _triangles3DBatch);

	_default3DProgram->disable();
	_default3DProgramSetup->tearDown();
}