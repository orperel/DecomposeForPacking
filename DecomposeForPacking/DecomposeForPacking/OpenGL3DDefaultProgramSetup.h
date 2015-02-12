#pragma once

#include "OpenGLLibs.h"
#include "OpenGLProgramSetup.h"
#include "OpenGLShaderProgram.h"

class OpenGL3DDefaultProgramSetup : public OpenGLProgramSetup
{
public:
	OpenGL3DDefaultProgramSetup();
	~OpenGL3DDefaultProgramSetup();

	void setUniformValues(MATRIX_4X4& mvp);
	void setup() override;
	void tearDown() override;

	void createAttributes(OpenGLShaderProgram& program);
	void createUniforms(OpenGLShaderProgram& program);

private:

	GLuint _positionIndex;
	GLuint _colorIndex;
	GLuint _mvpMatrixIndex;
};

