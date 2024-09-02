/*------------------------------------------------------------------------*\
|* sOpenGL.cpp
\*------------------------------------------------------------------------*/

#include "sOpenGL.h"
#include "../tool/cLog.h"
#include "../tool/nComun.h"
#include "../tool/cItem.h"


//--------------------------------------------------------------------------
// Statics
//--------------------------------------------------------------------------
// bool sOpenGL::m_bGlFinish = false;
//--------------------------------------------------------------------------
bool sOpenGL::m_bIniciado = false;
//--------------------------------------------------------------------------


cstatic void sOpenGL::syncro(eSync eSincr)
{
	switch (eSincr)
	{
		case eSync::Finish:
			glFinish();
			break;

		case eSync::Flush:
			glFlush();
			break;
	}
}


cstatic int sOpenGL::initOpenGL()
{
	if (!m_bIniciado)
	{
		// Initialize GLEW
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			cLog::error(" Error: sOpenGL::initOpenGL: glewInit: Fallo la inicializacion de Glew\n");
		}

		// Al menos establecemos el estado inicial de OpenGL
		glEnable(GL_DEPTH_TEST);

		m_bIniciado = true;
	}

	return 0;
}


cstatic void sOpenGL::toggle_Line_Fill()
{
	int vValor[2];

	glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	glGetIntegerv(GL_POLYGON_MODE, vValor);

    // Tres valores: GL_POINT, GL_LINE, GL_FILL

    // Dado uno, el que se pone es el siguiente:
    int iValor = vValor[1];

    iValor =
        (iValor == GL_POINT) ? GL_LINE :
        (iValor == GL_LINE)? GL_FILL:
        (iValor == GL_FILL)? GL_POINT: iValor;

    glPolygonMode(GL_FRONT_AND_BACK, iValor);
}


