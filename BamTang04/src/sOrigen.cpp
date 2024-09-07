/*------------------------------------------------------------------------*\
|* sOrigen.cpp
\*------------------------------------------------------------------------*/

#include "sOrigen.h"
#include "sCollisionSystem.h"
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


coverride int sOrigen::checkLimites(float fDeltaTime, int width, int height)
{
    glm::vec2 inc = sBall::calcIncremento(fDeltaTime, m_vecVelocidad);

    if (m_posicion.x + inc.x > width - (2.0f * m_radio))
        m_vecVelocidad.x = -m_vecVelocidad.x * sGlobal::m_fElasticidad;

    if (m_posicion.x + inc.x < m_radio)
        m_vecVelocidad.x = -m_vecVelocidad.x * sGlobal::m_fElasticidad;

    if (m_posicion.y + inc.y > height - (sGlobal::m_windowCaptionSize + m_radio))
        m_vecVelocidad.y = -m_vecVelocidad.y * sGlobal::m_fElasticidad;

    if (m_posicion.y + inc.y < m_radio)
        m_vecVelocidad.y = -m_vecVelocidad.y * sGlobal::m_fElasticidad;
        //m_vecVelocidad = glm::vec2(0.0f, 0.0f);

    return 0;

    //glm::vec2 inc = (m_vecVelocidad * fDeltaTime);
    //if (
    //    (m_posicion.x + inc.x > width - (2.0f*m_radio)) ||
    //    (m_posicion.x + inc.x < m_radio) ||
    //    (m_posicion.y + inc.y > height - sGlobal::m_windowCaptionSize) ||
    //    (m_posicion.y + inc.y < m_radio)
    //    )
    //{
    //    m_vecVelocidad = glm::vec2(0.0f, 0.0f);
    //}
}


coverride int sOrigen::render()
{
    sOpenGL::flecha(glm::vec3(m_posicion.x, m_posicion.y, 0.0f), m_radio, m_fdir, m_vColorFlecha);
    sBall::render();
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
