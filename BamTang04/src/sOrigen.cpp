/*------------------------------------------------------------------------*\
|* sOrigen.cpp
\*------------------------------------------------------------------------*/

#include "sOrigen.h"
#include "sGlobal.h"
#include <tool/nComun.h>
#include <swat/sOpenGL.h>
#include <tool/sMath.h>
#include <tool/cLog.h>


sOrigen::sOrigen()
{
}

sOrigen::~sOrigen()
{
    cLog::print(" Destructor sOrigen\n");
}


coverride int sOrigen::render(float fDeltaTime)
{
    sOpenGL::flecha(glm::vec3(m_posicion.x, m_posicion.y, 0.0f), m_radio, m_fdir, m_vColorFlecha);
    sBall::render(fDeltaTime);
    return 0;
}


float sfInc = 1000.0f;
int sOrigen::izquierda(float fDeltaTime)
{
    float finc = sfInc * fDeltaTime;

    m_vecVelocidad.x -= finc;

    return 0;
}
 

int sOrigen::derecha(float fDeltaTime)
{
    float finc = sfInc * fDeltaTime;

    m_vecVelocidad.x += finc;

    return 0;
}


int sOrigen::arriba(float fDeltaTime)
{
    float finc = sfInc * fDeltaTime;

    m_vecVelocidad.y += finc;

    return 0;
}


int sOrigen::abajo(float fDeltaTime)
{
    float finc = sfInc * fDeltaTime;

    m_vecVelocidad.y -= finc;

    return 0;
}


/*------------------------------------------------------------------------*\
|* Fin de sOrigen.cpp
\*------------------------------------------------------------------------*/
