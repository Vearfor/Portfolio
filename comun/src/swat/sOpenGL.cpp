/*------------------------------------------------------------------------*\
|* sOpenGL.cpp
\*------------------------------------------------------------------------*/

#include "sOpenGL.h"
#include "cAtributo.h"
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
//--------------------------------------------------------------------------
bool sOpenGL::m_bTransparencia = true;
bool sOpenGL::m_bProfundidad = true;
bool sOpenGL::m_bTextura2D = true;
//--------------------------------------------------------------------------
bool sOpenGL::m_bCullFace = true;
GLenum sOpenGL::m_eCullFaceMode = GL_BACK;      // Tambien: GL_FRONT, GL_FRONT_AND_BACK
GLenum sOpenGL::m_eFrontFace = GL_CCW;          // Tambien: GL_CW, GL_CCW
//--------------------------------------------------------------------------
float sOpenGL::m_vfSize_puntos[] = { 0.1f, 1.0f };
float sOpenGL::m_fInc_puntos = 0.1f;
//--------------------------------------------------------------------------
float sOpenGL::m_vfWidth_lineas[] = { 0.1f, 1.0f };
float sOpenGL::m_fInc_lineas = 0.1f;
//--------------------------------------------------------------------------
glm::vec4 sOpenGL::m_vCurrentColor{ 1.0f, 1.0f, 1.0f, 1.0f };
//--------------------------------------------------------------------------
eTipoCoord sOpenGL::m_eTipoCoord = eTipoCoord::eDNormal;
//--------------------------------------------------------------------------
int sOpenGL::m_xScreen = RISK_WIDTH;
int sOpenGL::m_yScreen = RISK_HEIGHT;
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

        cLog::traza(cLog::eTraza::min, " initOpenGL: Valores iniciales por defecto\n");

        int iValor;
        float fValor;
        //----------------------------------------------------------------------
        // Tamanio del punto.
        //----------------------------------------------------------------------
        glEnable(GL_POINT_SMOOTH);
        glGetFloatv(GL_POINT_SIZE_RANGE, m_vfSize_puntos);
        glGetFloatv(GL_POINT_SIZE_GRANULARITY, &m_fInc_puntos);

        fValor = m_vfSize_puntos[0] + ((m_vfSize_puntos[0] == 0) ? m_fInc_puntos : 0);
        glPointSize(fValor);
        //----------------------------------------------------------------------
        cLog::traza(cLog::eTraza::min, " - Habilitado 'point smooth'\n");
        cLog::traza(cLog::eTraza::min, " - Point size: %6.3f\n", fValor);
        //----------------------------------------------------------------------

        //----------------------------------------------------------------------
        // Tamanio de la linea.
        //----------------------------------------------------------------------
        glEnable(GL_LINE_SMOOTH);
        glGetFloatv(GL_LINE_WIDTH, &fValor);
        glGetFloatv(GL_LINE_WIDTH_RANGE, m_vfWidth_lineas);
        glGetFloatv(GL_LINE_WIDTH_GRANULARITY, &m_fInc_lineas);

        fValor = m_vfWidth_lineas[0] + ((m_vfWidth_lineas[0] == 0) ? m_fInc_lineas : 0);

        m_vfWidth_lineas[0] = (m_vfWidth_lineas[0] == 0) ? m_fInc_lineas : m_vfWidth_lineas[0];

        glLineWidth(fValor);
        //----------------------------------------------------------------------
        cLog::traza(cLog::eTraza::min, " - Habilitado 'line smooth'\n");
        cLog::traza(cLog::eTraza::min, " - Line width: %6.3f\n", fValor);
        //----------------------------------------------------------------------

        //----------------------------------------------------------------------
        // Cull Face: Eliminacion de caras.
        //----------------------------------------------------------------------
        m_bCullFace = (bool) glIsEnabled(GL_CULL_FACE);
        glGetIntegerv(GL_CULL_FACE_MODE, &iValor);
        m_eCullFaceMode = iValor;
        //----------------------------------------------------------------------
        cLog::traza(cLog::eTraza::min, " - Habilitado 'cull_face' %s\n", msTrue(m_bCullFace));
        //----------------------------------------------------------------------
        //  glCullFace(GL_BACK);    // GL_BACK: borrara la trasera.
        //----------------------------------------------------------------------

        //----------------------------------------------------------------------
        // Front Face
        // Que determina la cara Frontal o Trasera: .
        //  glFrontFace(GL_CCW);	// GL_CCW counterclockwise:
        //                             indica la cara frontal: 
        //                             en sentido contrario agujas del reloj
        //----------------------------------------------------------------------
        glGetIntegerv(GL_FRONT_FACE, &iValor);
        m_eFrontFace = iValor;
        //----------------------------------------------------------------------
        cLog::traza(cLog::eTraza::min, " - Front Face '%s'\n", sOpenGL::getNombreOpenGL(iValor));
        //----------------------------------------------------------------------

        //----------------------------------------------------------------------
        // Current Color
        //----------------------------------------------------------------------
        m_vCurrentColor = sOpenGL::getColor();
        //----------------------------------------------------------------------
        cLog::traza(cLog::eTraza::min, " - Current Color  { %6.3f, %6.3f, %6.3f, %6.3f }\n",
            m_vCurrentColor.r,
            m_vCurrentColor.g,
            m_vCurrentColor.b,
            m_vCurrentColor.a
        );
        //----------------------------------------------------------------------

        //----------------------------------------------------------------------
        // Test de profuncidad
        //----------------------------------------------------------------------
        sOpenGL::Act_depth();
        //----------------------------------------------------------------------
        cLog::traza(cLog::eTraza::min, " - Habilitado 'test profundidad' %s\n", msTrue(m_bProfundidad));
        //----------------------------------------------------------------------

        cLog::traza(cLog::eTraza::min, "\n");
		m_bIniciado = true;
	}

	return 0;
}


