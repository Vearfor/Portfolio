/*------------------------------------------------------------------------*\
|* sBall.cpp
\*------------------------------------------------------------------------*/

#include "sBall.h"
#include "sGlobal.h"
#include "sGame.h"
#include "sys/sRenderSystem.h"
#include "sExplosion.h"
#include <swat/sOpenGL.h>
#include <swat/cColor.h>
#include <tool/sMath.h>
#include <tool/cLog.h>


/*------------------------------------------------------------------------*\
|* Statics
\*------------------------------------------------------------------------*/
int sBall::s_countBolas = 0;
/*------------------------------------------------------------------------*/


sBall::sBall()
{
    m_bolaId = s_countBolas++;
}

sBall::~sBall()
{
    delete m_pExplosion;
    cLog::print(" Destructor sBall\n");
}


/*------------------------------------------------------------------------*\
|* Modificamos por colision la direccion y velocidad de una bola 
|*
|* void sCollisionSystem::modificoBola(sBall* pBall, float oldVel, float fdir)
|*
\*------------------------------------------------------------------------*/
void sBall::modifyByCollision(float oldVel, float fdir)
{
    float fVel, xVel, yVel;

    fVel = sMath::aplicaFactor(oldVel, sGlobal::m_fElasticidad);

    xVel = fVel * cos(deg2rad(fdir));
    yVel = fVel * sin(deg2rad(fdir));

    m_fdir = fdir;
    m_vecVelocidad = glm::vec2{ xVel, yVel };
}


//--------------------------------------------------------------------------
// Chequea los limites
// - pero solo cambia velocidades
//--------------------------------------------------------------------------
int sBall::checkLimites(float fDeltaTime, int width, int height)
{
    glm::vec2 inc = sBall::calcIncremento(fDeltaTime, m_vecVelocidad);

    if ((m_posicion.x + inc.x > width - m_radio) || (m_posicion.x + inc.x < m_radio))
    {
        float velx = sMath::aplicaFactor(m_vecVelocidad.x, sGlobal::m_fElasticidad);
        m_vecVelocidad.x = -velx;
        // Importante: se nos olvidaba recalcular el angulo en cada cambio
        m_fdir = sMath::getAngulo(m_vecVelocidad);
    }

    if ((m_posicion.y + inc.y > height - m_radio) || (m_posicion.y + inc.y < m_radio))
    {
        float vely = sMath::aplicaFactor(m_vecVelocidad.y, sGlobal::m_fElasticidad);
        m_vecVelocidad.y = -vely;
        // Importante: se nos olvidaba recalcular el angulo en cada cambio
        m_fdir = sMath::getAngulo(m_vecVelocidad);
    }

    return 0;
}


//--------------------------------------------------------------------------
// Chequea los limites
// - pero no deja que la posicion cambie fuera de los limites
// + Si no lo impedimos terminan por salirse fuera.
// + no se como estaba funcionando con los que he visto que se quedaban
//   fuera ??
//--------------------------------------------------------------------------
int sBall::checkPosicion(int width, int height)
{
    if (m_posicion.x > width - m_radio)
        m_posicion.x = (float)(width - m_radio);

    if (m_posicion.x < m_radio)
        m_posicion.x = m_radio;

    if (m_posicion.y > height)
        m_posicion.y = (float)(height - m_radio);

    if (m_posicion.y < m_radio)
        m_posicion.y = m_radio;

    return 0;
}


/*------------------------------------------------------------------------*\
|* update generico de las bolas
\*------------------------------------------------------------------------*/
int sBall::update(float fDeltaTime)
{
    if (m_explotando)
    {
        if (m_pExplosion->update(fDeltaTime))
        {
            // Cuando termine el tiempo de explosion:
            m_destruir = true;
            sGame::instancia()->setDestruccion(m_destruir);
        }
        return 0;
    }

    sMath::setZero(m_vecVelocidad);
    if (!sMath::isZero(m_vecVelocidad))
    {
        aplicoGravedad(fDeltaTime);
        aplicoFriccionAire(fDeltaTime);

        glm::vec2 inc = sBall::calcIncremento(fDeltaTime, m_vecVelocidad);
        m_posicion = m_posicion + inc;

        checkPosicion(getWidth(), getHeight());
        checkParada(fDeltaTime);
    }
    else
    {
        matarBola(fDeltaTime);
    }

    return 0;
}


