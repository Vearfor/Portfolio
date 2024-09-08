/*------------------------------------------------------------------------*\
|* sGame.cpp
\*------------------------------------------------------------------------*/

#include "sGame.h"
#include "sGlobal.h"
#include "laberinto/sMyMaze.h"
#include "sRenderSystem.h"


sGame::sGame()
{
}

sGame::~sGame()
{
    delete m_pRender;
    delete m_pLaberinto;
}


int sGame::init()
{
    if (!m_isRunning)
    {
        m_pLaberinto = new sMyMaze();
        miError(m_pLaberinto->createMaze2D(sGlobal::m_iDim));

        m_pRender = new sRenderSystem();
        miError(m_pRender->init(m_pLaberinto, sGlobal::m_iWidth, sGlobal::m_iHeight));

        m_isRunning = true;
    }

    return 0;
}


bool sGame::isRunning()
{
    return m_isRunning;
}


int sGame::eventos()
{
    m_isRunning = m_pRender->eventos();

    return 0;
}


int sGame::update(float fDeltaTime)
{
    // Por tener un numero que me de el numero de frames reales desde que empieza la ejecucion.
    m_numFrames++;

    m_pRender->update(fDeltaTime);

    return 0;
}


int sGame::render()
{
    m_pRender->render();

    return 0;
}


/*------------------------------------------------------------------------*\
|* Fin de sGame.cpp
\*------------------------------------------------------------------------*/