//--------------------------------------------------------------------------
// Encapsulamos los valores fijos de las luces
// ClearColor, AmbientColor, no se utiliza
//--------------------------------------------------------------------------
cstatic int sOpenGL::inicioLuces(glm::vec4 vAmbientColor)
{
    float vfLuzAmbiente[4];
    vfLuzAmbiente[0] = vAmbientColor.r;
    vfLuzAmbiente[1] = vAmbientColor.g;
    vfLuzAmbiente[2] = vAmbientColor.b;
    vfLuzAmbiente[3] = vAmbientColor.a;

    // Pero parece que esta suministra una luz ambiente general
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, vfLuzAmbiente);
    // Esta sola pata la Luz0 es suficiente
    //// Y esta la refuerza, por la existencia de la luz
    glLightfv(GL_LIGHT0, GL_AMBIENT, vfLuzAmbiente);
    Act_lighting();

    // When light is enabled, glColor3f has no effect ???? ...  puede que sea cierto ...
    // Esto es cuando No hay GL_LIGHTING, y funciona glColor
    // Por ahora, creo, que es incompatible con el GL_LIGHTING, no dice lo mismo el pu.. libro.
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    Act_colorMaterial();

    glShadeModel(GL_SMOOTH);
    // Pues no necesito esto para las esferas con cuadricas ...
    // glEnable(GL_NORMALIZE);  // Para forzar la normalizacion de los vectores normales
    // No creo que tenga sentido ...

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
cstatic int	sOpenGL::textura(float x, float y, float z, float dAncho, float dAlto, cTextura* poTex)
{
    if (poTex)
    {
        float fOne = 1.0f;
        poTex->activa();
        glBegin(GL_QUADS);
        {
            //--------------------------------------------------------------
            // Esta es la buena, hay que quedarse con cual es:
            //  Front Face CCW o el CW:
            // - Cull Face esta habilitado,
            // - El mode es el GL_BACK (La que se oculta)
            // - Y el front face es: GL_CCW
            //--------------------------------------------------------------
            glTexCoord2f(0.0f, fOne);                   //
            glVertex3f(x, y - dAlto, 0.0f);             // left + top

            glTexCoord2f(fOne, fOne);                   //
            glVertex3f(x + dAncho, y - dAlto, 0.0f);	// right + top, Se empieza con dos puntos:

            glTexCoord2f(fOne, 0.0f);                   // y se continua, cerrando un primer triangulo
            glVertex3f(x + dAncho, y, 0.0f);	        // right + top

            glTexCoord2f(0.0f, 0.0f);                   // y se termina, cerrando un segundo triangulo
            glVertex3f(x, y, 0.0f);	                    // left + top
        }
        glEnd();
        poTex->desActiva();
    }
    return 0;

    ////    // Lo que no se es si la normal asi esta bien
    ////    glNormal3d(0.0, 0.0, 1.0);                    // Se empieza con dos puntos
    ////    glVertex3d(x          , y         , z);       //  0       2       // Luego cada punto es el cierre de un triangulo
    ////    glVertex3d(x          , y - dIncY , z);       //  |       |
    ////    glVertex3d(x + dIncX  , y         , z);       //  |       |
    ////    glVertex3d(x + dIncX  , y - dIncY , z);       //  1       3       //
}


