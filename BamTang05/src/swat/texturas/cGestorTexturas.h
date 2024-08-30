/*========================================================================*\
|* cGestorTexturas.h
\*========================================================================*/


#pragma once
//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include "cTextura.h"
#include <map>
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Class
//--------------------------------------------------------------------------
class cGestorTexturas
{
    std::map<std::string, cTextura*> m_mapTexturas;

    static cGestorTexturas* m_instancia;

    cGestorTexturas();
    ~cGestorTexturas();
    void DeleteTexturas();

public:

    static cGestorTexturas* Instancia();
    static void Delete();

    int CargaTextura(
        const char* pcPathFichero, 
        const char* pcDirectorio = nullptr, 
        textureType type = textureType::TexSimple,
        bool generateMipMaps = true);
    cTextura* GetTextura(const char* pcNombreFichero);

    int listarTexturas();
};
//--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de cGestorTexturas.h
\*========================================================================*/
