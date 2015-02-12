#include "OpenGL3DDefaultProgramSetup.h"

static const int POSITION_SIZE = 3;
static const int COLOR_SIZE = 4;
static const int STRIDE = (POSITION_SIZE + COLOR_SIZE) * 4;

OpenGL3DDefaultProgramSetup::OpenGL3DDefaultProgramSetup()
{
}


OpenGL3DDefaultProgramSetup::~OpenGL3DDefaultProgramSetup()
{
}

void OpenGL3DDefaultProgramSetup::createAttributes(OpenGLShaderProgram& program)
{
	_positionIndex = program.addAttribute("aPosition");
	_colorIndex = program.addAttribute("aColor");
}

void OpenGL3DDefaultProgramSetup::createUniforms(OpenGLShaderProgram& program)
{
	_mvpMatrixIndex = program.addUniform("uMVP");
}

void OpenGL3DDefaultProgramSetup::setUniformValues(MATRIX_4X4& mvp)
{
	glUniformMatrix4fv(_mvpMatrixIndex, 1, GL_FALSE, &mvp[0][0]);
}

void OpenGL3DDefaultProgramSetup::setup()
{
	// Enable vertex attribute 0 (position)
	glEnableVertexAttribArray(_positionIndex);
	glVertexAttribPointer(
		_positionIndex,     // Match the attribute order in the shader.
		POSITION_SIZE,      // size
		GL_FLOAT,           // type
		GL_FALSE,           // is normalized
		STRIDE,             // stride
		(void*)0            // array buffer offset
		);

	// Enable vertex attribute 1 (color)
	glEnableVertexAttribArray(_colorIndex);
	glVertexAttribPointer(
		_colorIndex,        // Match the attribute order in the shader.
		COLOR_SIZE,         // size
		GL_FLOAT,           // type
		GL_FALSE,           // is normalized
		STRIDE,             // stride
		(void*)(POSITION_SIZE * 4) // array buffer offset
		);
}

void OpenGL3DDefaultProgramSetup::tearDown()
{
	glDisableVertexAttribArray(_positionIndex);
	glDisableVertexAttribArray(_colorIndex);
}