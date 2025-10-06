/*========================================================================*\
|* cGestorTexturas.h
\*========================================================================*/

#pragma once
//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include "cTextura.h"
#include <tool/nComun.h>
#include <map>
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// sTipoTextura
//--------------------------------------------------------------------------
struct _mExport sTipoTextura
{
    eOldTipoTextura eTipo;

    int		iEntorno;       // GL_MODULATE, GL_DECAL, GL_BLEND

    // Color de fundido si el entorno de textura se hace GL_BLEND
    GLfloat fColor[4];

    int     iStore;         // Como esta almacenado: 1 o 4
    int		iMinFilter;     // GL_LINEAR, GL_REPEAT, para Mipmap	GL_NEAREST_MIPMAP_LINEAR, etc ..
    int		iMaxFilter;     // igual que el anterior
    int     iAjuste;
    int		iComponentes;   // 1, 2, 3, 4, según Formato Pixels.
    int		iPixels;	    // 1:GL_LUMINANCE, 2:GL_LUMINANCE_ALPHA, 3:GL_RGB, 4:GL_RGBA
    int		iValores;	    // GL_BYTE, GL_UNSIGNED_BYTE, GL_BITMAP, GL_SHORT, GL_INT, GL_FLOAT y los unsigned.
    bool	bMipmap;	    // Si false 'glTexImage2D', si true 'gluBuild2DMipmaps'.

    // Introducimos el Nombre, el nombre que hayamos dado en el Xml o en la tabla Por defecto
    // Mejor en el mismo registro que estar haciendo busquedas en una lista.
    // Así lo tenemos directamente.
    char    vcNombre[64];

    void inicio(void)
    {
        eTipo = eOldTipoTextura::eTNoReg;
        iEntorno = GL_MODULATE;
        fColor[0] = 1.0f;
        fColor[1] = 1.0f;
        fColor[2] = 1.0f;
        fColor[0] = 0.0f;
        iStore = 4;
        iMinFilter = GL_NEAREST;
        iMaxFilter = GL_NEAREST;
        iAjuste = GL_REPEAT;
        iComponentes = 4;
        iPixels = GL_RGBA;
        iValores = GL_UNSIGNED_BYTE;
        bMipmap = false;
        mInicio(vcNombre);
    }
};
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Class
//--------------------------------------------------------------------------
class _mExport cGestorTexturas
{
    static sTipoTextura s_vtTipoTextura[(int)eOldTipoTextura::NUM_TIPOS_TEXTURA];

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