//--------------------------------------------------------------------------
// Rectangulo (lineas)
// (P_X, P_Y) :: Coordenada superior izquierda
//--------------------------------------------------------------------------
cstatic int	sOpenGL::rectangulo(
    eCoordRectangulo eCoord,
    GLenum eTipoLine,           // GL_FILL, GL_LINE
    glm::vec3 posOrigin,
    float fAncho, float fAlto,
    glm::vec4 vColor,
    float frotate)
{
    //------------------------------------------------------------------
    mGlKeepEstado(bLighting, GL_LIGHTING);
    mGlKeepEstado(bTextura2D, GL_TEXTURE_2D);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    //------------------------------------------------------------------
    // glColor4f(vColor.r, vColor.g, vColor.b, vColor.a);
    //------------------------------------------------------------------
    sOpenGL::Act_blend(vColor);
    sOpenGL::color(vColor);
    //------------------------------------------------------------------
    float fz = posOrigin.z;
    glm::vec3 pTopLeft;
    glm::vec3 pBotRight;
    //------------------------------------------------------------------
    sOpenGL::pushMatrix();
    {
        glTranslatef(posOrigin.x, posOrigin.y, posOrigin.z);
        glRotatef(frotate, 0.0f, 0.0f, 1.0f);
        switch (eCoord)
        {
            case eCoordRectangulo::eTopLeft:
                // Centro - Esquina Superior Izquierda del rectangulo
                pTopLeft.x = 0;
                pTopLeft.y = 0;

                pBotRight.x = fAncho;
                pBotRight.y = - fAlto;
                break;

            case eCoordRectangulo::eCenter:
                // Centro - Centro del rectangulo
                pTopLeft.x = -fAncho / 2.0f;
                pTopLeft.y = -fAlto / 2.0f;

                pBotRight.x = fAncho / 2.0f;
                pBotRight.y = fAlto / 2.0f;
                break;
        }
        GLenum queBegin;
        switch (eTipoLine)
        {
            case GL_FILL:
                {
                    queBegin = GL_QUADS;
                    glBegin(GL_QUADS);      // GL_TRIANGLE_STRIP
                    {
                        glNormal3f(0.0f, 0.0f, 1.0f);

                        glVertex3f(pTopLeft.x , pTopLeft.y, fz);
                        glVertex3f(pBotRight.x, pTopLeft.y, fz);
                        glVertex3f(pBotRight.x, pBotRight.y, fz);
                        glVertex3f(pTopLeft.x , pBotRight.y, fz);
                    }
                    glEnd();
                }
                break;

            case GL_LINE:
                {
                    queBegin = GL_LINE_LOOP;
                    glBegin(GL_LINE_LOOP);
                    {
                        glVertex3f(pTopLeft.x, pTopLeft.y, fz);
                        glVertex3f(pBotRight.x, pTopLeft.y, fz);
                        glVertex3f(pBotRight.x, pBotRight.y, fz);
                        glVertex3f(pTopLeft.x, pBotRight.y, fz);
                        glVertex3f(pTopLeft.x, pTopLeft.y, fz);
                    }
                    glEnd();
                }
                break;
        }
    }
    sOpenGL::popMatrix();
    //------------------------------------------------------------------
    sOpenGL::Des_blend();
    mGlRecEstado(bLighting, GL_LIGHTING);
    mGlRecEstado(bTextura2D, GL_TEXTURE_2D);

    return 0;
}


