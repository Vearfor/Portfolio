/*------------------------------------------------------------------------*\
|* sOpenGL.cpp
\*------------------------------------------------------------------------*/

#include "sOpenGL.h"
#include "../tool/cLog.h"
#include "../tool/nComun.h"
#include "../tool/cItem.h"
#include "../tool/sMath.h"
#include "../swat/texturas/cTextura.h"


//--------------------------------------------------------------------------
// Statics
//--------------------------------------------------------------------------
// bool sOpenGL::m_bGlFinish = false;
//--------------------------------------------------------------------------
bool sOpenGL::m_bIniciado = false;
int sOpenGL::m_iLevelMatrixVista = 0;
int sOpenGL::m_iLevelMatrixProyeccion = 0;
int sOpenGL::m_iLevelMatrixTextura = 0;
eMatrixModo sOpenGL::m_eMatrixModo = eMatrixModo::eGL_MODELVIEW;
float sOpenGL::m_vfWidth_lineas[] = { 0.1f, 1.0f };
float sOpenGL::m_fInc_lineas = 0.1f;
//--------------------------------------------------------------------------


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

        //----------------------------------------------------------------------
        // Tamanio de la linea.
        //----------------------------------------------------------------------
        float fValor;
        glEnable(GL_LINE_SMOOTH);
        glGetFloatv(GL_LINE_WIDTH, &fValor);
        glGetFloatv(GL_LINE_WIDTH_RANGE, m_vfWidth_lineas);
        glGetFloatv(GL_LINE_WIDTH_GRANULARITY, &m_fInc_lineas);

        fValor = m_vfWidth_lineas[0] + ((m_vfWidth_lineas[0] == 0) ? m_fInc_lineas : 0);

        m_vfWidth_lineas[0] = (m_vfWidth_lineas[0] == 0) ? m_fInc_lineas : m_vfWidth_lineas[0];

        glLineWidth(fValor);
        //----------------------------------------------------------------------

		// Al menos establecemos el estado inicial de OpenGL
		glEnable(GL_DEPTH_TEST);

		m_bIniciado = true;
	}

	return 0;
}


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
// Funciones de push y pop Matrix
//--------------------------------------------------------------------------
cstatic eMatrixModo sOpenGL::pushMatrix(eMatrixModo eModo)
{
    setMatrixModo(eModo);
    switch (m_eMatrixModo)
    {
    case eMatrixModo::eGL_MODELVIEW:
        m_iLevelMatrixVista++;
        break;
    case eMatrixModo::eGL_PROJECTION:
        m_iLevelMatrixProyeccion++;
        break;
    case eMatrixModo::eGL_TEXTURE:
        m_iLevelMatrixTextura++;
        break;
    default:
        break;
    }
    glPushMatrix();

    return m_eMatrixModo;
}


cstatic eMatrixModo sOpenGL::popMatrix(eMatrixModo eModo)
{
    setMatrixModo(eModo);
    switch (eModo)
    {
    case eMatrixModo::eGL_MODELVIEW:
        m_iLevelMatrixVista--;
        break;
    case eMatrixModo::eGL_PROJECTION:
        m_iLevelMatrixProyeccion--;
        break;
    case eMatrixModo::eGL_TEXTURE:
        m_iLevelMatrixTextura--;
        break;
    default:
        break;
    }

    // si fueran menor que 0, algun error se esta produciendo:
    glPopMatrix();

    return m_eMatrixModo;
}


cstatic int sOpenGL::getLevelPushMatrix(eMatrixModo eModo)
{
    int iLevel = 0;
    switch (eModo)
    {
    case eMatrixModo::eGL_MODELVIEW:
        iLevel = m_iLevelMatrixVista;
        break;
    case eMatrixModo::eGL_PROJECTION:
        iLevel = m_iLevelMatrixProyeccion;
        break;
    case eMatrixModo::eGL_TEXTURE:
        iLevel = m_iLevelMatrixTextura;
        break;
    }
    return iLevel;
}


cstatic void sOpenGL::setMatrixModo(eMatrixModo eModo)
{
    if (m_eMatrixModo != eModo)
    {
        switch (eModo)
        {
        case eMatrixModo::eGL_MODELVIEW:
            glMatrixMode(GL_MODELVIEW);
            break;
        case eMatrixModo::eGL_PROJECTION:
            glMatrixMode(GL_PROJECTION);
            break;
        case eMatrixModo::eGL_TEXTURE:
            glMatrixMode(GL_TEXTURE);
            break;
        }
        m_eMatrixModo = eModo;
    }
}


