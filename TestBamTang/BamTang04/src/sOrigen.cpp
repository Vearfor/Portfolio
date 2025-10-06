/*------------------------------------------------------------------------*\
|* sOrigen.cpp
\*------------------------------------------------------------------------*/

#include "sOrigen.h"
#include "sGlobal.h"
#include <tool/nComun.h>
#include <swat/sOpenGL.h>
#include <tool/sMath.h>
#include <tool/cLog.h>


/*------------------------------------------------------------------------*\
|* Constructor y Destructor
\*------------------------------------------------------------------------*/
sOrigen::sOrigen()
{
}

sOrigen::~sOrigen()
{
    cLog::print(" Destructor sOrigen\n");
}
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*\
|* Modificamos por colision la direccion y velocidad de una bola
|*
|* void sCollisionSystem::modificoBola(sBall* pBall, float oldVel, float fdir)
|*
\*------------------------------------------------------------------------*/
coverride void sOrigen::modifyByCollision(float oldVel, float fdir)
{
    if (sGlobal::m_hayChoqueOrigen)
    {
        float fVel, xVel, yVel;

        fVel = sMath::aplicaFactor(oldVel, sGlobal::m_fElasticidad);
        xVel = fVel * cos(deg2rad(fdir));
        yVel = fVel * sin(deg2rad(fdir));

        m_fdir = fdir;
        m_vecVelocidad = glm::vec2{ xVel, yVel };
    }
}


/*------------------------------------------------------------------------*\
|* update especifico del Origen de Disparos
\*------------------------------------------------------------------------*/
coverride int sOrigen::update(float fDeltaTime)
{
    // Somos la bola origen: no explotamos
    // quitado el explotando de sBall:

    // Cuando esta cerca del cero, pone a ceros, el problema es que cEpsilon
    // no estan pequeño, no esta siendo inferior de 0.4 o por ahí.
    sMath::setZero(m_vecVelocidad);
    if (!sMath::isZero(m_vecVelocidad))
    {
        // Especifico del Origen de Disparos:
        if (sGlobal::m_hayChoqueOrigen)
        {
            aplicoGravedad(fDeltaTime);
            aplicoFriccionAire(fDeltaTime);
        }

        glm::vec2 inc = sBall::calcIncremento(fDeltaTime, m_vecVelocidad);
        m_posicion = m_posicion + inc;

        checkPosicion(getWidth(), getHeight());
        // checkParada(fDeltaTime);
    }
    else
    {
        // La dejamos: ya sabemos que en sOrigen, no hace nada.
        // Se queda ahí por si acaso cambia la cosa
        matarBola(fDeltaTime);
    }

    return 0;
}


coverride int sOrigen::render()
{
    sOpenGL::flecha(glm::vec3(m_posicion.x, m_posicion.y, 0.0f), m_radio, m_fdir, m_vColorFlecha);
    // En la generica, se decide si estamos explotando,
    // en la de Origen de los Disparos no se explota.
    render_normal();
    return 0;
}

coverride int sOrigen::checkParada(float fDeltaTime)
{
    return sBall::checkParada(fDeltaTime);
}


/*------------------------------------------------------------------------*\
|* Facil para el Origen de Datos: nunca muere. no tiene que hacer nada
|* para el Origen de datos
\*------------------------------------------------------------------------*/
coverride int sOrigen::matarBola(float fDeltaTime)
{
    // Pues lo dicho, no se hace nada.
    return 0;
}


/*------------------------------------------------------------------------*\
|* Cambiamos la direccion de inicio de las bolas
|* - He hecho que la fDir de inicio de las bolas sea la fDir de las bolas
|*   nuevas cuando se lanzan.
\*------------------------------------------------------------------------*/
void sOrigen::cambiaDir(eIncr incr, float fDeltaTime)
{
    float finc = 100.0f * fDeltaTime;
    switch (incr)
    {
    case eIncr::eSuma:
        m_fdir += finc;
        if (m_fdir > 360.0f - finc)
            m_fdir = 0.0f;
        break;

    case eIncr::eResta:
        m_fdir -= finc;
        if (m_fdir < 0.0f)
            m_fdir = 360.0f - finc;
        break;
    }
}


/*------------------------------------------------------------------------*\
|* Cambiamos la velocidad de inicio de las bolas:
|* - lo podria haber hecho en cualquier parte, ya que solo modifica 
|*   variables globales que no son de sOrigen.
\*------------------------------------------------------------------------*/
void sOrigen::cambiaVel(eIncr incr, float fDeltaTime)
{
    // Tenia que ver con origen,
    // aunque si, se podria haber puesto en cualquier parte
    float finc = 100.0f * fDeltaTime;
    switch (incr)
    {
    case eIncr::eSuma:
        sGlobal::m_fVelocidadInicial += finc;
        if (sGlobal::m_fVelocidadInicial > sGlobal::m_fVelocidadMax)
            sGlobal::m_fVelocidadInicial = sGlobal::m_fVelocidadMax;
        break;

    case eIncr::eResta:
        sGlobal::m_fVelocidadInicial -= finc;
        if (sGlobal::m_fVelocidadInicial < sGlobal::m_fVelocidadMin)
            sGlobal::m_fVelocidadInicial = sGlobal::m_fVelocidadMin;
        break;
    }
}


/*------------------------------------------------------------------------*\
|* Update por Teclado
|* - izquierda
|* - derecha
|* - arriba
|* - abajo
\*------------------------------------------------------------------------*/
float sfInc = 1000.0f;
/*------------------------------------------------------------------------*/
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
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*\
|* Fin de sOrigen.cpp
\*------------------------------------------------------------------------*/
