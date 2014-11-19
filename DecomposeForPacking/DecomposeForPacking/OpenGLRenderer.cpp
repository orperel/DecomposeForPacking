#include "OpenGLRenderer.h"

#include "OpenGLLibs.h" // Must be first to enable OpenGL import
#include <stdio.h>  
#include <stdlib.h>  
#include "DFPLogger.h"

OpenGLRenderer::OpenGLRenderer()
{
}


OpenGLRenderer::~OpenGLRenderer()
{
}

#pragma region - Rendering Logic

void OpenGLRenderer::renderFrame()
{

}

#pragma region - GLFW service methods

/** Define an error callback for GLFW to call on errors */
static void error_callback(int error, const char* description)
{
	string message(description, strnlen(description, 4096));
	DFPLogger::getInstance().log(message, DFPLogger::ERROR);
}

/** Define the key input callback. Events hooked by GLFW can be delegated from here. */
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int OpenGLRenderer::initRenderingLoop()
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

	// Declare a window object  
	GLFWwindow* window;

	// Create a window and create its OpenGL context  
	window = glfwCreateWindow(640, 480, "Decompose for Packing", NULL, NULL);

	//If the window couldn't be created  
	if (!window)
	{
		DFPLogger::getInstance().log("Failed to open GLFW window", DFPLogger::ERROR);
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread.   
	glfwMakeContextCurrent(window);

	//Sets the key callback  
	glfwSetKeyCallback(window, key_callback);

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

	//Set a background color  
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//Main Loop  
	do
	{
		// Clear color buffer  
		glClear(GL_COLOR_BUFFER_BIT);

		// Render the given geometry
		renderFrame();

		// Swap buffers  
		glfwSwapBuffers(window);

		// Get and organize events, like keyboard and mouse input, window resizing, etc...  
		glfwPollEvents();

	} //Check if the ESC key had been pressed or if the window had been closed  
	while (!glfwWindowShouldClose(window));

	//Close OpenGL window and terminate GLFW  
	glfwDestroyWindow(window);
	//Finalize and clean up GLFW  
	glfwTerminate();

	exit(EXIT_SUCCESS);
}
