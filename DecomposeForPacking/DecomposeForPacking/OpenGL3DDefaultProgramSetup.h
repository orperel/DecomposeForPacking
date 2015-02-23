#pragma once

#include "OpenGLLibs.h"
#include "OpenGLProgramSetup.h"
#include "OpenGLShaderProgram.h"

class OpenGL3DDefaultProgramSetup : public OpenGLProgramSetup
{
public:
	OpenGL3DDefaultProgramSetup();
	~OpenGL3DDefaultProgramSetup();

	void setUniformValues(MATRIX_4X4& mvp, MATRIX_4X4& modelMatrix, MATRIX_4X4& viewMatrix,
						  VECTOR_3D& lightPosition);
	void setup() override;
	void tearDown() override;

	void createAttributes(OpenGLShaderProgram& program);
	void createUniforms(OpenGLShaderProgram& program);

private:

	GLuint _positionIndex;
	GLuint _normalIndex;
	GLuint _colorIndex;
	GLuint _mvpMatrixIndex;
	GLuint _modelMatrixIndex;
	GLuint _viewMatrixIndex;
	GLuint _lightPositionIndex;
};