//--------------------------------------------------------------------------
//int	sOpenGL::Circulo(
//    const v3d& posCentro,
//    float p_dRadio, float p_dInterno,
//    int iSegmentos,
//    int iColor,
//    float dInicio, float dAngle,
//    bool bDiscontinuo)
//{
//    miTrue(iColor == cNO_COLOR);
//
//    int			i;
//    float		dRadioExt, dRadioInt;
//    float		dStep, dAngulo, fX, fY, fZ;
//    float      dMeridiano;
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
//        sOpenGL::pushMatrix();
//        {
//            float x = posCentro.x;
//            float y = posCentro.y;
//            float z = posCentro.z;
//
//            glTranslated(x, y, z);
//
//            glRotated(dInicio, 0.0f, 0.0f, 1.0f);   // Eje Z.
//            glRotated(dAngle, 1.0f, 0.0f, 0.0f);    // Eje X.
//
//            bool bLighting = sOpenGL::disable(GL_LIGHTING);
//            bool bTextura2D = sOpenGL::disable(GL_TEXTURE_2D);
//            bool bCullFace = sOpenGL::disable(GL_CULL_FACE);
//            bool bLineStipple = (bDiscontinuo) ?
//                sOpenGL::enable(GL_LINE_STIPPLE) :
//                false;
//            int iCurColor = cColor::getCurrentColor();
//            float fWidthLines = sOpenGL::getFloat(GL_LINE_WIDTH);
//
//            if (bDiscontinuo)
//                //glLineStipple(1, 0x8888);
//                //glLineStipple(1, 0x8080);
//                //glLineStipple(1, 0x8000);
//                glLineStipple(1, 0xC000);
//
//            glLineWidth(sOpenGL::getMinWidthLineas());
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
//            sOpenGL::restore(bCullFace, GL_CULL_FACE);
//            sOpenGL::restore(bLighting, GL_LIGHTING);
//            sOpenGL::restore(bTextura2D, GL_TEXTURE_2D);
//            sOpenGL::restore(bLineStipple, GL_LINE_STIPPLE);
//            glLineWidth(fWidthLines);
//        }
//        sOpenGL::popMatrix();
//    }
//    return 0;
//}
//--------------------------------------------------------------------------


//----------------------------------------------------------------------
cstatic int sOpenGL::Act_blend(glm::vec4 vColor, GLenum z_sfactor, GLenum z_dfactor, bool bActiva)
{
    if (bActiva)
    {
        float fAlfa = vColor.a;
        m_bTransparencia = false;
        if (fAlfa < 1.0f)
        {
            if (!glIsEnabled(GL_BLEND))
            {
                glEnable(GL_BLEND);
                m_bTransparencia = true;		// Hay que deshabilitar la transparencia.
            }
            glBlendFunc(z_sfactor, z_dfactor);
        }
    }
    return 0;
}

cstatic int sOpenGL::Act_blend(GLenum z_sfactor, GLenum z_dfactor, bool bActiva)
{
    if (bActiva)
    {
        m_bTransparencia = false;
        if (!glIsEnabled(GL_BLEND))
        {
            glEnable(GL_BLEND);
            m_bTransparencia = true;		// Hay que deshabilitar la transparencia.
        }
        glBlendFunc(z_sfactor, z_dfactor);
    }
    return 0;
}

cstatic int sOpenGL::Des_blend(bool bActiva)
{
    if (bActiva)
    {
        if (m_bTransparencia)
            glDisable(GL_BLEND);
        m_bTransparencia = false;
    }
    else
    {
        if (glIsEnabled(GL_BLEND))
        {
            glDisable(GL_BLEND);
            m_bTransparencia = true;
        }
    }
    return 0;
}

//----------------------------------------------------------------------
cstatic int sOpenGL::Act_depth(bool bActiva)
{
    if (bActiva)
    {
        if (!glIsEnabled(GL_DEPTH_TEST))
        {
            glEnable(GL_DEPTH_TEST);
            m_bProfundidad = true;		// Hay que deshabilitar la profundidad.
        }
    }
    else
    {
        if (m_bProfundidad)
        {
            glEnable(GL_DEPTH_TEST);
            m_bProfundidad = false;
        }
    }
    return 0;
}

cstatic int sOpenGL::Des_depth(bool bActiva)
{
    if (bActiva)
    {
        if (m_bProfundidad)
        {
            glDisable(GL_DEPTH_TEST);
            m_bProfundidad = false;
        }
    }
    else
    {
        if (glIsEnabled(GL_DEPTH_TEST))
        {
            glDisable(GL_DEPTH_TEST);
            m_bProfundidad = true;
        }
    }
    return 0;
}

