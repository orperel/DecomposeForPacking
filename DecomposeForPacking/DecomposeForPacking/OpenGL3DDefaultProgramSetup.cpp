#include "OpenGL3DDefaultProgramSetup.h"

static const int POSITION_SIZE = 3;
static const int NORMAL_SIZE = 3;
static const int COLOR_SIZE = 4;
static const int STRIDE = (POSITION_SIZE + NORMAL_SIZE + COLOR_SIZE) * 4;

OpenGL3DDefaultProgramSetup::OpenGL3DDefaultProgramSetup()
{
}


OpenGL3DDefaultProgramSetup::~OpenGL3DDefaultProgramSetup()
{
}

void OpenGL3DDefaultProgramSetup::createAttributes(OpenGLShaderProgram& program)
{
	_positionIndex = program.addAttribute("aPosition");
	_normalIndex = program.addAttribute("aNormal");
	_colorIndex = program.addAttribute("aColor");
}

void OpenGL3DDefaultProgramSetup::createUniforms(OpenGLShaderProgram& program)
{
	_mvpMatrixIndex = program.addUniform("uMVP");
	_modelMatrixIndex = program.addUniform("uM");
	_viewMatrixIndex = program.addUniform("uV");
	_lightPositionIndex = program.addUniform("uLightPosition");
}

void OpenGL3DDefaultProgramSetup::setUniformValues(MATRIX_4X4& mvp, MATRIX_4X4& modelMatrix,
												   MATRIX_4X4& viewMatrix, VECTOR_3D& lightPosition)
{
	glUniformMatrix4fv(_mvpMatrixIndex, 1, GL_FALSE, &mvp[0][0]);
	glUniformMatrix4fv(_modelMatrixIndex, 1, GL_FALSE, &modelMatrix[0][0]);
	glUniformMatrix4fv(_viewMatrixIndex, 1, GL_FALSE, &viewMatrix[0][0]);
	glUniform3fv(_lightPositionIndex, 1, &lightPosition[0]);
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

	// Enable vertex attribute 1 (normal)
	glEnableVertexAttribArray(_normalIndex);
	glVertexAttribPointer(
		_normalIndex,        // Match the attribute order in the shader.
		NORMAL_SIZE,         // size
		GL_FLOAT,            // type
		GL_FALSE,            // is normalized
		STRIDE,              // stride
		(void*)(POSITION_SIZE * 4) // array buffer offset
		);

	// Enable vertex attribute 2 (color)
	glEnableVertexAttribArray(_colorIndex);
	glVertexAttribPointer(
		_colorIndex,        // Match the attribute order in the shader.
		COLOR_SIZE,         // size
		GL_FLOAT,           // type
		GL_FALSE,           // is normalized
		STRIDE,             // stride
		(void*)((POSITION_SIZE + NORMAL_SIZE) * 4) // array buffer offset
		);
}

void OpenGL3DDefaultProgramSetup::tearDown()
{
	glDisableVertexAttribArray(_positionIndex);
	glDisableVertexAttribArray(_normalIndex);
	glDisableVertexAttribArray(_colorIndex);
}