cstatic eMatrixModo sOpenGL::getMatrixModo(void)
{
    int iMatrixMode = 0;
    glGetIntegerv(GL_MATRIX_MODE, &iMatrixMode);
    eMatrixModo eModo = eMatrixModo::eGL_MODELVIEW;
    switch (iMatrixMode)
    {
    case GL_PROJECTION:
        eModo = eMatrixModo::eGL_PROJECTION;
        break;
    case GL_TEXTURE:
        eModo = eMatrixModo::eGL_TEXTURE;
        break;

    case GL_MODELVIEW:
    default:
        break;
    }

    if (m_eMatrixModo != eModo)
        m_eMatrixModo = eModo;

    return m_eMatrixModo;
}


//--------------------------------------------------------------------------
// Dibujamos circulos/circunferencias
// Determinamos circunferencia o circulo, o disco, por la diferencia
// Entre iRadio y iInterno (radio interno).
// iRadio == iInterno :: Circunferencia
// iInterno=0 :: Circulo
// iRadio != iInterno :: Disco
//--------------------------------------------------------------------------
// Se supone en el plano XY
// No utilizamos cuadrica.
//--------------------------------------------------------------------------
cstatic void sOpenGL::circulo(glm::vec3 centro, float fRadio, glm::vec4 vColor, float fRadioInt)
{
    glColor4f(vColor.r, vColor.g, vColor.b, vColor.a);
    sOpenGL::pushMatrix();
    {
        glTranslated(centro.x, centro.y, centro.z);
        glBegin(GL_TRIANGLE_STRIP);
        {
            float fX, fY, fZ;
            float fStep;
            float fAngulo = 0.0f;
            float fMeridiano = 0.0f;
            float fRadioExterno = fRadio;
            float fRadioInterno = fRadioInt;
            int iSegmentos = 50;

            fZ = centro.z;
            fStep = (float) TWOPI / (float)iSegmentos;

            for (int i = 0; i < iSegmentos + 1; i++, fAngulo += fStep)
            {
                fMeridiano = fAngulo;
                fX = fRadioExterno * cos(fMeridiano);
                fY = fRadioExterno * sin(fMeridiano);
                glVertex3f(fX, fY, fZ);
            
                fX = fRadioInterno * cos(fMeridiano);
                fY = fRadioInterno * sin(fMeridiano);
                glVertex3f(fX, fY, fZ);
            }
        }
        glEnd();
    }
    sOpenGL::popMatrix();
}

//--------------------------------------------------------------------------
// Flecha
//--------------------------------------------------------------------------
cstatic void sOpenGL::flecha(glm::vec3 centro, float fradio, float fdir, glm::vec4 vColor)
{
    glColor4f(vColor.r, vColor.g, vColor.b, vColor.a);
    sOpenGL::pushMatrix();
    {
        float fOldValor;
        glGetFloatv(GL_LINE_WIDTH, &fOldValor);
        glLineWidth(2.0f);

        float fX = centro.x;
        float fY = centro.y;
        float fZ = centro.z;
        glTranslated(centro.x, centro.y, centro.z);

        glRotatef(fdir, 0.0f, 0.0f, 1.0f);
        glScalef(0.8f, 0.8f, 0.8f);
        glBegin(GL_LINES);
        {
            glVertex3f(- fradio  , 0.0f, 0.0f);
            glVertex3f(+fradio, 0.0f, 0.0f);

            glVertex3f((float)(fradio / 2), (float)(fradio / 2), 0.0f);
            glVertex3f(+fradio, 0.0f, 0.0f);

            glVertex3f((float)(fradio / 2), (float)(- fradio / 2), 0.0f);
            glVertex3f(+fradio, 0.0f, 0.0f);
        }
        glEnd();

        glLineWidth(fOldValor);
    }
    sOpenGL::popMatrix();
}


//--------------------------------------------------------------------------
// Rectangulo (poligono relleno: Area)
// (P_X, P_Y) :: Coordenada superior izquierda
//--------------------------------------------------------------------------
cstatic int	sOpenGL::textura(double x, double y, double z, double dAncho, double dAlto, cTextura* p_poTex)
{
    if (p_poTex)
    {
        //p_poTex->activa();
        glBegin(GL_QUADS);
        {
            glTexCoord2f(1, 1);
            glVertex3d(x + dAncho, y, 0.0f);	// Se empieza con dos puntos

            glTexCoord2f(0, 1);
            glVertex3d(x, y, 0.0f);	//

            glTexCoord2f(0, 0);
            glVertex3d(x, y - dAlto, 0.0f);	// Luego cada punto es el cierre de un triangulo

            glTexCoord2f(1, 0);
            glVertex3d(x + dAncho, y - dAlto, 0.0f);	//
        }
        glEnd();
        //p_poTex->desActiva();
    }
    return 0;
}