//--------------------------------------------------------------------------
// Devuelve el valor OpenGL asociado a la cadena
// - Traducimos las cadena de parametros OpenGL introducidos para
//   los parametros de Textura, primero.
// - Nos sirve para cualquier valor OpenGL, no solo para las texturas
//--------------------------------------------------------------------------
sItem s_vcNombresOpenGL[] =
{
    // Parametro Entorno
    { GL_MODULATE       , "GL_MODULATE"     , nullptr },
    { GL_REPLACE        , "GL_REPLACE"      , nullptr },
    { GL_DECAL          , "GL_DECAL"        , nullptr },
    { GL_BLEND          , "GL_BLEND"        , nullptr },
    { GL_ADD            , "GL_ADD"          , nullptr },
    // Filtrado
    // minFilter, maxFilter
    { GL_NEAREST                    , "GL_NEAREST"                  , nullptr },
    { GL_LINEAR                     , "GL_LINEAR"                   , nullptr },
    { GL_NEAREST_MIPMAP_NEAREST     , "GL_NEAREST_MIPMAP_NEAREST"   , nullptr },
    { GL_NEAREST_MIPMAP_LINEAR      , "GL_NEAREST_MIPMAP_LINEAR"    , nullptr },
    { GL_LINEAR_MIPMAP_NEAREST      , "GL_LINEAR_MIPMAP_NEAREST"    , nullptr },
    { GL_LINEAR_MIPMAP_LINEAR       , "GL_LINEAR_MIPMAP_LINEAR"     , nullptr },
    //
    // Ajuste de textura (bordes)
    // ajuste (ya veremos cuando introducimos)
    //  GL_TEXTURE_WRAP_S
    //  GL_TEXTURE_WRAP_T
    //  GL_TEXTURE_WRAP_R ? (Texturas 3D ?)
    { GL_CLAMP                  , "GL_CLAMP"            , nullptr },
    { GL_REPEAT                 , "GL_REPEAT"           , nullptr },
    { GL_CLAMP_TO_BORDER        , "GL_CLAMP_TO_BORDER"  , nullptr },
    { GL_CLAMP_TO_EDGE          , "GL_CLAMP_TO_EDGE"    , nullptr },
    // pixels
    { GL_RED                , "GL_RED"              , nullptr },
    { GL_GREEN              , "GL_GREEN"            , nullptr },
    { GL_BLUE               , "GL_BLUE"             , nullptr },
    { GL_RGB                , "GL_RGB"              , nullptr },
    { GL_ALPHA              , "GL_ALPHA"            , nullptr },
    { GL_RGBA               , "GL_RGBA"             , nullptr },
    { GL_BGR                , "GL_BGR"              , nullptr },
    { GL_BGRA               , "GL_BGRA"             , nullptr },
    { GL_BGR_EXT            , "GL_BGR_EXT"          , nullptr },
    { GL_BGRA_EXT           , "GL_BGRA_EXT"         , nullptr },
    { GL_LUMINANCE          , "GL_LUMINANCE"        , nullptr },
    { GL_LUMINANCE_ALPHA    , "GL_LUMINANCE_ALPHA"  , nullptr },
    { GL_STENCIL_INDEX      , "GL_STENCIL_INDEX"    , nullptr },
    { GL_DEPTH_COMPONENT    , "GL_DEPTH_COMPONENT"  , nullptr },
    // valores
    { GL_UNSIGNED_BYTE          , "GL_UNSIGNED_BYTE"        , nullptr },
    { GL_BYTE                   , "GL_BYTE"                 , nullptr },
    { GL_BITMAP                 , "GL_BITMAP"               , nullptr },
    { GL_UNSIGNED_SHORT         , "GL_UNSIGNED_SHORT"       , nullptr },
    { GL_SHORT                  , "GL_SHORT"                , nullptr },
    { GL_UNSIGNED_INT           , "GL_UNSIGNED_INT"         , nullptr },
    { GL_INT                    , "GL_INT"                  , nullptr },
    { GL_FLOAT                  , "GL_FLOAT"                , nullptr },
    // No los pongo todos, pongo este para recordar que hay otros
    { GL_UNSIGNED_BYTE_3_3_2    , "GL_UNSIGNED_BYTE_3_3_2"  , nullptr },
    // FaceModo
    { GL_BACK                   , "GL_BACK"               , nullptr},
    { GL_FRONT                  , "GL_FRONT"              , nullptr},
    { GL_FRONT_AND_BACK         , "GL_FRONT_AND_BACK"     , nullptr},
    //
    // Cara frotal segun el sentido los vertices (creo)
    { GL_CCW                    , "GL_CCW"              , nullptr },
    { GL_CW                     , "GL_CW"               , nullptr },
    //
    { GL_POINT                  , "GL_POINT"            , nullptr },
    { GL_LINE                   , "GL_LINE"             , nullptr },
    { GL_FILL                   , "GL_FILL"             , nullptr },
};
//--------------------------------------------------------------------------
cstatic int sOpenGL::getCodigoOpenGL(const char* pcNombreOpenGL)
{
    int iValor = -1;

    if (mNoVacia(pcNombreOpenGL))
    {
        char vcNombre[64];
        mCopia(vcNombre, pcNombreOpenGL);

        int iNum = mSizeArray(s_vcNombresOpenGL);

        for (int i = 0; i < iNum && iValor == -1; i++)
        {
            if (!strcmp(vcNombre, s_vcNombresOpenGL[i].vcNombre))
            {
                iValor = s_vcNombresOpenGL[i].iIndex;
            }
        }
    }

    return iValor;
}


cstatic const char* sOpenGL::getNombreOpenGL(int iCodigoOpenGL)
{
    const char* pcNombreOpenGL = nullptr;

    if (iCodigoOpenGL > -1)
    {
        int iNum = mSizeArray(s_vcNombresOpenGL);

        for (int i = 0; i < iNum && !pcNombreOpenGL; i++)
        {
            if (iCodigoOpenGL == s_vcNombresOpenGL[i].iIndex)
            {
                pcNombreOpenGL = s_vcNombresOpenGL[i].vcNombre;
            }
        }
    }

    return pcNombreOpenGL;
}


cstatic int sOpenGL::getValorAtributo(cAtributo* poAtributo, int valorPorDefecto)
{
    int iValor = valorPorDefecto;

    // Si no hay atributo, se queda con el valor por defecto.
    //if (poAtributo)
    //{
    //    iValor = sOpenGL::getCodigoOpenGL(poAtributo->getValor().getString());

    //    // Sino se encuentra se queda el valor por defecto:
    //    iValor = (iValor != -1) ? iValor : valorPorDefecto;
    //}

    return iValor;
}



/*------------------------------------------------------------------------*\
|* Fin de sOpenGL.h
\*------------------------------------------------------------------------*/
