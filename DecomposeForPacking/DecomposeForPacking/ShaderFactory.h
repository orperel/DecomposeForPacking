#pragma once

#include <iostream>
#include <memory>
#include "OpenGLShader.h"

using std::string;
using std::unique_ptr;

/** Responsible for creation and loading of shader programs in the graphics engine. */
class ShaderFactory {
    
public:
    
    ShaderFactory();
    virtual ~ShaderFactory();
    
    /** Creates, loads and compiles a shader of the given type. The shader is read from the given file at the path: shaderFile. */
    unique_ptr<OpenGLShader> loadShaderFromFile(const GLenum shaderType, const string shaderFile);
    
    /** Creates, loads and compiles a shader of the given type. The shader source code is contained in shaderSrc as string */
    unique_ptr<OpenGLShader> loadShaderFromString(const GLenum shaderType, const string shaderSrc);
};