//----------------------------------------------------------------------
cstatic int sOpenGL::Act_tex2d(bool bActiva)
{
    if (bActiva)
    {
        if (!glIsEnabled(GL_TEXTURE_2D))
        {
            glEnable(GL_TEXTURE_2D);
            m_bTextura2D = true;
        }
    }
    else
    {
        if (m_bTextura2D)
        {
            glEnable(GL_TEXTURE_2D);
            m_bTextura2D = false;
        }
    }
    return 0;
}

cstatic int sOpenGL::Act_tex2d(int iTex, bool bActiva)
{
    if (bActiva)
    {
        if (iTex > 0)
        {
            if (!glIsEnabled(GL_TEXTURE_2D))
            {
                glEnable(GL_TEXTURE_2D);
                m_bTextura2D = true;		// Hay que deshabilitar la textura luego.
            }
            else
            {
                m_bTextura2D = false;
            }
            glBindTexture(GL_TEXTURE_2D, iTex);
        }
    }
    else
    {
        if (iTex > 0)
        {
            if (m_bTextura2D)
            {
                glEnable(GL_TEXTURE_2D);
                m_bTextura2D = false;
            }
            glBindTexture(GL_TEXTURE_2D, iTex);
        }
    }
    return 0;
}

cstatic int sOpenGL::Des_tex2d(bool bActiva)
{
    if (bActiva)
    {
        if (m_bTextura2D)
        {
            glDisable(GL_TEXTURE_2D);
            m_bTextura2D = false;
        }
    }
    else
    {
        if (glIsEnabled(GL_TEXTURE_2D))
        {
            glDisable(GL_TEXTURE_2D);
            m_bTextura2D = true;
        }
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    return 0;
}

cstatic bool sOpenGL::estaActivaTex2D(void)
{
    m_bTextura2D = glIsEnabled(GL_TEXTURE_2D);
    return m_bTextura2D;
}

//--------------------------------------------------------------------------
// Activamos / Desactivamos : Culling : GL_CULL_FACE.
//
// Condiciones por defecto:
//  glCullFace(GL_BACK);		// GL_BACK: borrara la trasera.
//  glFrontFace(GL_CCW);		// GL_CCW counterclockwise: indica la cara frontal: en sentido contrario a las agujas del reloj
//----------------------------------------------------------------------
cstatic int sOpenGL::Act_cull(void)
{
    if (!glIsEnabled(GL_CULL_FACE))
    {
        glEnable(GL_CULL_FACE);		// Elimina el calculo de caras posteriores
        m_bCullFace = true;
    }
    return 0;
}

cstatic int sOpenGL::Des_cull(void)
{
    if (m_bCullFace)
    {
        glDisable(GL_CULL_FACE);
        m_bCullFace = false;
    }
    return 0;
}

//----------------------------------------------------------------------
cstatic int sOpenGL::Act_lighting(void)
{
    if (!glIsEnabled(GL_LIGHTING))
    {
        glEnable(GL_LIGHTING);
    }
    return 0;
}

cstatic int sOpenGL::Des_lighting(void)
{
    if (glIsEnabled(GL_LIGHTING))
    {
        glDisable(GL_LIGHTING);
    }
    return 0;
}

//----------------------------------------------------------------------
cstatic int sOpenGL::Act_colorMaterial(void)
{
    if (!glIsEnabled(GL_COLOR_MATERIAL))
    {
        glEnable(GL_COLOR_MATERIAL);
    }
    return 0;
}

cstatic int sOpenGL::Des_colorMaterial(void)
{
    if (glIsEnabled(GL_COLOR_MATERIAL))
    {
        glDisable(GL_COLOR_MATERIAL);
    }
    return 0;
}
//----------------------------------------------------------------------


//--------------------------------------------------------------------------
// Funciones de Color:
// - ...
// - quizas pendiente de hacer un gestor de colores.
// - ¿ dependen del color material ?
//--------------------------------------------------------------------------
cstatic int sOpenGL::color(glm::vec4 vColor)
{
    if (vColor.r != -1.0f && vColor.g != -1.0f && vColor.b != -1.0f && vColor.a != -1.0f)
    {
        glColor4f(vColor.r, vColor.g, vColor.b, vColor.a);
        m_vCurrentColor = vColor;
    }
    return 0;
}

cstatic glm::vec4 sOpenGL::getColor()
{
    float vValor[4];
    glGetFloatv(GL_CURRENT_COLOR, vValor);
    m_vCurrentColor.r = vValor[0];
    m_vCurrentColor.g = vValor[1];
    m_vCurrentColor.b = vValor[2];
    m_vCurrentColor.a = vValor[3];
    return m_vCurrentColor;
}
//--------------------------------------------------------------------------


//----------------------------------------------------------------------
// Control final de resolucion
// Centralizamos los valores finales segun las configuraciones de las
// ventanas y sus resolcuiones.
//----------------------------------------------------------------------
void sOpenGL::SetCoordType(eTipoCoord eTipo)
{
    m_eTipoCoord = eTipo;
}


float sOpenGL::SetSentidoCoordY(float y, float despY)
{
    float fY;
    if (m_eTipoCoord == eTipoCoord::eDRisk)
    {
        fY = y + despY;
    }
    else
    {
        // Lo que estaba en cFuente
        // m_iY = iYp - (getAltura(poWindow) + 1);
        fY = y - despY;
    }
    return fY;
}

//
// static float DarY(float dY, int dimHeightRisk = RISK_HEIGHT, int dimFinalHeight = -1);
//
float sOpenGL::DarY(float pY, int dimHeightRisk)
{
    // Por ahora esta pensado para el risk. 640x480, y la Y en sentido
    // contrario a OpenGL
    float dY;

    if (m_eTipoCoord == eTipoCoord::eDRisk)
    {
        dY = dimHeightRisk - pY;
    }
    else
    {
        dY = pY;
    }

    dY = CalcDimFinalY(dY, dimHeightRisk);

    return dY;
}


//
// static float DarX(float dX, int dimWidthRisk = RISK_WIDTH, int dimFinalWidth = -1);
//
float sOpenGL::DarX(float pX, int dimWidthRisk)
{
    // Por ahora esta pensado para el risk. 640x480, y la Y en sentido
    // contrario a OpenGL
    float dX;

    dX = pX;
    dX = CalcDimFinalX(dX, dimWidthRisk);

    return dX;
}


float sOpenGL::CalcDimFinalX(float pX, int dimWidthRisk)
{
    float dX = pX;
    // si con dimWidthRisk, tengo x, con dimFinalWidth, tendre y
    if (dimWidthRisk != sOpenGL::m_xScreen)
    {
        dX = (dX * sOpenGL::m_xScreen) / dimWidthRisk;
    }
    return dX;
}


float sOpenGL::CalcDimFinalY(float pY, int dimHeightRisk)
{
    float dY = pY;
    if (dimHeightRisk != sOpenGL::m_yScreen)
    {
        dY = (dY * sOpenGL::m_yScreen) / dimHeightRisk;
    }
    return dY;
}


//--------------------------------------------------------------------------
// Funciones de activacion y desativacion
// Guardan el estado, y con Restore se restaura el valor anterior
//--------------------------------------------------------------------------
cstatic bool sOpenGL::enable(GLenum glItem)
{
    GLboolean bFlag = glIsEnabled(glItem);
    glEnable(glItem);
    return !!bFlag;
}


cstatic bool sOpenGL::disable(GLenum glItem)
{
    GLboolean bFlag = glIsEnabled(glItem);
    glDisable(glItem);
    return !!bFlag;
}


cstatic void sOpenGL::restore(bool bFlag, GLenum glItem)
{
    if (bFlag)
    {
        if (!glIsEnabled(glItem))
        {
            glEnable(glItem);
        }
    }
    else
    {
        if (glIsEnabled(glItem))
        {
            glDisable(glItem);
        }
    }
}


//--------------------------------------------------------------------------
// Y con esta, solo preguntamos por el estado,, pero no lo modifica
//--------------------------------------------------------------------------
cstatic bool sOpenGL::isEnabled(GLenum glItem)
{
    GLboolean bFlag = glIsEnabled(glItem);
    return !!bFlag;
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
    if (poAtributo)
    {
        iValor = sOpenGL::getCodigoOpenGL(poAtributo->getValor().getString());
        // Sino se encuentra se queda el valor por defecto:
        iValor = (iValor != -1) ? iValor : valorPorDefecto;
    }

    return iValor;
}



/*------------------------------------------------------------------------*\
|* Fin de sOpenGL.h
\*------------------------------------------------------------------------*/
