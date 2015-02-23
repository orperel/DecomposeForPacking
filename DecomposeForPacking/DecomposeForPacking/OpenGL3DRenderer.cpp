#include "OpenGL3DRenderer.h"

#include <algorithm>
#include "ShaderFactory.h"
#include "OpenGLShader.h"
#include "DFPLogger.h"
#include "DFPConfiguration.h"

static const double NO_PRESS = -1;

OpenGL3DRenderer::OpenGL3DRenderer() :
OpenGLRenderer(), _lastFrameRenderTime(0), _pressPositionX(NO_PRESS), _pressPositionY(NO_PRESS), _viewState(UNINITIALIZED)
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

	_bgColor = std::make_tuple(0.0f, 0.0f, 0.0f); // Set background color to black
}

shared_ptr<MATRIX_4X4> OpenGL3DRenderer::generateModelMatrix(int windowWidth, int windowHeight)
{
	VECTOR_3D worldCenter = VECTOR_3D(-_renderContext->width()*1.0f / 2,
									  -_renderContext->height()*1.0f / 2,
									  -_renderContext->depth()*1.0f / 2);
	glm::mat4 model = glm::translate(glm::mat4(1.0f), worldCenter); // Center object around (0, 0, 0)

	_lastModelMatrix = std::make_shared<MATRIX_4X4>(model);
	return _lastModelMatrix;
}