//--------------------------------------------------------------------------
// Forzamos la parada
// A partir del valor dado por sGlobal::m_fVelParada;
//--------------------------------------------------------------------------
int sBall::checkParada(float fDeltaTime)
{
    //----------------------------------------------------------------------
    // Y ahora lo haremos tambien si estamos colisionando
    // Estar en el suelo: (m_posicion.y < m_radio + 1.0f) ||
    //----------------------------------------------------------------------
    float fVel = sMath::modulo(m_vecVelocidad);
    if (
            fVel < sGlobal::m_fVelParada
        //&&
        //    (
        //        m_posicion.y < m_radio + 1.0f ||
        //        m_estaColisionando
        //    )
       )
    {
        if (!m_parando)
        {
            m_parando = true;
        }
        //------------------------------------------------------------------
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


int sBall::matarBola(float fDeltaTime)
{
    // Si es cero: temporizador de borrado.
    if (m_parando)
    {
        cLog::print(" Hemos parado: %ld\n", m_bolaId);
        m_parando = false;
        m_tiempo = 0.0f;
    }

    m_tiempo += fDeltaTime;
    m_explotando = (m_tiempo > sGlobal::m_fTiempoDestruccion);

    return 0;
}


int sBall::render()
{
    if (m_explotando)
        render_explosion();
    else
        render_normal();

    return 0;
}


void sBall::render_normal()
{
    float radioInt =
        (!sGlobal::m_hayGravedad) ? m_radio - 1.0f :
        (!sGlobal::m_hayFriccion) ? 0.0f :
        3.0f;

    sOpenGL::circulo(glm::vec3(m_posicion.x, m_posicion.y, 0.0f), m_radio, m_color, radioInt);

    sBall* pSelected = sGame::instancia()->getSelected();
    if (pSelected)
    {
        // Somos el seleccionado
        if (pSelected->m_bolaId == m_bolaId)
        {
            float radio = m_radio * 1.5f;
            sOpenGL::circulo(glm::vec3(m_posicion.x, m_posicion.y, 0.0f), radio, cColor::vBlanco, radio - 1.0f);
        }
    }
}


/*------------------------------------------------------------------------*\
|* Crea la explosion
|* y una vez creada renderiza la explosion segun el momento del tiempo
\*------------------------------------------------------------------------*/
void sBall::render_explosion()
{
    /*--------------------------------------------------------------------*\ 
    |* Rompemos en trozos y todos se van alejando del centro.
    |* a(t) = A
    |* v(t) = Vo + At
    |* x(t) = Xo + VoT + (1 / 2)At2
    |* Aplicamos la funcion del espacio a cada trozito:
    |* y cada trozo tendra su direccion.
    \*--------------------------------------------------------------------*/
    if (m_pExplosion)
    {
        m_pExplosion->render();
    }
    else
    {
        m_pExplosion = new sExplosion(this);
    }
    /*--------------------------------------------------------------------*/
}


/*------------------------------------------------------------------------*\
|* Unificamos el calculo del decremeto de la Gravedad
|* - para hacerlo siempre de la misma manera
\*------------------------------------------------------------------------*/
int sBall::aplicoGravedad(float fDeltaTime)
{
    if (sGlobal::m_hayGravedad)
    {
        //------------------------------------------------------------------
        // Si la velocidad fuese cero, seria correcto continuar:
        // seria el caso en el cual nos sueltan en el aire con velocidad 0
        //------------------------------------------------------------------
        // Bien: se calcula el 'decremento' por gravedad.
        // No se si quitar el 'sGlobal::m_fFactorMaximizador' ya que por culpa de los
        // tiempos fDeltaTime los valores son muy pequeños:
        // - volver a probar necesidad -
        float decGravedad = sGlobal::m_fGravedad * fDeltaTime * sGlobal::m_fFactorMaximizador;
        // Solo en el eje Y:
        glm::vec2 gravedad = glm::vec2(0.0, decGravedad);
        // Debe venir negativa, porque el valor de la gravedad ya me viene negativa
        m_vecVelocidad += gravedad;
        m_fdir = sMath::getAngulo(m_vecVelocidad);
        //------------------------------------------------------------------
    }
    return 0;
}


/*------------------------------------------------------------------------*\
|* Unificamos el calculo del decremeto de la Friccion del Aire
|* - para hacerlo siempre de la misma manera
\*------------------------------------------------------------------------*/
int sBall::aplicoFriccionAire(float fDeltaTime)
{
    if (sGlobal::m_hayFriccion)
    {
        float fVel = sMath::modulo(m_vecVelocidad);
        //------------------------------------------------------------------
        // si ya estamos en 0, poco mas podremos hacer
        // aunque no creo que llegemos a eso aqui, creo
        // De todas formas, la friccion, por definicion, solo aparece si
        // nos movemos.
        // Por lo cual sera correcto no hacer nada.
        //------------------------------------------------------------------
        if (fVel > 0.0f)
        {
            // Se calcula el 'decremento'
            float decFriccion = sGlobal::m_fFriccionAire * fDeltaTime;
            // Velocidad final: la que tiene menos el decremento (que ya es negativo):
            // el 'decremento' se 'suma': que en realidad estamos restando
            float fVelFinal = fVel + decFriccion;
            // Si menor que 0: 0.0f. La friccion habra provocado parada
            fVelFinal = (fVelFinal < 0.0f) ? 0.0f : fVelFinal;
            float xDec = fVelFinal * cos(deg2rad(m_fdir));
            float yDec = fVelFinal * sin(deg2rad(m_fdir));
            m_vecVelocidad = { xDec, yDec };
        }
        //------------------------------------------------------------------
    }
    return 0;
}


/*------------------------------------------------------------------------*\
|* Unificamos el calculo del incremento
|* - para llamar siempre a esta funcion tan simple y asegurarnos que en
|*   todas parte el incremento se calcula siempre de la misma manera
\*------------------------------------------------------------------------*/
cstatic glm::vec2 sBall::calcIncremento(float fDeltaTime, glm::vec2 velBola)
{
    glm::vec2 inc = (velBola * fDeltaTime);
    return inc;
}


/*------------------------------------------------------------------------*\
|* Hacemos el uso de la instancia sGame:
|* - para tener acceso a los variables de ancho y alto, que precisamente
|*   son variables por culpa del 'onsize' de la ventana.
\*------------------------------------------------------------------------*/
int sBall::getWidth()
{
    return sGame::instancia()->getRender()->getWidth();
}


int sBall::getHeight()
{
    return sGame::instancia()->getRender()->getWidth();
}
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*\
|* Fin de sBall.cpp
\*------------------------------------------------------------------------*/
