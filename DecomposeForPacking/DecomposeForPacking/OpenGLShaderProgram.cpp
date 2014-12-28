#include "OpenGLLibs.h" // Must be first to enable OpenGL import
#include "OpenGLShaderProgram.h"
#include <sstream>
#include "DFPLogger.h"

using std::stringstream;
using std::endl;

#pragma region - Lifecycle methods

/** Creates a new shader program from the given vertex & fragment shaders.
 *  The shader program has to be assigned attributes and uniforms and linked before it can be used.
 */
OpenGLShaderProgram::OpenGLShaderProgram(unique_ptr<OpenGLShader> vertexShader, unique_ptr<OpenGLShader> fragmentShader):
    _attributesCount(0),
    _uniformCount(0),
    _vertexShader(std::move(vertexShader)),
    _fragmentShader(std::move(fragmentShader)) {
    
    // Creates the shader program object. _programID will contain the assigned id by OpenGL for this shader program object.
    // If this instruction fails, _programID is initialized with CREATE_PROGRAM_ERROR.
    // A failure of this instruction may indicate an invalid rendering context.
    _programId = glCreateProgram();
    
    // Attach the shaders only if the program object is valid
    if (_programId != CREATE_PROGRAM_ERROR) {
        
        // The shaders are identified by OpenGL according to their assigned id.
		glAttachShader(_programId, _vertexShader->id());
		glAttachShader(_programId, _fragmentShader->id());
    }
    else
    { // Log on error.
        stringstream msg;
        msg << "Failed to create shader program with the shaders: " << endl
			<< *_vertexShader << endl << *_fragmentShader << endl
            << "This error may indicate an invalid rendering context.";
        DFPLogger::getInstance().log(msg.str(),  DFPLogger::ERROR);
    }
}

OpenGLShaderProgram::~OpenGLShaderProgram() {

    glDeleteProgram(_programId);
}

#pragma region - Logic methods

/** Adds the attribute to the shader program.
 *  The attribute's index will be assigned sequentially.
 */
GLuint OpenGLShaderProgram::addAttribute(const string attributeName) {
    
    const GLchar* cAttributeName = attributeName.c_str();
    glBindAttribLocation(_programId, _attributesCount, cAttributeName);
    _attributesCount++;

	return _attributesCount - 1;
}

/** Adds the uniform to the shader program.
 *  The uniform's index will be assigned sequentially.
 */
GLuint OpenGLShaderProgram::addUniform(const string uniformName) {

    const GLchar* cUniformName = uniformName.c_str();
    GLuint uniformID = glGetUniformLocation(_programId, cUniformName);
    _uniformCount++;
    return uniformID;
}

/** Links the given program with the given shaders, attributes and uniforms.
 *  Returns GL_TRUE if the linking succeeds, or GL_FALSE otherwise.
 */
GLuint OpenGLShaderProgram::link() {
    
    glLinkProgram(_programId);
    
    GLint linkResult;
    glGetProgramiv(_programId, GL_LINK_STATUS, &linkResult);
    
    if (!linkResult) {
        stringstream msg;
        msg << "Failed to link shader program of id: " << _programId << endl;
        DFPLogger::getInstance().log(msg.str(),  DFPLogger::ERROR);
    }
    
    // Detach and delete shaders after program compilation to free OpenGL resources.
    // A pointer to the OpenGLShader objects is maintained to enable the application access
    // information about the shaders.
    glDetachShader(_programId, _vertexShader->id());
    glDetachShader(_programId, _fragmentShader->id());
    glDeleteShader(_vertexShader->id());
    glDeleteShader(_fragmentShader->id());
    
    return linkResult;
}

/** Instructs OpenGL to use this program as the active shader program for the upcoming renders. */
void OpenGLShaderProgram::use() {

    glUseProgram(_programId);
}

/** Instructs OpenGL to stop using this program for renderings. */
void OpenGLShaderProgram::disable() {

    glUseProgram(0);
}

#pragma region - Getters & Setters

/** The shader program unique identifier (used by OpenGL) */
GLuint OpenGLShaderProgram::id() const {
    return _programId;
}

/** Id of the given attribute in the program (used by OpenGL) */
GLuint OpenGLShaderProgram::attribute(const string name) const {

    const GLchar* cName = name.c_str();
    GLuint id = glGetAttribLocation(_programId, cName);
    return id;
}

/** Id of the given uniform in the program (used by OpenGL) */
GLuint OpenGLShaderProgram::uniform(const string name) const {
    
    const GLchar* cName = name.c_str();
    GLuint id = glGetUniformLocation(_programId, cName);
    return id;
}


#pragma region - Operator Overloading

/** Operator overloading to support string representation of this object */
std::ostream& operator<<(std::ostream &strm, const OpenGLShaderProgram  &shaderProgram) {
    
    return strm << "OpenGL Shader program of ID: " << shaderProgram.id()
                << "with " << shaderProgram._attributesCount << " attributes and "
                << shaderProgram._uniformCount << " uniforms." << endl << "The program uses the following shaders: "
                << *(shaderProgram._vertexShader) << endl << *(shaderProgram._fragmentShader);
}