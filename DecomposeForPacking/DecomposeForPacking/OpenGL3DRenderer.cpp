#include "OpenGL3DRenderer.h"

#include <algorithm>
#include "ShaderFactory.h"
#include "OpenGLShader.h"
#include "DFPLogger.h"
#include "DFPConfiguration.h"

static const double NO_PRESS = -1;
shared_ptr<Arcball> OpenGL3DRenderer::_arcball = std::make_shared<Arcball>(); // Static variable allocation

OpenGL3DRenderer::OpenGL3DRenderer() :
OpenGLRenderer(), _lastFrameRenderTime(0), _viewState(UNINITIALIZED)
{
}

OpenGL3DRenderer::~OpenGL3DRenderer()
{
}

#pragma region - Rendering Logic


void OpenGL3DRenderer::determineWindowSize()
{
	_windowWidth = 640;
	_windowHeight = 480;
}

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

	glfwSetCursorPosCallback(_window, cursor_position_callback);
	glfwSetMouseButtonCallback(_window, mouse_button_callback);
}

void OpenGL3DRenderer::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	_arcball->handleMouseMove(xpos, ypos);
}

void OpenGL3DRenderer::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button != GLFW_MOUSE_BUTTON_LEFT)
		return;

	bool isLeftMouseButtonPressed = ((button == GLFW_MOUSE_BUTTON_LEFT) && (action == GLFW_PRESS));

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	_arcball->handleMousePress(isLeftMouseButtonPressed, xpos, ypos);
}

shared_ptr<MATRIX_4X4> OpenGL3DRenderer::generateModelMatrix(int windowWidth, int windowHeight)
{
	VECTOR_3D worldCenter = VECTOR_3D(-_renderContext->width()*1.0f / 2,
									  -_renderContext->height()*1.0f / 2,
									  -_renderContext->depth()*1.0f / 2);
	glm::mat4 translation = glm::translate(glm::mat4(1.0f), worldCenter); // Center object around (0, 0, 0)

	// Arcball code starts here

	_arcball->updateScreenSize(windowWidth, windowHeight);

	if (_lastViewMatrix == nullptr) // Handle first frame case
		_lastViewMatrix = generateViewMatrix(windowWidth, windowHeight);

	auto rotation = _arcball->modelRotationMatrix(std::make_shared<MATRIX_4X4>(translation), _lastViewMatrix);

	// TODO: handle FPS
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - _lastFrameRenderTime);
	_lastFrameRenderTime = currentTime;

	// Arcball code ends here

	//_lastModelMatrix = std::make_shared<MATRIX_4X4>(translation * (*rotation));
	return std::make_shared<MATRIX_4X4>((*rotation) * translation);
}

shared_ptr<MATRIX_4X4> OpenGL3DRenderer::generateViewMatrix(int windowWidth, int windowHeight)
{
	if (_viewState == UNINITIALIZED)
	{
			_zoom = 1.0f;
			_viewState = READY;
	}
	else
	{ // Handle zoom change
		if (glfwGetKey(_window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS) // Zoom out
		{
			float width = _renderContext->width();
			_zoom = std::min(float(_renderContext->width() * 20), float(_zoom + 0.01));
		}
		else if (glfwGetKey(_window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS) // Zoom in
		{
			_zoom = std::max(0.1f, float(_zoom - 0.01));
		}
	}

	VECTOR_3D cameraPos =
		glm::vec3(_renderContext->width() * 3, _renderContext->height() * 3, _renderContext->depth() * 3);

	glm::mat4 view = glm::lookAt(
		_zoom * cameraPos, // Camera position in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);

	_lastViewMatrix = std::make_shared<MATRIX_4X4>(view);

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
	VECTOR_3D spotLight = VECTOR_3D(_renderContext->width()*1.0f * 1.5,
									_renderContext->height()*1.0f * 1.5,
									_renderContext->depth()*1.0f * 1.5);

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