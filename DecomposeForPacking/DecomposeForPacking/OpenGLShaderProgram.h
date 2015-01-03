#pragma once

#include <iostream>
#include <memory>
#include "OpenGLShader.h"

using std::unique_ptr;

/** This error code is returned when a shader program object fails to be created or linked */
const static GLuint CREATE_PROGRAM_ERROR = 0;

/** Represents an OpenGL shader program, combining multiple shaders.
 *  This class serves as a wrapper for loading, activating / deactivating OpenGL shader programs.
 */
class OpenGLShaderProgram {
    
public:
    /** Creates a new shader program from the given vertex & fragment shaders.
     *  The shader program has to be assigned attributes and uniforms and linked before it can be used.
     */
    OpenGLShaderProgram(unique_ptr<OpenGLShader> vertexShader, unique_ptr<OpenGLShader> fragmentShader);
    
    virtual ~OpenGLShaderProgram();
    
    /** Adds the attribute to the shader program.
     *  The attribute's index will be assigned sequentially.
     */
	GLuint addAttribute(const string attributeName);
    
    /** Adds the uniform to the shader program.
     *  The uniform's index will be assigned sequentially. The ID of the assigned uniform is returned.
     *  If there's no uniform with the given name in the program, -1 is returned.
     */
    GLuint addUniform(const string uniformName);
    
    /** Links the given program with the given shaders, attributes and uniforms. 
     *  Returns GL_TRUE if the linking succeeds, or GL_FALSE otherwise.     
     */
    GLuint link();
    
    /** Instructs OpenGL to use this program as the active shader program for the upcoming renders. */
    void use();
    
    /** Instructs OpenGL to stop using this program for renderings. */
    void disable();
    
    /**  - Getters & Setters - */
    
    /** The shader program unique identifier (used by OpenGL) */
    GLuint id() const;
    GLuint attribute(const string name) const;
    GLuint uniform(const string name) const;
    
private:
    
    /** The unique program id assigned by OpenGL when the program is created. */
    GLuint _programId;
    
    /** The vertex and fragment shaders that compose the OpenGL program. */
    unique_ptr<OpenGLShader> _vertexShader;
    unique_ptr<OpenGLShader> _fragmentShader;
    
    /** The number of attributes assigned to the program. */
    short _attributesCount;
    
    /** The number of uniforms assigned to the program. */
    short _uniformCount;
    
    /** Operator overloading to support string representation of this object */
    friend std::ostream& operator<<(std::ostream&, const OpenGLShaderProgram&);
};