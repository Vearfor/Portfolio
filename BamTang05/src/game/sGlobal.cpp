/*------------------------------------------------------------------------*\
|* sGlobal.cpp
\*------------------------------------------------------------------------*/

#include "sGlobal.h"

/*------------------------------------------------------------------------*\
|* Statics
\*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*\
|* Minima 'dimensión'/tamaño del laberinto. No se permite por debajo de
|* este valor.
\*------------------------------------------------------------------------*/
int sGlobal::m_iDim = 5;

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
|* Para el nombre del programa
\*------------------------------------------------------------------------*/
char sGlobal::vcNombrePrograma[] = "";

/*------------------------------------------------------------------------*\
|* Cuando la Demo este activa, el tiempo entre orden y orden al cubo
|* dentro del laberinto se lanza cada cierto tiempo
|* Por ejemplo: 0.25, es cad 0.25 segundos.
\*------------------------------------------------------------------------*/
float sGlobal::m_fIntervaloDemo = 0.25f;
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*\
|* Fin de sGlobal.cpp
\*------------------------------------------------------------------------*/
