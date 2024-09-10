/*------------------------------------------------------------------------*\
|* sOpenGL.h
\*------------------------------------------------------------------------*/

#pragma once
/*------------------------------------------------------------------------*\
|* Includes
\*------------------------------------------------------------------------*/
#include <dll/nExport.h>
#include <GL/glew.h>
#include <GLM/glm.hpp>
/*------------------------------------------------------------------------*/


//--------------------------------------------------------------------------
// Defines / Const
//--------------------------------------------------------------------------
#ifndef RISK_HEIGHT
#define RISK_WIDTH      640
#define RISK_HEIGHT     480
#endif // !RISK_HEIGHT
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Macros
//--------------------------------------------------------------------------
// A ver si esto es mas simple
// Nos habremos guardado en la variable bool el estado de un determinado
// elemento OpenGL
// Con esta macro recuperamos el estado.
//--------------------------------------------------------------------------
#define mGlKeepEstado(bBool,glItem) \
    GLboolean bBool = glIsEnabled(glItem);
//--------------------------------------------------------------------------
#define mGlRecEstado(bBool,glItem)  \
    if (bBool)                      \
    {                               \
        if (!glIsEnabled(glItem))   \
        {                           \
            glEnable(glItem);       \
        }                           \
    }                               \
    else                            \
    {                               \
        if (glIsEnabled(glItem))    \
        {                           \
            glDisable(glItem);      \
        }                           \
    }
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// enumerados
//--------------------------------------------------------------------------
enum class eSync
{
    Finish,
    Flush
};
//--------------------------------------------------------------------------
enum class eMatrixModo
{
    eGL_MODELVIEW,
    eGL_PROJECTION,
    eGL_TEXTURE
};
//--------------------------------------------------------------------------
enum class eCoordRectangulo
{
    eTopLeft,   // Las coordenadas son las de la esquina superior izquierda
    eCenter,    // Las corrdenadas son las del centro del rectangulo
};
//--------------------------------------------------------------------------
// Estilos de coordenadas
//--------------------------------------------------------------------------
enum class eTipoCoord
{
    eDNormal = 0,
    eDRisk,       // Para compatibilizar las coordenadas del Antiguo Risk
};
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Struct
//--------------------------------------------------------------------------
class cAtributo;
class cTextura;
struct _mExport sOpenGL
{
    //----------------------------------------------------------------------
    // Variables
    //----------------------------------------------------------------------
    static bool m_bIniciado;
    static int m_iLevelMatrixVista;
    static int m_iLevelMatrixProyeccion;
    static int m_iLevelMatrixTextura;
    static eMatrixModo m_eMatrixModo;

    static bool m_bTransparencia;
    static bool m_bProfundidad;
    static bool m_bTextura2D;

    static bool m_bCullFace;
    static GLenum m_eCullFaceMode;
    static GLenum m_eFrontFace;

    static float m_vfSize_puntos[2];
    static float m_fInc_puntos;

    static float m_vfWidth_lineas[2];
    static float m_fInc_lineas;

    static glm::vec4 m_vCurrentColor;

    static eTipoCoord m_eTipoCoord;

    static int m_xScreen;
    static int m_yScreen;


    //----------------------------------------------------------------------
    // Metodos
    //----------------------------------------------------------------------
    static int initOpenGL();
    static int inicioLuces(glm::vec4 vAmbientColor);
    static void syncro(eSync sync);
    static void toggle_Line_Fill();
    //----------------------------------------------------------------------
    static eMatrixModo pushMatrix(eMatrixModo eModo = eMatrixModo::eGL_MODELVIEW);
    static eMatrixModo popMatrix(eMatrixModo eModo = eMatrixModo::eGL_MODELVIEW);
    static int getLevelPushMatrix(eMatrixModo eModo = eMatrixModo::eGL_MODELVIEW);
    static void setMatrixModo(eMatrixModo eModo);
    static eMatrixModo getMatrixModo(void);
    //----------------------------------------------------------------------
    //static cMatrix getMatrix(eMatrixModo eModo = eMatrixModo::eGL_MODELVIEW);
    //----------------------------------------------------------------------
    static void circulo(glm::vec3 centro, float fRadio, glm::vec4 vColor, float fRadioInterno = 0);
    static void flecha(glm::vec3 centro, float fradio, float fdir, glm::vec4 vColor);
    static int	textura(float x, float y, float z, float dAncho, float dAlto, cTextura* poTex);
    static int	rectangulo(eCoordRectangulo eCoord, GLenum eTipoLine, glm::vec3 posOrigin, float dAncho, float dAlto, glm::vec4 vColor,
        float frotate = 0.0f);
    //----------------------------------------------------------------------
    // 
    //----------------------------------------------------------------------
    static int  Act_blend(glm::vec4 vColor, GLenum z_sfactor = GL_SRC_ALPHA, GLenum z_dfactor = GL_ONE_MINUS_SRC_ALPHA, bool p_bActiva = true);
    static int  Act_blend(GLenum z_sfactor = GL_SRC_ALPHA, GLenum z_dfactor = GL_ONE_MINUS_SRC_ALPHA, bool p_bActiva = true);
    static int  Des_blend(bool p_bActiva = true);
    //----------------------------------------------------------------------
    static int  Act_depth(bool p_bActiva = true);
    static int  Des_depth(bool p_bActiva = true);
    //----------------------------------------------------------------------
    static int  Act_tex2d(bool bActiva = true);
    static int  Act_tex2d(int iTex, bool bActiva = true);
    static int  Des_tex2d(bool bActiva = true);
    static bool estaActivaTex2D(void);
    //----------------------------------------------------------------------
    static int  Act_cull(void);
    static int  Des_cull(void);
    //----------------------------------------------------------------------
    static int  Act_lighting(void);
    static int  Des_lighting(void);
    //----------------------------------------------------------------------
    static int  Act_colorMaterial(void);
    static int  Des_colorMaterial(void);
    //----------------------------------------------------------------------
    static int color(glm::vec4 vColor);
    static glm::vec4 getColor();
    //----------------------------------------------------------------------

    //----------------------------------------------------------------------
    static void SetCoordType(eTipoCoord eTipo);
    static float SetSentidoCoordY(float y, float despY);
    static float DarX(float fX, int dimWidthRisk = RISK_WIDTH);
    static float DarY(float fY, int dimHeightRisk = RISK_HEIGHT);
    static float CalcDimFinalX(float pX, int dimWidthRisk);
    static float CalcDimFinalY(float pY, int dimHeightRisk);
    //----------------------------------------------------------------------

    //----------------------------------------------------------------------
    static bool enable(GLenum glItem);
    static bool disable(GLenum glItem);
    static void restore(bool bFlag, GLenum glItem);
    static bool isEnabled(GLenum glItem);
    //----------------------------------------------------------------------

    //----------------------------------------------------------------------
    static int getCodigoOpenGL(const char* pcNombreOpenGL);
    static const char* getNombreOpenGL(int iCodigoOpenGL);
    static int getValorAtributo(cAtributo* poAtributo, int valorPorDefecto);
    //----------------------------------------------------------------------
};
//--------------------------------------------------------------------------


/*------------------------------------------------------------------------*\
|* Fin de sOpenGL.h
\*------------------------------------------------------------------------*/
