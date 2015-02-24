#define RENDERER_DEBUG_MODE

#include "OpenGLRenderer.h"
#include <stdio.h>  
#include <stdlib.h>  
#include "DFPLogger.h"
#include "DFPConfiguration.h"

vector<IInputListener*> OpenGLRenderer::_inputListeners;
int OpenGLRenderer::_windowWidth;
int OpenGLRenderer::_windowHeight;

OpenGLRenderer::OpenGLRenderer() : _isReady(false), _isDirty(false)
{

}


OpenGLRenderer::~OpenGLRenderer()
{
}

void OpenGLRenderer::addInputListener(IInputListener* listener)
{
	_inputListeners.push_back(listener);
}

#pragma region - Rendering Logic

void OpenGLRenderer::commitContext(shared_ptr<OpenGLRenderContext> context)
{
	_isDirty = true;
	_renderContext = context;
}

void OpenGLRenderer::updateScene(shared_ptr<OpenGLRenderContext> context, GLFWwindow* window)
{
	if (!_isDirty || (context == NULL))
		return;

	// Happens asynchronically to renderFrame --
	// Data is buffered to openGL buffers, so the next render will capture this new data.
	// This actually implements double buffering as well.
	bufferData(context); // According to 2d / 3d

	glfwSetWindowTitle(window, _renderContext->contentDescription().c_str());

	_isDirty = false;
}

#pragma region - GLFW service methods

/** Define an error callback for GLFW to call on errors */
static void error_callback(int error, const char* description)
{
	string message(description, strnlen(description, 4096));
	DFPLogger::getInstance().log(message, DFPLogger::ERROR);
}

/** Define the key input callback. Events hooked by GLFW can be delegated from here. */
void OpenGLRenderer::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	KEYBOARD_KEY keyPressed = KEYBOARD_KEY::UNSUPPORTED;

	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		keyPressed = KEYBOARD_KEY::KEY_RIGHT;
	else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		keyPressed = KEYBOARD_KEY::KEY_LEFT;

	if (keyPressed != KEYBOARD_KEY::UNSUPPORTED)
	{
		for (auto& listener : _inputListeners)
		{
			listener->onKeyPressed(keyPressed);
		}
	}
}

/** Updates the viewport of the current window (so OpenGL knows how to mao [-1,1] coordinates
 *  to the correct window size)
 */
void OpenGLRenderer::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	_windowWidth = width;
	_windowHeight = height;
	glViewport(0, 0, width, height);
}

int OpenGLRenderer::initRenderingLoop(shared_ptr<OpenGLRenderContext> context)
{
	//Set the error callback for GLFW
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW  
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional  
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version  
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version  
	//glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing  
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  

	_renderContext = context; // For world dimensions
	determineWindowSize();

	// Create a window and create its OpenGL context  
	_window = glfwCreateWindow(_windowWidth, _windowHeight, "World (object mask view)", NULL, NULL);

	//If the window couldn't be created  
	if (!_window)
	{
		DFPLogger::getInstance().log("Failed to open GLFW window", DFPLogger::ERROR);
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread.   
	glfwMakeContextCurrent(_window);

	//Sets the key callback  
	glfwSetKeyCallback(_window, key_callback);

	//Window resize callback (frame buffer is the active area, excluding the title bar, etc)
	glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);

	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK)
	{
		const char* cerrMessageGlew = "Error initializing GLEW: %s", glewGetErrorString(err);
		string errMessageGlew(cerrMessageGlew, strnlen(cerrMessageGlew, 4096));
		DFPLogger::getInstance().log(errMessageGlew, DFPLogger::ERROR);
		exit(EXIT_FAILURE);
	}

	// Load shaders and create buffers needed
	setup(); // Do according to 2d / 3d mode

	// Renderer is ready to accept data
	_isReady = true;

	// Set a background color  
	glClearColor(std::get<0>(_bgColor), std::get<1>(_bgColor), std::get<2>(_bgColor), 1.0f);

	// Main Loop  
	do
	{
		// Buffer the render context if it has been changed
		updateScene(_renderContext, _window);

		#ifdef RENDERER_DEBUG_MODE
			GLenum errorCode = glGetError();
			
			if (errorCode != GL_NO_ERROR)
			{
				string errorMsg = "OpenGL error occured with code: " + std::to_string(errorCode);
				DFPLogger::getInstance().log(errorMsg, DFPLogger::ERROR);
			}
		#endif

		// Clear color buffer  
		glClear(GL_COLOR_BUFFER_BIT);

		renderFrame();

		// Swap buffers  
		glfwSwapBuffers(_window);

		// Get and organize events, like keyboard and mouse input, window resizing, etc...  
		glfwPollEvents();

	} //Check if the ESC key had been pressed or if the window had been closed  
	while (!glfwWindowShouldClose(_window));

	//Close OpenGL window and terminate GLFW  
	glfwDestroyWindow(_window);
	//Finalize and clean up GLFW  
	glfwTerminate();

	exit(EXIT_SUCCESS);
}
