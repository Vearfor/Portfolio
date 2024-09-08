/*========================================================================*\
|* cShader.cpp
\*========================================================================*/

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "cShader.h"
#include "../../tool/cTool.h"
#include "../../tool/cLog.h"
#include <GLM/gtc/type_ptr.hpp>
#include <stdarg.h>


//--------------------------------------------------------------------------
// Cuerpo de la Clase
//--------------------------------------------------------------------------
cShader::cShader()
    : m_nombre("")
    , m_uiProgram(0)
{
}

cShader::~cShader()
{
    m_mapLocations.clear();
    glDeleteProgram(m_uiProgram);
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Resto de metodos
//--------------------------------------------------------------------------
std::string cShader::fileToString(const char* fileName)
{
    std::stringstream stringStream;
    std::ifstream file;

    try
    {
        file.open(fileName, std::ios::in);
        if (!file.fail())
        {
            stringStream << file.rdbuf();
        }
    }
    catch (const std::exception&)
    {
        std::cerr << "Error:: cShader::fileToString reading file: [" << fileName << "]" << std::endl;
    }

    return stringStream.str();
}


int cShader::checkCompileError(GLuint shaderId, const char* nombreShader, int iResp, int typeOper)
{
    int iRes = iResp;
    GLint success;
    GLchar infoLog[512];

    if (typeOper == GL_LINK_STATUS)
    {
        glGetProgramiv(shaderId, typeOper, &success);
        if (!success)
        {
            glGetProgramInfoLog(shaderId, 512, NULL, infoLog);
            std::cerr << "Error: cShader::" << nombreShader << "  [" << m_nombre << "] \n" << infoLog << std::endl;
            iRes = EXIT_FAILURE;
        }
    }
    else
    {
        glGetShaderiv(shaderId, typeOper, &success);
        // Print errors if any
        if (!success)
        {
            glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
            std::cerr << "Error: cShader::" << nombreShader << "  [" << m_nombre << "] \n" << infoLog << std::endl;
            iRes = EXIT_FAILURE;
        }
    }
    return iRes;
}


int cShader::CargaShader(
    const char* pcNombreShader,
    const GLchar* vertexPath,
    const GLchar* geometryPath,
    const GLchar* fragmentPath,
    const char* directory)
{
    if (pcNombreShader)
    {
        m_nombre = pcNombreShader;
    }

    // 0. Name/path of shader files.
    char vertexFile[LON_BUFF];
    char fragmentFile[LON_BUFF];

    mInicio(vertexFile);
    mInicio(fragmentFile);
    if (directory)
    {
        sprintf_s(vertexFile, sizeof(vertexFile), "%s/%s", directory, vertexPath);
        sprintf_s(fragmentFile, sizeof(vertexFile), "%s/%s", directory, fragmentPath);
    }
    else
    {
        strcpy_s(vertexFile, sizeof(vertexFile), vertexPath);
        strcpy_s(fragmentFile, sizeof(vertexFile), fragmentPath);
    }

    m_vertexFile = vertexFile;
    m_fragmentFile = fragmentFile;

    int iRes = EXIT_SUCCESS;

    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode = fileToString(vertexFile);
    std::string fragmentCode = fileToString(fragmentFile);

    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar* fShaderCode = fragmentCode.c_str();

    // 2. Compile shaders
    GLuint vertex, fragment;

    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // Print compile errors if any
    iRes = checkCompileError(vertex, "Vertex Compilation", iRes, GL_COMPILE_STATUS);

    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // Print compile errors if any
    iRes = checkCompileError(fragment, "Fragment Compilation", iRes, GL_COMPILE_STATUS);

    // Shader Program
    this->m_uiProgram = glCreateProgram();
    glAttachShader(this->m_uiProgram, vertex);
    glAttachShader(this->m_uiProgram, fragment);
    glLinkProgram(this->m_uiProgram);
    // Print linking errors if any
    iRes = checkCompileError(this->m_uiProgram, " Linking Program", iRes, GL_LINK_STATUS);

    // Delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    if (iRes == EXIT_SUCCESS)
    {
        std::cout << " Shader Created: " << m_nombre << std::endl;
        std::cout << "  Vertex  : [" << vertexPath << "]" << std::endl;
        std::cout << "  Fragment: [" << fragmentPath << "]" << std::endl;
    }

    m_mapLocations.clear();

    return iRes;
}


// Uses the current shader
void cShader::use()
{
    if (this->m_uiProgram > 0)
    {
        glUseProgram(this->m_uiProgram);
    }
}


GLint cShader::getLocation(const char* cadenaEnGLSL)
{
    GLint loc = -1;

    if (this->m_uiProgram)
    {
        std::map<std::string, GLint>::iterator it = m_mapLocations.find(cadenaEnGLSL);
        if (it == m_mapLocations.end())
        {
            loc = glGetUniformLocation(this->m_uiProgram, cadenaEnGLSL);
            if (loc < 0)
            {
                std::cerr << "ERROR::cShader::getLocation:: [" << cadenaEnGLSL << "] " << loc << std::endl;
            }
            else
            {
                m_mapLocations[cadenaEnGLSL] = loc;

                // La primera vez debemos ver que metemos:
                cLog::print(" [%-20.20s]  Variable: '%s': uniformID: %d\n", m_nombre.c_str(), cadenaEnGLSL, loc);
            }
        }
        else
        {
            loc = m_mapLocations[cadenaEnGLSL];
        }
    }
    return loc;
}


const char* cShader::getNombre()
{
    const char* nombre = nullptr;
    if (!m_nombre.empty())
    {
        nombre = m_nombre.c_str();
    }
    return nombre;
}


void cShader::SetUniform(const char* cadenaEnGLSL, const glm::mat4& mat, GLboolean traspose)
{
    GLint iLoc = getLocation(cadenaEnGLSL);
    cShader::SetUniform(iLoc, mat, traspose);
}


void cShader::SetUniform(const char* cadenaEnGLSL, const glm::vec4& vec)
{
    GLint iLoc = getLocation(cadenaEnGLSL);
    cShader::SetUniform(iLoc, vec);
}


void cShader::SetUniform(const char* cadenaEnGLSL, const glm::vec3& vec)
{
    GLint iLoc = getLocation(cadenaEnGLSL);
    cShader::SetUniform(iLoc, vec);
}


void cShader::SetUniform(const char* cadenaEnGLSL, const glm::vec2& vec)
{
    GLint iLoc = getLocation(cadenaEnGLSL);
    cShader::SetUniform(iLoc, vec);
}


void cShader::SetUniform(const char* cadenaEnGLSL, GLfloat valor)
{
    GLint iLoc = getLocation(cadenaEnGLSL);
    cShader::SetUniform(iLoc, valor);
}


void cShader::SetUniform(const char* cadenaEnGLSL, int valor)
{
    GLint iLoc = getLocation(cadenaEnGLSL);
    cShader::SetUniform(iLoc, valor);
}


void cShader::SetUniform(const char* cadenaEnGLSL, bool valor)
{
    GLint iLoc = getLocation(cadenaEnGLSL);
    cShader::SetUniform(iLoc, valor);
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Funciones
//--------------------------------------------------------------------------
int compileShader(const char* sourceShader, int iTypeShader, const char* sTypeShader, GLuint& idShaderFinal)
{
    GLuint idShader = glCreateShader(iTypeShader);

    glShaderSource(idShader, 1, &sourceShader, nullptr);
    glCompileShader(idShader);

    GLint success;
    glGetShaderiv(idShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[512];
        glGetShaderInfoLog(idShader, 512, nullptr, infoLog);
        std::cout << "\nError::Shader::" << sTypeShader << "::Compilation_Failed\n" << infoLog << std::endl;
        std::cout << sTypeShader << "::Shader:\n" << sourceShader << std::endl;
        return -1;
    }

    idShaderFinal = idShader;

    return 0;
}


int createProgram(GLuint vertexShader, GLuint fragmentShader, GLuint& idProgramFinal)
{
    GLuint idProgram = glCreateProgram();
    if (vertexShader > 0)
    {
        glAttachShader(idProgram, vertexShader);
    }
    if (fragmentShader > 0)
    {
        glAttachShader(idProgram, fragmentShader);
    }
    glLinkProgram(idProgram);

    GLint success;
    glGetProgramiv(idProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[512];
        glGetProgramInfoLog(idProgram, 512, nullptr, infoLog);
        std::cout << "\nError::Program::Linking_Failed\n" << infoLog << std::endl;
        return -1;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    idProgramFinal = idProgram;
    return 0;
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Comunes Statics
//--------------------------------------------------------------------------
void cShader::SetUniform(GLint iLoc, const glm::mat4& mat, GLboolean transpose)
{
    if (iLoc > -1)
    {
        glUniformMatrix4fv(iLoc, 1, transpose, glm::value_ptr(mat));
    }
}


void cShader::SetUniform(GLint iLoc, const glm::vec4& vec)
{
    if (iLoc > -1)
    {
        glUniform4f(iLoc, vec.x, vec.y, vec.z, vec.w);
    }
}


void cShader::SetUniform(GLint iLoc, const glm::vec3& vec)
{
    if (iLoc > -1)
    {
        glUniform3f(iLoc, vec.x, vec.y, vec.z);
    }
}


void cShader::SetUniform(GLint iLoc, const glm::vec2& vec)
{
    if (iLoc > -1)
    {
        glUniform2f(iLoc, vec.x, vec.y);
    }
}


void cShader::SetUniform(GLint iLoc, GLfloat valor)
{
    if (iLoc > -1)
    {
        glUniform1f(iLoc, valor);
    }
}


void cShader::SetUniform(GLint iLoc, int valor)
{
    if (iLoc > -1)
    {
        glUniform1i(iLoc, valor);
    }
}


void cShader::SetUniform(GLint iLoc, bool valor)
{
    if (iLoc > -1)
    {
        glUniform1i(iLoc, valor? 1: 0);
    }
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
char* cShader::SetNombre(char* pcNombre, int iSizeNombre, const char* pcFormato, ...)
{
    if (pcNombre && pcFormato)
    {
        memset(pcNombre, 0, iSizeNombre);
        va_list stList;

        va_start(stList, pcFormato);
        //------
        vsprintf_s(pcNombre, iSizeNombre, pcFormato, stList);
        //------
        va_end(stList);

        return pcNombre;
    }
    return nullptr;
}


char* cShader::SetPatron(char* pcPatron, int iSizePatron, const char* nombre, const char* nomVariable)
{
    if (pcPatron && nombre && nomVariable)
    {
        memset(pcPatron, 0, iSizePatron);

        sprintf_s(pcPatron, iSizePatron, "%s.%s", nombre, nomVariable);

        return pcPatron;
    }
    return nullptr;
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// To use for 0, and do not use shader program
//--------------------------------------------------------------------------
cstatic void cShader::use(GLuint uiProgram)
{
    glUseProgram(uiProgram);
}
//--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de cShader.cpp
\*========================================================================*/
