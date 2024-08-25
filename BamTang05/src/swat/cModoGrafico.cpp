/*========================================================================*\
|* cModoGrafico.cpp
\*========================================================================*/


#include "cModoGrafico.h"
#include "../tool/nComun.h"


//--------------------------------------------------------------------------
// Constructor & Destructor
//--------------------------------------------------------------------------
cModoGrafico::cModoGrafico(void)
{
    resetModoGrafico();
}


cModoGrafico::~cModoGrafico(void)
{
}


//--------------------------------------------------------------------------
// Resto de metodos
//--------------------------------------------------------------------------
void cModoGrafico::resetModoGrafico(void)
{
    memset(&m_tDevMode, 0, sizeof(DEVMODE));
    m_tDevMode.dmSize = sizeof(DEVMODE);
    // Desconozco que voy a utilizar y que tamaño poner:
    m_tDevMode.dmDriverExtra = LON_BUFF / 4;
}



/*========================================================================*\
|* Fin de cModoGrafico.cpp
\*========================================================================*/
