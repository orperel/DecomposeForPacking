#include "OpenGL2DDefaultProgramSetup.h"

static const int POSITION_SIZE = 2;
static const int COLOR_SIZE = 4;
static const int STRIDE = (POSITION_SIZE + COLOR_SIZE) * 4;

OpenGL2DDefaultProgramSetup::OpenGL2DDefaultProgramSetup(OpenGLShaderProgram& program)
{
}

OpenGL2DDefaultProgramSetup::~OpenGL2DDefaultProgramSetup()
{
}

void OpenGL2DDefaultProgramSetup::createAttributes(OpenGLShaderProgram& program)
{
	_positionIndex = program.addAttribute("aPosition");
	_colorIndex = program.addAttribute("aColor");
}

void OpenGL2DDefaultProgramSetup::createUniforms(OpenGLShaderProgram& program)
{
	_mvpMatrixIndex = program.addUniform("uMVP");
}

void OpenGL2DDefaultProgramSetup::setUniformValues(MATRIX_4X4& mvp)
{
	glUniformMatrix4fv(_mvpMatrixIndex, 1, GL_FALSE, &mvp[0][0]);
}

void OpenGL2DDefaultProgramSetup::setup()
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
		(void*)(POSITION_SIZE*4) // array buffer offset
		);
}

void OpenGL2DDefaultProgramSetup::tearDown()
{
	glDisableVertexAttribArray(_positionIndex);
	glDisableVertexAttribArray(_colorIndex);
}