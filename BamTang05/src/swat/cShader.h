/*========================================================================*\
|* cShader.h
\*========================================================================*/

#pragma once
//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include "sOpenGL.h"
#include <GLM/glm.hpp>
#include <string>
#include <map>
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Class
//--------------------------------------------------------------------------
class cShader
{
    std::string _nombre;
    GLuint _uiProgram;
    std::map<std::string, GLint> _mapLocations;

    std::string fileToString(const char* fileName);
    int checkCompileError(GLuint shaderId, const char* nombreShader, int iResp, int typeOper);

public:

    cShader();
    ~cShader();

    int carga(
        const GLchar* vertexPath,
        const GLchar* geometryPath,
        const GLchar* fragmentPath,
        const char* directory = nullptr, const char* pcNombreShader = nullptr);
    void use();
    GLint getLocation(const char* cadenaenGLSL);
    const char* getNombre();

    //----------------------------------------------------------------------
    // No Static
    //----------------------------------------------------------------------
    void SetUniform(const char* cadenaEnGLSL, const glm::mat4& mat, GLboolean traspose = GL_FALSE);
    void SetUniform(const char* cadenaEnGLSL, const glm::vec4& vec);
    void SetUniform(const char* cadenaEnGLSL, const glm::vec3& vec);
    void SetUniform(const char* cadenaEnGLSL, const glm::vec2& vec);
    void SetUniform(const char* cadenaEnGLSL, GLfloat valor);
    void SetUniform(const char* cadenaEnGLSL, int valor);
    void SetUniform(const char* cadenaEnGLSL, bool valor);
    //----------------------------------------------------------------------

    //----------------------------------------------------------------------
    // Static
    //----------------------------------------------------------------------
    static void SetUniform(GLint iLoc, const glm::mat4& mat, GLboolean traspose = GL_FALSE);
    static void SetUniform(GLint iLoc, const glm::vec4& vec);
    static void SetUniform(GLint iLoc, const glm::vec3& vec);
    static void SetUniform(GLint iLoc, const glm::vec2& vec);
    static void SetUniform(GLint iLoc, GLfloat valor);
    static void SetUniform(GLint iLoc, int valor);
    static void SetUniform(GLint iLoc, bool valor);
    //----------------------------------------------------------------------
    static char* SetNombre(char* nombre, int iSizeNombre, const char* formato, ...);
    static char* SetPatron(char* patron, int iSizePatron, const char* nombre, const char* nomVariable);
    //----------------------------------------------------------------------
};
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Prototipos
//--------------------------------------------------------------------------
int compileShader(const char* sourceShader, int iTypeShader, const char* sTypeShader, GLuint& idShader);
int createProgram(GLuint vertexShader, GLuint fragmentShader, GLuint& idProgram);
//--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de cShader.h
\*========================================================================*/
