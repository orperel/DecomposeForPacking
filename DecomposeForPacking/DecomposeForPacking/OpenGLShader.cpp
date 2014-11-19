#include "OpenGLShader.h"
#include <sstream>
#include <fstream>
#include "DFPLogger.h"

using std::stringstream;
using std::ifstream;

#pragma region - Lifecycle methods

/** Creates a new shader of the given type: vertex shader, fragment shader, etc..
 *  Shaders cannot be used until loaded.
 */
OpenGLShader::OpenGLShader(const GLenum shaderType): _shaderType(shaderType){
    
    // Creates the shader object
    // If this instruction fails, _shaderID is initialized with CREATE_SHADER_ERROR.
    // A failure of this instruction may indicate an invalid rendering context.
    _shaderID = glCreateShader(shaderType);
}

OpenGLShader::~OpenGLShader() {
    
    glDeleteShader(_shaderID);
}

#pragma region - Logic methods

/** Loads the shader source, of the given type.
 *  shaderSrc should contain the shader's source code.
 */
void OpenGLShader::loadShaderString(const string shaderSrc) {
    
    const char* cShaderSrc = shaderSrc.c_str();
    
    // Load the shader with the source code
    glShaderSource(_shaderID, 1, &cShaderSrc, NULL);
}

/** Creates, loads and compiles the shader source, of the given type.
 *  shaderFile should contain the shader file's path.
 */
bool OpenGLShader::loadShaderFile(const string shaderFile) {
    
    ifstream inputFile;
    inputFile.open(shaderFile.c_str());
    
    if (!inputFile.good()) {
        DFPLogger::getInstance().log("Error reading shader file: " + shaderFile,  DFPLogger::ERROR);
        return false;
    }
    
    // Dump the content's of the shader file into a string stream
    stringstream strStream;
    strStream << inputFile.rdbuf();
    
    // Close the file to release IO resources
    inputFile.close();
    
    // Convert the string stream to string and load the source code into the shader
    loadShaderString(strStream.str());
    
    return true;
}

/** Compiles the shader and returns it if successful (otherwise an error code is returned). */
GLuint OpenGLShader::compile() {
    
    GLuint shader = id();
    glCompileShader(shader);
    
    // Query whether the shader compiled correctly
    GLint isCompiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    
    // If an error occured during compilation, log the details and release resources
    if (!isCompiled) {
        
        GLint infoLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
        
        if (infoLogLength > 0) {
            
            GLchar* strInfoLog = new GLchar[infoLogLength + 1];
            glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);
            
            DFPLogger::getInstance().log(strInfoLog, DFPLogger::ERROR);
            
            delete[] strInfoLog;
        }
        
        glDeleteShader(shader);
        return CREATE_SHADER_ERROR;
    }
    
    return shader;
}

#pragma region - Getters & Setters

/** The shader unique identifier (used by OpenGL) */
GLuint OpenGLShader::id() const {
    return _shaderID;
}

/** The shader type: vertex, fragment, etc.. */
GLenum OpenGLShader::type() const {
    return _shaderType;
}

#pragma region - Operator Overloading

/** Operator overloading to support string representation of this object */
std::ostream& operator<<(std::ostream &strm, const OpenGLShader &shader) {
    
    string title;
    
    switch (shader.type()) {
        case(GL_VERTEX_SHADER): title = "OpenGL Vertex Shader "; break;
        case(GL_FRAGMENT_SHADER): title = "OpenGL Fragment Shader "; break;
        default: title = "Unknown OpenGL Shader type "; break;
    }
    
    return strm << title << "of ID: " << shader.id();
}