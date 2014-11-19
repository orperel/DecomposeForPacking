#include "ShaderFactory.h"

#pragma region - Lifecycle methods

ShaderFactory::ShaderFactory() {
}

ShaderFactory::~ShaderFactory() {
}

#pragma region - Logic methods

/** Creates, loads and compiles a shader of the given type. The shader is read from the given file at the path: shaderFile. */
unique_ptr<OpenGLShader> ShaderFactory::loadShaderFromFile(const GLenum shaderType, const string shaderFile) {
    
    std::unique_ptr<OpenGLShader> shader(new OpenGLShader(shaderType));
    
    // Report shader object creation exception (rendering context is invalid, etc).
    if (shader->id() == CREATE_SHADER_ERROR)
        return NULL;

    // Report shader source loading from file exception (file is missing or corrupted, etc).
    if (!shader->loadShaderFile(shaderFile))
        return NULL;
    
    // Report shader compilation exception (shader syntax may be incorrect, does not match specs, etc).
    if (shader->compile() == CREATE_SHADER_ERROR)
        return NULL;
    
    return shader;
}

/** Creates, loads and compiles a shader of the given type. The shader source code is contained in shaderSrc as string */
unique_ptr<OpenGLShader> ShaderFactory::loadShaderFromString(const GLenum shaderType, const string shaderSrc) {
    
    std::unique_ptr<OpenGLShader> shader(new OpenGLShader(shaderType));
    
    // Report shader object creation exception (rendering context is invalid, etc).
    if (shader->id() == CREATE_SHADER_ERROR)
        return NULL;

    shader->loadShaderString(shaderSrc);
    
    // Report shader compilation exception (shader syntax may be incorrect, does not match specs, etc).
    if (shader->compile() == CREATE_SHADER_ERROR)
        return NULL;
    
    return shader;
}