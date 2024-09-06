/*------------------------------------------------------------------------*\
|* sGlobal.h
\*------------------------------------------------------------------------*/

#pragma once
//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include <tool/nComun.h>
#include <GLM/glm.hpp>
#include <string>
//--------------------------------------------------------------------------

#pragma region Titulo
//--------------------------------------------------------------------------
// El Titulo de la Ventana
//--------------------------------------------------------------------------
#ifdef _X64
#define PLATFORM " [64"
#else
#define PLATFORM " [32"
#endif

#ifdef _DEBUG
#define CONFIG " Debug]"
#else
#define CONFIG " Release]"
#endif

#define TITLE PLATFORM CONFIG
//--------------------------------------------------------------------------
#pragma endregion


/*------------------------------------------------------------------------*/
struct sGlobal
{
    static int m_iWidth;
    static int m_iHeight;
    // Deberiamos ser proporcionales
    // que el radio de las bolas nos parezca igual independientemente
    // de las dimensiones de la ventana
    static glm::vec2 m_posOrigen;
    static glm::vec4 m_colorOrigen;
    static glm::vec4 m_colorFlecha;
    static std::string m_sTexturaOrigen;
    static float m_fRadioOrigen;
    static float m_fRadio;
    static glm::vec4 m_colorBolas;
    static float m_fVelocidad;
    static int m_limBolas;
    /*--------------------------------------------------------------------*/
    static float m_fGravedad;
    static float m_fFriccionAire;
    static float m_fElasticidad;
    /*--------------------------------------------------------------------*\
    |* Debe reducir o aumentar los efectos de velocidad muy altos o muy
    |* bajos la gravedad o por la friccion.
    |* El tiempo al aplicarlo esta dando valores muy bajos.
    \*--------------------------------------------------------------------*/
    static float m_fFactorMaximizador;      // segun toque para
    static float m_fFactorReductor;         // reducir o aumentar
    /*--------------------------------------------------------------------*/
    static int m_windowCaptionSize;
    static float m_fVelParada;
    static float m_fTiempoDestruccion;
    /*--------------------------------------------------------------------*/
};
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*\
|* Fin de sGlobal.h
\*------------------------------------------------------------------------*/
