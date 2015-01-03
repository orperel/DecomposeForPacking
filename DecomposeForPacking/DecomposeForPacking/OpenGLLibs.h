/** Libraries used by OpenGL should be defined here for easier control over imports. */

// GLEW is used to load OpenGL extensions, and also OpenGL api functions above version 1.1
// The OpenGL Api is supplied by this library.
#include <GL/glew.h>

// GLFW is used for cross-platform easy window and openGL context creation
#include <GLFW/glfw3.h>

// GLM is used for math graphics related calculations - such as setting up the model view projection matrix.
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

using MATRIX_4X4 = glm::mat4;