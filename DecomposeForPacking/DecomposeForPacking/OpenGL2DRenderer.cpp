#include "OpenGL2DRenderer.h"

#include "ShaderFactory.h"
#include "OpenGLShader.h"
#include "DFPLogger.h"
#include "DFPConfiguration.h"

OpenGL2DRenderer::OpenGL2DRenderer() : OpenGLRenderer()
{
}


OpenGL2DRenderer::~OpenGL2DRenderer()
{
}

#pragma region - Rendering Logic

void OpenGL2DRenderer::setup()
{
	// Load shaders
	unique_ptr<OpenGLShader> vertexShader = ShaderFactory::loadShaderFromFile(GL_VERTEX_SHADER,
		DEFAULT_2D_VERTSHADER_PATH);

	unique_ptr<OpenGLShader> fragmentShader = ShaderFactory::loadShaderFromFile(GL_FRAGMENT_SHADER,
		DEFAULT_2D_FRAGSHADER_PATH);

	_default2DProgram = unique_ptr<OpenGLShaderProgram>(new OpenGLShaderProgram(std::move(vertexShader), std::move(fragmentShader)));
	_default2DProgramSetup = unique_ptr<OpenGL2DDefaultProgramSetup>(new OpenGL2DDefaultProgramSetup());
	_default2DProgramSetup->createAttributes(*(_default2DProgram.get()));
	_default2DProgram->link();
	_default2DProgramSetup->createUniforms(*(_default2DProgram.get()));


	// Generate VertexArrayBuffers and VertexBufferObjects in OpenGL
	_pointsBatch.generate();
	_linesBatch.generate();
	_boldLinesBatch.generate();
	_triangles2DBatch.generate();
}

shared_ptr<MATRIX_4X4> OpenGL2DRenderer::generateMVPMatrix(int windowWidth, int windowHeight)
{
	float scale = 0.5;
	float screen_width = _renderContext->width();
	float screen_height = _renderContext->height();
	glm::mat4 projection = glm::ortho(0.0f, 1.0f*screen_height / scale, 0.0f, 1.0f*screen_width / scale);
	return std::make_shared<MATRIX_4X4>(projection);

	/*
	//float scale = glutGet(GLUT_ELAPSED_TIME) / 1000.0 * .2;  // 20% per second
	float scale = 1;
	float screen_width = _renderContext->width() + 1;
	float screen_height = _renderContext->height() + 1;

	float aspectRatio = (1.0f*windowHeight) / (1.0f*windowWidth);

	// Object to world matrix
	glm::mat4 model = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 0.0f)) *
	glm::scale(glm::mat4(), glm::vec3(1.0f, 1.0f, 1.0f));

	glm::mat4 projection = glm::ortho(0.0f, 1.0f*screen_width * 4 / 3 / scale, 0.0f, 1.0f*screen_height/aspectRatio* 4 / 3 / scale);
	return std::make_shared<MATRIX_4X4>(model*projection);
	*/
}

void OpenGL2DRenderer::bufferData(shared_ptr<OpenGLRenderContext> context)
{
	// Happens asynchronically to renderFrame --
	// Data is buffered to openGL buffers, so the next render will capture this new data.
	// This actually implements double buffering as well.
	_triangles2DBatch.bind();
	_triangles2DBatch.bufferData(context->triangles2D());
	_linesBatch.bind();
	_linesBatch.bufferData(context->lines());
	_boldLinesBatch.bind();
	_boldLinesBatch.bufferData(context->boldLines());
	_pointsBatch.bind();
	_pointsBatch.bufferData(context->points());
}

void OpenGL2DRenderer::renderBatch(GLenum geometryType, OpenGLBatch& batch)
{
	batch.bind();

	shared_ptr<MATRIX_4X4> mvpMatrix = generateMVPMatrix(_windowWidth, _windowHeight);
	_default2DProgramSetup->setUniformValues(*mvpMatrix);
	_default2DProgramSetup->setup();
	glDrawArrays(geometryType, 0, batch.numOfVertices());

	batch.unbind();
	_default2DProgramSetup->tearDown();
}

void OpenGL2DRenderer::renderFrame()
{
	_default2DProgram->use();

	if (_triangles2DBatch.sizeInBytes() > 0)
		renderBatch(GL_TRIANGLES, _triangles2DBatch);

	if (_boldLinesBatch.sizeInBytes() > 0)
	{
		glLineWidth(3.0f);
		renderBatch(GL_LINES, _boldLinesBatch);
	}

	if (_linesBatch.sizeInBytes() > 0)
	{
		glLineWidth(1.0f);
		renderBatch(GL_LINES, _linesBatch);
	}

	if (_pointsBatch.sizeInBytes() > 0)
		renderBatch(GL_POINTS, _pointsBatch);

	_default2DProgram->disable();
}