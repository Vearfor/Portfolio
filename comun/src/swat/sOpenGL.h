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
// Defines
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Macros
//--------------------------------------------------------------------------
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


//--------------------------------------------------------------------------
// Struct
//--------------------------------------------------------------------------
class cAtributo;
class cTextura;
struct _mExport sOpenGL
{
    static bool m_bIniciado;
    static int m_iLevelMatrixVista;
    static int m_iLevelMatrixProyeccion;
    static int m_iLevelMatrixTextura;
    static eMatrixModo m_eMatrixModo;
    static float m_vfWidth_lineas[2];
    static float m_fInc_lineas;

    static int initOpenGL();
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
    static int	textura(double x, double y, double z, double dAncho, double dAlto, cTextura* p_poTex);

    static int getCodigoOpenGL(const char* pcNombreOpenGL);
    static const char* getNombreOpenGL(int iCodigoOpenGL);
    static int getValorAtributo(cAtributo* poAtributo, int valorPorDefecto);
};
//--------------------------------------------------------------------------


/*------------------------------------------------------------------------*\
|* Fin de sOpenGL.h
\*------------------------------------------------------------------------*/
