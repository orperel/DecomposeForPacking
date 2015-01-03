#pragma once

class OpenGLProgramSetup
{
public:
	OpenGLProgramSetup() = default;
	virtual ~OpenGLProgramSetup() = default;
	virtual void setup() = 0;
	virtual void tearDown() = 0;
};