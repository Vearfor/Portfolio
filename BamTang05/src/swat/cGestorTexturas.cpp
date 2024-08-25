/*========================================================================*\
|* cGestorTexturas.cpp
\*========================================================================*/


#include "cGestorTexturas.h"
#include "../tool/cTool.h"


//--------------------------------------------------------------------------
// Constructor & Destructor
//--------------------------------------------------------------------------
cGestorTexturas* cGestorTexturas::_instancia = nullptr;
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
    if (!_instancia)
    {
        _instancia = new cGestorTexturas();
    }
    return _instancia;
}

void cGestorTexturas::Delete()
{
    if (_instancia)
    {
        delete _instancia;
        _instancia = nullptr;
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
    auto iter = _mapTexturas.find(nombreFichero);
    if (iter == _mapTexturas.end())
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
            _mapTexturas.insert(par);
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
    auto iter = _mapTexturas.find(pcNombreFichero);
    if (iter != _mapTexturas.end())
    {
        textura = iter->second;
    }
    return textura;
}


void cGestorTexturas::DeleteTexturas()
{
    if (_mapTexturas.size() > 0)
    {
        for (auto iter = _mapTexturas.begin(); iter != _mapTexturas.end(); iter++)
        {
            cTextura* textura = iter->second;
            if (textura)
            {
                textura->deleteTextura();
                delete textura;
            }
        }
        _mapTexturas.clear();
    }
}


void cGestorTexturas::listarTexturas()
{
    uint32_t totalSize = 0;
    printf("\n Texturas Cargadas: %d\n", (int)_mapTexturas.size());
    int i = 0;
    for (auto iter = _mapTexturas.begin(); iter != _mapTexturas.end(); iter++)
    {
        cTextura* textura = iter->second;
        if (textura)
        {
            totalSize += (int)textura->getSize();
            printf(" + %2d:  idTex: %2d  [%-25.25s]    [%4d x %4d x %d] == %12d\n",
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
    printf("\n [%d bytes  ==  %d K  == %d Megas]\n",
        totalSize,
        totalSize / 1024,
        totalSize / (1024 * 1024)
        );
}

//--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de cGestorTexturas.h
\*========================================================================*/
