/*========================================================================*\
|* cGestorTexturas.cpp
\*========================================================================*/


#include "cGestorTexturas.h"
#include "../../tool/cTool.h"
#include "../../tool/cLog.h"


//--------------------------------------------------------------------------
// Variables internas propias de este modulo
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Tabla de Tipos de Textura por defecto que metemos en el Xml
// - los metemos en la lista todos:
//      + tanto los por defecto
//      + como los nuevos que declararemos en el Xml.
//--------------------------------------------------------------------------
//  eOldTipoTextura eTipo      ;	// El tipo enumerado es el indice de la tabla
//  int		iEntorno		;	// GL_MODULATE, GL_DECAL, GL_BLEND
//  GLfloat fColor[4]       ;   // Color de fundido
//  int     iStore          ;   // No lo tengo claro: 1 o 4
//  int		iMinFilter		;	// GL_LINEAR, GL_REPEAT, para Mipmap	GL_NEAREST_MIPMAP_LINEAR, etc ..
//  int		iMaxFilter		;	// igual que el anterior
//  int     iAjuste         ;   // Ajuste de textura: 
//  int		iComponentes	;   // 1, 2, 3, 4, según Formato Pixels.
//  int		iPixels			;	// 1:GL_LUMINANCE, 2:GL_LUMINANCE_ALPHA, 3:GL_RGB, 4:GL_RGBA
//  int		iValores		;	// GL_BYTE, GL_UNSIGNED_BYTE, GL_BITMAP, GL_SHORT, GL_INT, GL_FLOAT y los unsigned.
//  bool	bMipmap			;	// Si false 'glTexImage2D', si true 'gluBuild2DMipmaps'.
//--------------------------------------------------------------------------
//  char    vcNombre[64]    ;   // Sumamos el nombre, por facilitar las cosas
//--------------------------------------------------------------------------
// sTipoTextura cListaTipoTexturas::s_vtTipoTextura[] =
//--------------------------------------------------------------------------
sTipoTextura cGestorTexturas::s_vtTipoTextura[] =
{
    // Tipo     Entorno    f.Color                      St  MinFilter					MaxFilter				    Ajuste  Comp Pixels				Valores			Mipmap. Nombre.
    { eOldTipoTextura::eAT0	, GL_MODULATE, { 1.0f, 1.0f, 1.0f, 0.0f },  1, GL_NEAREST               , GL_NEAREST				, GL_REPEAT, 3, GL_RGB			, GL_UNSIGNED_BYTE, 0 , "eAT0"       },  //  0, AT0 Tex Bmp sin alpha
    { eOldTipoTextura::eAT1	, GL_MODULATE, { 1.0f, 1.0f, 1.0f, 0.0f },  1, GL_LINEAR				, GL_LINEAR					, GL_REPEAT, 3, GL_RGB			, GL_UNSIGNED_BYTE, 0 , "eAT1"       },  //  1, AT1 Tex Bmp sin alpha
    { eOldTipoTextura::eAT2	, GL_MODULATE, { 1.0f, 1.0f, 1.0f, 0.0f },  1, GL_LINEAR_MIPMAP_NEAREST	, GL_LINEAR					, GL_REPEAT, 3, GL_RGB			, GL_UNSIGNED_BYTE, 1 , "eAT2"       },	 //  2, AT2 Tex Bmp sin alpha
    { eOldTipoTextura::eAT3 , GL_MODULATE, { 1.0f, 1.0f, 1.0f, 0.0f },  4, GL_LINEAR                , GL_LINEAR                 , GL_REPEAT, 3, GL_BGR_EXT      , GL_UNSIGNED_BYTE, 0 , "eAT3"       },  //  3, AT3 Tex Bmp ?

    { eOldTipoTextura::eAT4 , GL_MODULATE, { 1.0f, 1.0f, 1.0f, 0.0f },  1, GL_LINEAR                , GL_LINEAR                 , GL_REPEAT, 4, GL_RGBA         , GL_UNSIGNED_BYTE, 0 , "eAT4"       },  //  4, AT4 Cook ?

    { eOldTipoTextura::eFT0	, GL_MODULATE, { 1.0f, 1.0f, 1.0f, 0.0f },  1, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_NEAREST	, GL_REPEAT, 4, GL_RGBA		    , GL_UNSIGNED_BYTE, 1 , "eFT0"       },	//  3, FT0 Tex Tga con alpha
    { eOldTipoTextura::eFT1	, GL_MODULATE, { 1.0f, 1.0f, 1.0f, 0.0f },  1, GL_NEAREST				, GL_LINEAR					, GL_REPEAT, 3, GL_RGB			, GL_UNSIGNED_BYTE, 0 , "eFT1"       },	//  4, FT1 Tex Bmp sin alpha
    { eOldTipoTextura::eFT2	, GL_MODULATE, { 1.0f, 1.0f, 1.0f, 0.0f },  1, GL_NEAREST				, GL_LINEAR					, GL_REPEAT, 4, GL_RGBA		    , GL_UNSIGNED_BYTE, 0 , "eFT2"       },	//  5, FT2 Tex Tga con alpha
                                                                                                                                                        //  SH0 : Brillos y flares, MinFilter GL_LINEAR_MIPMAP_LINEAR si mipmap
    { eOldTipoTextura::eSH0	, GL_MODULATE, { 1.0f, 1.0f, 1.0f, 0.0f },  1, GL_LINEAR				, GL_LINEAR					, GL_REPEAT, 1, GL_LUMINANCE    , GL_UNSIGNED_BYTE, 0 , "eSH0"       },	//  6, SH0
    { eOldTipoTextura::eSH1	, GL_MODULATE, { 1.0f, 1.0f, 1.0f, 0.0f },  1, GL_LINEAR_MIPMAP_LINEAR  , GL_LINEAR					, GL_REPEAT, 1, GL_LUMINANCE    , GL_UNSIGNED_BYTE, 1 , "eSH1"       },	//  7, SH1
                                                                                                                                                        //
    { eOldTipoTextura::eFLR	, GL_MODULATE, { 1.0f, 1.0f, 1.0f, 0.0f },  1, GL_LINEAR_MIPMAP_LINEAR	, GL_LINEAR					, GL_REPEAT, 3, GL_RGB			, GL_UNSIGNED_BYTE, 1 , "eFLR"       },	// 10, eFLR
    { eOldTipoTextura::eENG	, GL_MODULATE, { 1.0f, 1.0f, 1.0f, 0.0f },  1, GL_LINEAR_MIPMAP_NEAREST	, GL_LINEAR					, GL_REPEAT, 3, GL_BGR_EXT		, GL_UNSIGNED_BYTE, 1 , "eENG"       },	//  7, ENG
    { eOldTipoTextura::eLOG	, GL_MODULATE, { 1.0f, 1.0f, 1.0f, 0.0f },  1, GL_NEAREST				, GL_NEAREST				, GL_REPEAT, 3, GL_RGB			, GL_UNSIGNED_BYTE, 0 , "eLOG"       },	//  8, eLOG
    { eOldTipoTextura::eCOL0, GL_MODULATE, { 1.0f, 1.0f, 1.0f, 0.0f },  1, GL_LINEAR_MIPMAP_NEAREST	, GL_LINEAR					, GL_REPEAT, 4, GL_RGBA		    , GL_UNSIGNED_BYTE, 1 , "eCOL0"      },	//  9, eLOG
                                                                                                                                                        //----------------------------------------------------------------------
    { eOldTipoTextura::eTNoReg, GL_MODULATE,{ 1.0f, 1.0f, 1.0f, 0.0f }, 4, GL_LINEAR                 , GL_LINEAR                 , GL_REPEAT, 3, GL_BGR_EXT     , GL_UNSIGNED_BYTE, 0 , "eTNoReg"    },  // No registrada, por defecto AT3.
                                                                                                                                                        //----------------------------------------------------------------------
};
//--------------------------------------------------------------------------
// int cListaTipoTexturas::s_iNumTiposTextura = sizeof(cListaTipoTexturas::s_vtTipoTextura) / sizeof(sTipoTextura);
//--------------------------------------------------------------------------


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