//--------------------------------------------------------------------------
//int	cOpenGL::Circulo(
//    const v3d& posCentro,
//    double p_dRadio, double p_dInterno,
//    int iSegmentos,
//    int iColor,
//    double dInicio, double dAngle,
//    bool bDiscontinuo)
//{
//    miTrue(iColor == cNO_COLOR);
//
//    int			i;
//    double		dRadioExt, dRadioInt;
//    double		dStep, dAngulo, fX, fY, fZ;
//    double      dMeridiano;
//    GLenum		glMode;
//
//    dRadioExt = (p_dRadio < p_dInterno) ? p_dInterno : p_dRadio;
//    dRadioInt = (p_dRadio < p_dInterno) ? p_dRadio : p_dInterno;
//    dStep = (float)TWOPI / (float)iSegmentos;
//
//    // Si ya estoy haciendo el translate con z, fZ debe ser 0.0, no z !!!!
//    fZ = (float)0.0;
//
//
//    // dAngulo estara en radianes.
//    // dStep estara en radianes.
//    // Utilizaremos dMeridiano como angulo final.
//
//    if (dRadioExt > 0)
//    {
//        cOpenGL::pushMatrix();
//        {
//            double x = posCentro.x;
//            double y = posCentro.y;
//            double z = posCentro.z;
//
//            glTranslated(x, y, z);
//
//            glRotated(dInicio, 0.0f, 0.0f, 1.0f);   // Eje Z.
//            glRotated(dAngle, 1.0f, 0.0f, 0.0f);    // Eje X.
//
//            bool bLighting = cOpenGL::disable(GL_LIGHTING);
//            bool bTextura2D = cOpenGL::disable(GL_TEXTURE_2D);
//            bool bCullFace = cOpenGL::disable(GL_CULL_FACE);
//            bool bLineStipple = (bDiscontinuo) ?
//                cOpenGL::enable(GL_LINE_STIPPLE) :
//                false;
//            int iCurColor = cColor::getCurrentColor();
//            float fWidthLines = cOpenGL::getFloat(GL_LINE_WIDTH);
//
//            if (bDiscontinuo)
//                //glLineStipple(1, 0x8888);
//                //glLineStipple(1, 0x8080);
//                //glLineStipple(1, 0x8000);
//                glLineStipple(1, 0xC000);
//
//            glLineWidth(cOpenGL::getMinWidthLineas());
//
//            Act_blend(iColor, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//            cColor::color(iColor);
//            glMode = (p_dRadio == p_dInterno) ? GL_LINE_STRIP : GL_TRIANGLE_STRIP;
//            glBegin(glMode);
//            {
//                switch (glMode)
//                {
//                case GL_LINE_STRIP:
//                    for (i = 0, dAngulo = 0; i < iSegmentos + 1; i++, dAngulo += dStep)
//                    {
//                        dMeridiano = dAngulo + deg2rad(dInicio);
//                        fX = dRadioExt * Cos(dMeridiano);
//                        fY = dRadioExt * Sin(dMeridiano);
//                        glVertex3d(fX, fY, fZ);
//                    }
//                    break;
//
//                case GL_TRIANGLE_STRIP:
//                    for (i = 0, dAngulo = 0; i < iSegmentos + 1; i++, dAngulo += dStep)
//                    {
//                        dMeridiano = dAngulo + deg2rad(dInicio);
//                        fX = dRadioExt * Cos(dMeridiano);
//                        fY = dRadioExt * Sin(dMeridiano);
//                        glVertex3d(fX, fY, fZ);
//
//                        fX = dRadioInt * Cos(dMeridiano);
//                        fY = dRadioInt * Sin(dMeridiano);
//                        glVertex3d(fX, fY, fZ);
//                    }
//                    break;
//                }
//            }
//            glEnd();
//            Des_blend();
//            cColor::restore(iCurColor);
//            cOpenGL::restore(bCullFace, GL_CULL_FACE);
//            cOpenGL::restore(bLighting, GL_LIGHTING);
//            cOpenGL::restore(bTextura2D, GL_TEXTURE_2D);
//            cOpenGL::restore(bLineStipple, GL_LINE_STIPPLE);
//            glLineWidth(fWidthLines);
//        }
//        cOpenGL::popMatrix();
//    }
//    return 0;
//}
//--------------------------------------------------------------------------


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
