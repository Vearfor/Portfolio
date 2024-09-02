/*========================================================================*\
|* cGestorTexturas.cpp
\*========================================================================*/


#include "cGestorTexturas.h"
#include "../../tool/cTool.h"
#include "../../tool/cLog.h"


//--------------------------------------------------------------------------
// Constructor & Destructor
//--------------------------------------------------------------------------
cGestorTexturas* cGestorTexturas::m_instancia = nullptr;
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Constructor & Destructor
//--------------------------------------------------------------------------
cGestorTexturas::cGestorTexturas()
{
}

cGestorTexturas::~cGestorTexturas()
{
    DeleteTexturas();
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Singleton
//--------------------------------------------------------------------------
cGestorTexturas* cGestorTexturas::Instancia()
{
    if (!m_instancia)
    {
        m_instancia = new cGestorTexturas();
    }
    return m_instancia;
}

void cGestorTexturas::Delete()
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
int cGestorTexturas::CargaTextura(const char* pcPathFichero, const char* pcDirectorio, textureType type, bool generateMipMaps)
{
    int iRes = 0;
    std::string caminoCompleto;

    // Si tenemos directorio se añade al pathFichero
    caminoCompleto = (pcDirectorio)?
        pcDirectorio + std::string("/") + pcPathFichero :
        pcPathFichero;

    // Obtener el nombre del fichero sin el path
    char nombreFichero[LON_BUFF];
    cTool::getNombreFichero(caminoCompleto.c_str(), nombreFichero, sizeof(nombreFichero));

    // Comprobar si ya se ha cargado esta textura
    auto iter = m_mapTexturas.find(nombreFichero);
    if (iter == m_mapTexturas.end())
    {
        // Si no se ha cargado se crea y se carga
        // No lo ha encontrado, es una nueva textura que hay que cargar
        // Si hay error en la carga se devuelve error iRes = -1;
        sResultText sTex;

        sTex = cTextura::cargaTextura(type, pcPathFichero, pcDirectorio, generateMipMaps);
        if (sTex._iTex < 1)
        {
            iRes = -1;
        }
        else
        {
            cTextura* textura = new cTextura(nombreFichero, sTex._iTex, type);
            textura->setWidth(sTex._widthTextura);
            textura->setHeight(sTex._heightTextura);
            textura->setBytesPorPixel(sTex._bytesPorPixelTextura);
            textura->setSize(sTex._sizeTextura);
            std::pair<std::string, cTextura*> par(nombreFichero, textura);
            m_mapTexturas.insert(par);
        }
    }
    else
    {
        // lo ha encontrado no hay que hacer nada mas.
        // Y no es error.
    }
    return iRes;
}


cTextura* cGestorTexturas::GetTextura(const char* pcNombreFichero)
{
    // Solo devolverá la textura que encuentre en el map.
    cTextura* textura = nullptr;
    auto iter = m_mapTexturas.find(pcNombreFichero);
    if (iter != m_mapTexturas.end())
    {
        textura = iter->second;
    }
    return textura;
}


void cGestorTexturas::DeleteTexturas()
{
    if (m_mapTexturas.size() > 0)
    {
        for (auto iter = m_mapTexturas.begin(); iter != m_mapTexturas.end(); iter++)
        {
            cTextura* textura = iter->second;
            if (textura)
            {
                textura->deleteTextura();
                delete textura;
            }
        }
        m_mapTexturas.clear();
    }
}


int cGestorTexturas::listarTexturas()
{
    uint32_t totalSize = 0;
    cLog::print("\n Texturas Cargadas: %d\n", (int)m_mapTexturas.size());
    int i = 0;
    for (auto iter = m_mapTexturas.begin(); iter != m_mapTexturas.end(); iter++)
    {
        cTextura* textura = iter->second;
        if (textura)
        {
            totalSize += (int)textura->getSize();
            cLog::print(" + %2d:  idTex: %2d  [%-25.25s]    [%4d x %4d x %d] == %12d\n",
                i,
                textura->getIdTex(),
                textura->getNombre(),
                textura->getWidth(),
                textura->getHeight(),
                textura->getBytesPorPixel(),
                (int)textura->getSize());
        }
        i++;
    }
    cLog::print("\n [%d bytes  ==  %d K  == %d Megas]\n",
        totalSize,
        totalSize / 1024,
        totalSize / (1024 * 1024)
        );
    return 0;
}

//--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de cGestorTexturas.h
\*========================================================================*/
