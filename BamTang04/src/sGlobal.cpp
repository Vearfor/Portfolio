/*------------------------------------------------------------------------*\
|* sGlobal.cpp
\*------------------------------------------------------------------------*/

#include "sGlobal.h"


/*------------------------------------------------------------------------*\
|* Statics
\*------------------------------------------------------------------------*/
int sGlobal::m_iWidth = 1024;
int sGlobal::m_iHeight = 768;
glm::vec2 sGlobal::m_posOrigen{ (float)(sGlobal::m_iWidth / 2), (float)(sGlobal::m_iHeight / 4) };
glm::vec4 sGlobal::m_colorOrigen{ 0.3f, 1.0f, 0.1f, 1.0f };
glm::vec4 sGlobal::m_colorFlecha{ 1.0f, 0.0f, 0.0f, 1.0f };
std::string sGlobal::m_sTexturaOrigen{ "" };
float sGlobal::m_fRadioOrigen = 15.0f;
float sGlobal::m_fRadio = 15.0f;
glm::vec4 sGlobal::m_colorBolas{ 1.0f, 0.0f, 0.0f, 1.0f };
float sGlobal::m_fVelocidadInicial = 150.0f;
int sGlobal::m_limBolas = 60;
/*------------------------------------------------------------------------*/
float sGlobal::m_fGravedad      = - 9.8f;     // aumenta 9.8 metros por segundo
float sGlobal::m_fFriccionAire  = - 1.0f;     // quita 1.0 metros por segundo
/*------------------------------------------------------------------------*\
|* Bajamos la velocidad en cada rebote y en cada choque con otras bolas: 
|* no debe ser mayor que 1. Daria mas velocidad en cada rebote o choque
\*------------------------------------------------------------------------*/
float sGlobal::m_fElasticidad = 0.9f;
/*------------------------------------------------------------------------*/
float sGlobal::m_fFactorMaximizador = 4.0f;         // segun toque para
float sGlobal::m_fFactorReductor = 0.1f;             // reducir o aumentar
/*------------------------------------------------------------------------*/
int sGlobal::m_windowCaptionSize = 40;
/*------------------------------------------------------------------------*/
float sGlobal::m_fVelParada = 25.0f;
float sGlobal::m_fTiempoDestruccion = 20.0f;
float sGlobal::m_fTiempoExplosion = 10.0f;
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*\
|* Fin de sGlobal.cpp
\*------------------------------------------------------------------------*/
