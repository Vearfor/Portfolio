/*========================================================================*\
|* cGestorShaders.cpp
\*========================================================================*/


#include "cGestorShaders.h"
#include "../../tool/cTool.h"
#include "../../tool/cLog.h"


//--------------------------------------------------------------------------
// Constructor & Destructor
//--------------------------------------------------------------------------
cGestorShaders* cGestorShaders::m_instancia = nullptr;
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Constructor & Destructor
//--------------------------------------------------------------------------
cGestorShaders::cGestorShaders()
{
}

cGestorShaders::~cGestorShaders()
{
    DeleteShaders();
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Singleton
//--------------------------------------------------------------------------
cGestorShaders* cGestorShaders::Instancia()
{
    if (!m_instancia)
    {
        m_instancia = new cGestorShaders();
    }
    return m_instancia;
}

void cGestorShaders::Delete()
{
    if (m_instancia)
    {
        delete m_instancia;
        m_instancia = nullptr;
    }
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Resto de Metodos
//--------------------------------------------------------------------------
int cGestorShaders::CargaShader(
    const char* pcNombreShader,
    const GLchar* vertexPath,
    const GLchar* geometryPath,
    const GLchar* fragmentPath,
    const char* pcDirectory)
{
    std::string nombreShader = pcNombreShader;

    // Comprobar si ya se ha cargado esta Shader
    auto iter = m_mapShaders.find(nombreShader);
    if (iter == m_mapShaders.end())
    {
        cShader* pNewShader = new cShader();
        if (!pNewShader)
        {
            cLog::error(" Error: cGestorShaders::CargaShader: no hay memoria para new cShader\n");
            return -1;
        }

        miError(pNewShader->CargaShader(pcNombreShader, vertexPath, geometryPath, fragmentPath, pcDirectory));

        std::pair<std::string, cShader*> par(nombreShader, pNewShader);
        m_mapShaders.insert(par);
    }
    else
    {
        // lo ha encontrado no hay que hacer nada mas.
        // Y no es error.
    }
    return 0;
}


cShader* cGestorShaders::GetShader(const char* pcNombreShader)
{
    std::string nombreShader = pcNombreShader;
    cShader* pShader = nullptr;
    auto iter = m_mapShaders.find(nombreShader);
    if (iter != m_mapShaders.end())
    {
        pShader = iter->second;
    }
    return pShader;
}


void cGestorShaders::DeleteShaders()
{
    if (m_mapShaders.size() > 0)
    {
        for (auto iter = m_mapShaders.begin(); iter != m_mapShaders.end(); iter++)
        {
            cShader* pShader = iter->second;
            if (pShader)
            {
                delete pShader;
            }
        }
        m_mapShaders.clear();
    }
}


int cGestorShaders::listarShaders()
{
    uint32_t totalSize = 0;
    cLog::print("\n Shaders Cargadas: %d\n", (int)m_mapShaders.size());
    int i = 0;
    for (auto iter = m_mapShaders.begin(); iter != m_mapShaders.end(); iter++)
    {
        cShader* pShader = iter->second;
        if (pShader)
        {
            cLog::print(" + %2d:  [%-25.25s]\n",
                i,
                pShader->getNombre()
            );
        }
        i++;
    }
    cLog::print("\n");

    return 0;
}

//--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de cGestorShaders.h
\*========================================================================*/
