/*------------------------------------------------------------------------*\
|* sExplosion.cpp
\*------------------------------------------------------------------------*/

#include "sExplosion.h"
#include "sGame.h"
#include "sGlobal.h"
#include "sBall.h"
#include <swat/sOpenGL.h>
#include <tool/cRandom.hpp>
#include <tool/cLog.h>
#include <tool/sMath.h>


/*------------------------------------------------------------------------*/
sExplosion::sExplosion(sBall* pBola)
    : m_explId(pBola->m_bolaId)
    , m_posicion(pBola->m_posicion)
    , m_radio(pBola->m_radio)
    , m_color(pBola->m_color)
    , m_fdir(pBola->m_fdir)
    , m_vecVelocidad(pBola->m_vecVelocidad)
    , m_tiempo(0.0f)
{
    cRandom* pMotor = sGame::instancia()->getMotor();

    int iTrozos = static_cast<int>(pMotor->aleatorio(6, 30));
    bool rectObola = false;
    for (int i = 0; i < iTrozos; i++)
    {
        sTrozo trozo;
        trozo.m_fdir = static_cast<float>(pMotor->aleatorio(1.0f, 360.0f));
        trozo.m_pos = m_posicion;
        trozo.m_fradio = m_radio/static_cast<float>(pMotor->aleatorio(4.0f, iTrozos));
        trozo.m_velocidad = static_cast<float>(pMotor->aleatorio(150.0f, 350.0f));
        rectObola = !rectObola;
        trozo.rectObola = rectObola;
        m_vecTrozo.push_back(trozo);
    }
}

sExplosion::~sExplosion()
{
    cLog::print(" Destructor sExplosion\n");
}
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*/
int sExplosion::update(float fDeltaTime)
{
    // Deberia modificar en el update
    m_tiempo += fDeltaTime;
    if (m_vecTrozo.size())
    {
        for (auto& trozo : m_vecTrozo)
        {
            trozo.m_pos.x = trozo.m_pos.x + trozo.m_velocidad * fDeltaTime * cos(deg2rad(trozo.m_fdir));
            trozo.m_pos.y = trozo.m_pos.y + trozo.m_velocidad * fDeltaTime * sin(deg2rad(trozo.m_fdir));
            trozo.m_fRotate += fDeltaTime * 100.0f;
        }
    }
    if (m_tiempo > sGlobal::m_fTiempoExplosion)
        return 1;

    return 0;
}


int sExplosion::render()
{
    // Y en el render solo renderizar
    if (m_vecTrozo.size())
    {
        for (auto& trozo : m_vecTrozo)
        {
            glm::vec3 pos = { trozo.m_pos.x, trozo.m_pos.y, 0.0f };
            float fradio = trozo.m_fradio;

            if (trozo.rectObola)
                sOpenGL::rectangulo(eCoordRectangulo::eCenter, GL_FILL, pos, fradio, fradio / 3.0f, m_color, trozo.m_fRotate);
            else
                sOpenGL::circulo(pos, fradio, m_color, 2.0f);
        }
    }
    return 0;
}
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*\
|* Fin de sExplosion.h
\*------------------------------------------------------------------------*/
