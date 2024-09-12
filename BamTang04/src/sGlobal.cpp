/*------------------------------------------------------------------------*\
|* sGlobal.cpp
\*------------------------------------------------------------------------*/
#include "sGlobal.h"


/*========================================================================*\
|* Statics
\*========================================================================*/
/*------------------------------------------------------------------------*\
|* m_iWidth y m_iHeight: son el ancho y el alto iniciales de la ventana
|* principal.
|* Estos permanecen fijos. Luego el evento OnSize de la ventana podria
|* tomar nuevos valores.
|* Por ello se toman los valores directamente de la ventana.
\*------------------------------------------------------------------------*/
int sGlobal::m_iWidth = 1024;
int sGlobal::m_iHeight = 768;

/*------------------------------------------------------------------------*\
|* Nivel de informacion que se muestra en la esquina superior izquierda.
|* Con 0: evidentemente no se muestra nada
|*     1: Velocidad Incial de las bolas al ejecutar con Space 'shootBall'
|*        Dir: o direccion del Origen de Disparos. Direccion con la cual
|*        se lanzaran las bolas.
|*     2:
|* Estos permanecen fijos. Luego el evento OnSize de la ventana podria
|* tomar nuevos valores.
|* Por ello se toman los valores directamente de la ventana.
\*------------------------------------------------------------------------*/
int sGlobal::m_imostrarInfo = 3;
bool sGlobal::m_bmostrarFps = true;
float sGlobal::m_dirOrigen = 40.0f;
glm::vec2 sGlobal::m_posOrigen{ (float)(sGlobal::m_iWidth / 2), (float)(sGlobal::m_iHeight / 4) };
glm::vec4 sGlobal::m_colorOrigen{ 0.3f, 1.0f, 0.1f, 1.0f };
glm::vec4 sGlobal::m_colorFlecha{ 1.0f, 0.0f, 0.0f, 1.0f };
std::string sGlobal::m_sTexturaOrigen{ "" };
float sGlobal::m_fRadioOrigen = 15.0f;
float sGlobal::m_fRadio = 15.0f;
glm::vec4 sGlobal::m_colorBolas{ 1.0f, 0.0f, 0.0f, 1.0f };
/*------------------------------------------------------------------------*/
int sGlobal::m_limBolas = 60;
/*------------------------------------------------------------------------*/
float sGlobal::m_fVelocidadInicial = 150.0f;
float sGlobal::m_fVelocidadMin = 30.0f;
float sGlobal::m_fVelocidadMax = 500.0f;
/*------------------------------------------------------------------------*/
bool sGlobal::m_hayGravedad = true;
bool sGlobal::m_hayFriccion = true;
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
float sGlobal::m_fVelParada = 1.0f;
float sGlobal::m_fTiempoDestruccion = 20.0f;
float sGlobal::m_fTiempoExplosion = 10.0f;
/*------------------------------------------------------------------------*/
bool sGlobal::m_hayChoqueOrigen = false;
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*\
|* Fin de sGlobal.cpp
\*------------------------------------------------------------------------*/