shared_ptr<MATRIX_4X4> OpenGL3DRenderer::generateViewMatrix(int windowWidth, int windowHeight)
{
	switch (_viewState)
	{
		case (UNINITIALIZED) :
		{
			_rotationMatrix = std::make_shared<MATRIX_4X4>(1.0f);

			// Initialize to world center
			shared_ptr<VECTOR_3D> worldCenter = std::make_shared<VECTOR_3D>(_renderContext->width()*1.0f / 2,
																		    _renderContext->height()*1.0f / 2,
																		    _renderContext->depth()*1.0f / 2);

			_lastCameraPosition =
				std::make_shared<VECTOR_3D>(glm::vec3(_renderContext->width() * 3, _renderContext->height() * 3, _renderContext->depth() * 3));

			glm::mat4 view = glm::lookAt(
				*_lastCameraPosition, // Camera position in World Space
				glm::vec3(0, 0, 0), // and looks at the origin
				glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
				);

			_lastViewMatrix = std::make_shared<MATRIX_4X4>(view);
		}

		case (STANDBY_MODE) :
		{
			// Go into rotate mode
			if (glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			{
				// Get mouse position
				double xpos, ypos;
				glfwGetCursorPos(_window, &xpos, &ypos);
				_pressPositionX = xpos;
				_pressPositionY = ypos;
				_pressCameraPosition = _lastCameraPosition;
				_lastFrameRenderTime = glfwGetTime();
				_viewState = ROTATE_MODE;
			}
		}

		case (ROTATE_MODE) :
		{
			// Go back to standby mode
			if (glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
			{
				_pressPositionX = NO_PRESS;
				_pressPositionY = NO_PRESS;
				_viewState = STANDBY_MODE;
			}
			else // Perform rotate
			{
				// Get mouse position
				double xpos, ypos;
				glfwGetCursorPos(_window, &xpos, &ypos);

				// As the mouse goes away from the anchor, we update the view matrix
				double deltaX = glm::radians((_pressPositionX - xpos) / 2);
				double deltaY = glm::radians((_pressPositionY - ypos) / 2);

				_pressPositionX = xpos;
				_pressPositionY = ypos;

				double currentTime = glfwGetTime();
				float deltaTime = float(currentTime - _lastFrameRenderTime);
				_lastFrameRenderTime = currentTime;
				/*
				glm::mat3x3 rotateXTransform = glm::mat3x3(glm::vec3(1, 0, 0),
					glm::vec3(0, cos(deltaY), -sin(deltaY)),
					glm::vec3(0, sin(deltaY), cos(deltaY)));
				glm::mat3x3 rotateYTransform = glm::mat3x3(glm::vec3(cos(deltaX), 0, sin(deltaX)),
					glm::vec3(0, 1, 0),
					glm::vec3(-sin(deltaX), 0, cos(deltaX)));
				glm::mat3x3 rotateZTransform = glm::mat3x3(glm::vec3(cos(deltaX), -sin(deltaX), 0),
					glm::vec3(sin(deltaX), cos(deltaX), 0),
					glm::vec3(0, 0, 1));

				glm::vec3 cameraPosition = rotateYTransform * glm::inverse(rotateXTransform) *  (*_pressCameraPosition);
				*/
				
				MATRIX_4X4 rotationMatrix = *_rotationMatrix; 

				glm::vec3 xAxis = glm::vec3(glm::inverse(rotationMatrix) * glm::vec4(1.0f, 0.0f, 0.0f, 1.0));
				rotationMatrix = glm::rotate(rotationMatrix, float(deltaY), xAxis);
				glm::vec3 yAxis = glm::vec3(glm::inverse(rotationMatrix) * glm::vec4(0.0f, 1.0f, 0.0f, 1.0));
				rotationMatrix = glm::rotate(rotationMatrix, float(deltaX), yAxis);

				//glm::mat4 cubeInv = glm::inverse(*_lastViewMatrix * *_lastModelMatrix);
				//glm::vec4 rX = cubeInv * (*_lastViewMatrix) * glm::vec4(0.f, -1.f, 0.f, 1.f);
				//glm::vec4 rY = cubeInv * (*_lastViewMatrix) * glm::vec4(-1.f, 0.f, 0.f, 1.f);
				//rotationMatrix = glm::rotate(rotationMatrix, float(deltaY), glm::vec3(rX.x, rX.y, rX.z));
				//rotationMatrix = glm::rotate(rotationMatrix, float(deltaX), glm::vec3(rY.x, rY.y, rY.z));


				glm::vec3 cameraPosition = glm::vec3(glm::vec4((*_pressCameraPosition), 0.0f));
				_rotationMatrix = std::make_shared<MATRIX_4X4>(rotationMatrix);

				//glm::vec3 cameraPosition = *_lastCameraPosition;

				glm::mat4 view = glm::lookAt(
					cameraPosition, // Camera position in World Space
					glm::vec3(0, 0, 0), // and looks at the origin
					glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
					);

				_lastCameraPosition = std::make_shared<VECTOR_3D>(cameraPosition);
				_lastViewMatrix = std::make_shared<MATRIX_4X4>(view*rotationMatrix);
			}
		}
	}

	return _lastViewMatrix;
}

shared_ptr<MATRIX_4X4> OpenGL3DRenderer::generateProjectionMatrix(int windowWidth, int windowHeight)
{
	float screenRatio = float(_windowWidth) / float(_windowHeight);

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 projection = glm::perspective(45.0f, screenRatio, 0.1f, 100.0f);

	return std::make_shared<MATRIX_4X4>(projection);
}

shared_ptr<MATRIX_4X4> OpenGL3DRenderer::generateMVPMatrix(int windowWidth, int windowHeight)
{
	auto projection = generateProjectionMatrix(windowWidth, windowHeight);
	auto view = generateViewMatrix(windowWidth, windowHeight);
	auto model = generateModelMatrix(windowWidth, windowHeight);

	// Matrix multiplication is the other way around
	return std::make_shared<MATRIX_4X4>((*projection) * (*view) * (*model));
}

shared_ptr<VECTOR_3D> OpenGL3DRenderer::generateLightSource()
{
	glm::vec3 spotLight = glm::vec3(10.0f, 10.0f, 10.0f);
	return std::make_shared<VECTOR_3D>(spotLight);
}

void OpenGL3DRenderer::bufferData(shared_ptr<OpenGLRenderContext> context)
{
	// Happens asynchronically to renderFrame --
	// Data is buffered to openGL buffers, so the next render will capture this new data.
	// This actually implements double buffering as well.
	_triangles3DBatch.bind();
	_triangles3DBatch.bufferData(context->triangles3D());
}

void OpenGL3DRenderer::renderBatch(GLenum geometryType, OpenGLBatch& batch)
{
	batch.bind();

	shared_ptr<MATRIX_4X4> modelMatrix = generateModelMatrix(_windowWidth, _windowHeight);
	shared_ptr<MATRIX_4X4> viewMatrix = generateViewMatrix(_windowWidth, _windowHeight);
	shared_ptr<MATRIX_4X4> mvpMatrix = generateMVPMatrix(_windowWidth, _windowHeight);
	shared_ptr<VECTOR_3D> lightSourcePos = generateLightSource();
	_default3DProgramSetup->setUniformValues(*mvpMatrix, *modelMatrix, *viewMatrix, *lightSourcePos);
	_default3DProgramSetup->setup();
	glDrawArrays(geometryType, 0, batch.numOfVertices());

	batch.unbind();
	_default3DProgramSetup->tearDown();
}

void OpenGL3DRenderer::renderFrame()
{
	// Enable Z buffer for 3d z-order effecrt
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL); // Depth function set to less or equal

	// Clear Z buffer buffer
	glClearDepth(1.0);
	glClear(GL_DEPTH_BUFFER_BIT);

	// Enable alpha blend for transparency effect
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	_default3DProgram->use();

	if (_triangles3DBatch.sizeInBytes() > 0)
		renderBatch(GL_TRIANGLES, _triangles3DBatch);

	_default3DProgram->disable();
	_default3DProgramSetup->tearDown();
}