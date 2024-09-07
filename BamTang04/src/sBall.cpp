/*------------------------------------------------------------------------*\
|* sBall.cpp
\*------------------------------------------------------------------------*/

#include "sBall.h"
#include "sGlobal.h"
#include "sCollisionSystem.h"
#include "sGame.h"
#include <swat/sOpenGL.h>
#include <tool/sMath.h>
#include <tool/cLog.h>


/*------------------------------------------------------------------------*\
|* Statics
\*------------------------------------------------------------------------*/
bool sBall::m_hayGravedad = true;
bool sBall::m_hayFriccion = true;
int sBall::s_countBolas = 0;
/*------------------------------------------------------------------------*/


sBall::sBall()
{
    m_bolaId = s_countBolas++;
}

sBall::~sBall()
{
    cLog::print(" Destructor sBall\n");
}


//--------------------------------------------------------------------------
// Chequea los limites
// - pero solo cambia velocidades
//--------------------------------------------------------------------------
int sBall::checkLimites(float fDeltaTime, int width, int height)
{
    glm::vec2 inc = sBall::calcIncremento(fDeltaTime, m_vecVelocidad);

    if (m_posicion.x + inc.x > width - (2.0f*m_radio))
        m_vecVelocidad.x = -m_vecVelocidad.x * sGlobal::m_fElasticidad;

    if (m_posicion.x + inc.x < m_radio)
        m_vecVelocidad.x = -m_vecVelocidad.x * sGlobal::m_fElasticidad;

    if (m_posicion.y + inc.y > height - (m_radio + sGlobal::m_windowCaptionSize))
        m_vecVelocidad.y = -m_vecVelocidad.y * sGlobal::m_fElasticidad;

    if (m_posicion.y + inc.y < m_radio)
        m_vecVelocidad.y = -m_vecVelocidad.y * sGlobal::m_fElasticidad;

    return 0;
}


//--------------------------------------------------------------------------
// Chequea los limites
// - pero no deja que la posicion cambie fuera de los limites
// + Si no lo impedimos terminan por salirse fuera.
//--------------------------------------------------------------------------
int sBall::checkPosicion(int width, int height)
{
    if (m_posicion.x > width - (2.0f * m_radio))
        m_posicion.x = width - (2.0f * m_radio);

    if (m_posicion.x < m_radio)
        m_posicion.x = m_radio;

    if (m_posicion.y > height - sGlobal::m_windowCaptionSize)
        m_posicion.y = (float)(height - sGlobal::m_windowCaptionSize);

    if (m_posicion.y < m_radio)
        m_posicion.y = m_radio;

    return 0;
}


//--------------------------------------------------------------------------
// Forzamos la parada
// A partir del valor dado por sGlobal::m_fVelParada;
//--------------------------------------------------------------------------
int sBall::checkParada(float fDeltaTime)
{
    //----------------------------------------------------------------------
    // Solo lo estamos haciendo en la parte del 'Suelo'
    // Y ahora lo haremos tambien si estamos colisionando
    //----------------------------------------------------------------------
    float fVel = sMath::modulo(m_vecVelocidad);
    if (
            (fVel < sGlobal::m_fVelParada) && 
            (
                (m_posicion.y < m_radio + 1.0f) ||
                (m_estaColisionando)
            )
       )
    {
        float fFactor = sMath::getFactorReduccion(3.0f, fDeltaTime);
        m_vecVelocidad *= fFactor;
        if (!m_parando)
        {
            m_parando = true;
            //cLog::print(" Empezamos a parar el circulo: %ld\n", m_bolaId);
        }
        //else
        //{
        //    cLog::print(" - %ld: %6.3f\n", m_bolaId, fVel);
        //}
        //------------------------------------------------------------------
        // A la mierda: si estamos por debajo de 1.0f, velocidad a cero:
        //------------------------------------------------------------------
        if (fVel < 1.0f)
            m_vecVelocidad = glm::vec2(0.0f, 0.0f);
        //------------------------------------------------------------------
    }
    else
    {
        m_parando = false;
        m_tiempo = 0.0f;
    }
    //----------------------------------------------------------------------
    return 0;
}


cstatic glm::vec2 sBall::calcIncremento(float fDeltaTime, glm::vec2 velBola)
{
    glm::vec2 inc = (velBola * fDeltaTime);
    return inc;
}


int sBall::update(float fDeltaTime)
{
    sMath::setZero(m_vecVelocidad);
    if (!sMath::isZero(m_vecVelocidad))
    {
        if (sBall::m_hayGravedad)
        {
            float decGravedad = sGlobal::m_fGravedad * fDeltaTime * sGlobal::m_fFactorMaximizador;
            glm::vec2 gravedad = glm::vec2(0.0, decGravedad);
            // Debe venir negativa, porque el valor de la gravedad ya me viene negativa
            m_vecVelocidad += gravedad;
        }

        if (sBall::m_hayFriccion)
        {
            float decFriccion = sGlobal::m_fFriccionAire * fDeltaTime;
            glm::vec2 friccion = { decFriccion, decFriccion };
            // Debe venir negativa, porque el valor de la friccion ya me viene negativa
            m_vecVelocidad += friccion;
        }

        glm::vec2 inc = sBall::calcIncremento(fDeltaTime, m_vecVelocidad);
        m_posicion = m_posicion + inc;

        // Hay que hacerlo en el momento: no podemos permitir que se hayan quedado fuera de los limites:
        checkPosicion(sCollisionSystem::m_iWidth, sCollisionSystem::m_iHeight);
        checkParada(fDeltaTime);
    }
    else
    {
        matarBola(fDeltaTime);
    }

    return 0;
}


int sBall::matarBola(float fDeltaTime)
{
    // Si es cero: temporizador de borrado.
    if (m_parando)
    {
        //cLog::print(" Hemos parado: %ld\n", m_bolaId);
        m_parando = false;
        m_tiempo = 0.0f;
    }

    // Solo sino eres el Origen de los disparos:
    if (m_bolaId > 0)
    {
        m_tiempo += fDeltaTime;
        //cLog::print(" - Tiempo: %6.3f\n", m_tiempo);

        // Podemos destruir todas las bolas, menos la bola de origen.
        // Despues de 20 segundos.
        m_destruir = ((m_tiempo > sGlobal::m_fTiempoDestruccion) && m_bolaId > 0);
        sGame::instancia()->setDestruccion(m_destruir);
    }

    return 0;
}


int sBall::render()
{
    float radioInt =
        (!sBall::m_hayGravedad) ? m_radio - 0.8f :
        (!sBall::m_hayFriccion) ? 0.0f :
        3.0f;

    sOpenGL::circulo(glm::vec3(m_posicion.x, m_posicion.y,  0.0f), m_radio, m_color, radioInt);
    return 0;
}


void sBall::cambiaDir(eIncrGrados incr)
{
    float finc = 3;
    switch (incr)
    {
        case eIncrGrados::eSuma:
            m_fdir += finc;
            if (m_fdir > 360.0f-finc)
                m_fdir = 0.0f;
            break;

        case eIncrGrados::eResta:
            m_fdir -= finc;
            if (m_fdir < 0.0f)
                m_fdir = 360.0f - finc;
            break;
    }
}


/*------------------------------------------------------------------------*\
|* Fin de sBall.cpp
\*------------------------------------------------------------------------*/
