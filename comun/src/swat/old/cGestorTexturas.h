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
    std::map<std::string, cTextura*> _mapTexturas;

    static cGestorTexturas* _instancia;

public:

    cGestorTexturas();
    ~cGestorTexturas();

    static cGestorTexturas* Instancia();
    static void Delete();

    int CargaTextura(
        const char* pcPathFichero, 
        const char* pcDirectorio = nullptr, 
        textureType type = textureType::TexSimple,
        bool generateMipMaps = true);
    cTextura* GetTextura(const char* pcNombreFichero);
    void DeleteTexturas();

    void listarTexturas();
};
//--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de cGestorTexturas.h
\*========================================================================*/
