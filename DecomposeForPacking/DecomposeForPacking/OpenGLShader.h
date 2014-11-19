#pragma once

#include "OpenGLLibs.h" // Must be first to enable OpenGL import
#include <iostream>

using std::string;

/** This error code is returned when a shader object fails to be created or compiled */
const static GLuint CREATE_SHADER_ERROR = 0;

/** Represents an instance of a single shader of OpenGL.
 *  This class serves as a wrapper for creating, loading and destroying OpenGL shaders.
 */
class OpenGLShader {
    
public:
    
    /** Creates a new shader of the given type: vertex shader, fragment shader, etc..
     *  Shaders cannot be used until loaded.
     */
    OpenGLShader(const GLenum shaderType);
    virtual ~OpenGLShader();
    
    /** Creates, loads and compiles the shader source, of the given type.
     *  shaderSrc should contain the shader's source code.
     */
    void loadShaderString(const string shaderSrc);

    /** Creates, loads and compiles the shader source, of the given type.
     *  shaderFile should contain the shader file's path.
     *  Return value indicates if the load was successful or not.
     */
    bool loadShaderFile(const string shaderFile);
    
    /** Compiles the shader and returns its id if successful (otherwise an error code is returned). */
    GLuint compile();
    
    /**  - Getters & Setters - */
    
    /** The shader unique identifier (used by OpenGL) */
    GLuint id() const;
    
    /** The shader type: vertex, fragment, etc.. */
    GLenum type() const;
    
private:
    
    /** The shader unique identifier (used by OpenGL).
      * Initialized only after the shader is loaded. This value is immutable
      */
    GLuint _shaderID;
    
    /** The shader type: vertex, fragment, etc..
     *  Determined during object creation. This value is immutable.
     */
    GLuint _shaderType;
    
    /** Operator overloading to support string representation of this object */
    friend std::ostream& operator<<(std::ostream&, const OpenGLShader&);
};