/*========================================================================*\
|* cGestorShaders.h
\*========================================================================*/


#pragma once
//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include "cShader.h"
#include <map>
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Class
//--------------------------------------------------------------------------
class _mExport cGestorShaders
{
    std::map<std::string, cShader*> m_mapShaders;

    static cGestorShaders* m_instancia;

    cGestorShaders();
    ~cGestorShaders();
    void DeleteShaders();

public:

    static cGestorShaders* Instancia();
    static void Delete();

    int CargaShader(
        const char* pcNombreShader,
        const GLchar* vertexPath,
        const GLchar* geometryPath,
        const GLchar* fragmentPath,
        const char* directory = nullptr);
    cShader* GetShader(const char* pcNombreShader);

    int listarShaders();
};
//--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de cGestorShaders.h
\*========================================================================*/
