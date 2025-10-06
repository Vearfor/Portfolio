/*------------------------------------------------------------------------*\
|* sGame.cpp
\*------------------------------------------------------------------------*/

#include "sGame.h"
#include "sGlobal.h"
#include "laberinto/sMyMaze.h"
#include "sRenderSystem.h"
#include "vistas/sVista3D.h"
#include "sGameWindow.h"


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
        m_pLaberinto = new sMyMaze(this);
        miError(m_pLaberinto->createMaze2D(sGlobal::m_iDim));

        m_pRender = new sRenderSystem();
        miError(m_pRender->init(m_pLaberinto, sGlobal::m_iWidth, sGlobal::m_iHeight));

        m_pLaberinto->setPlayingDemo(m_hayDemo);
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

    (
        playingDemo(fDeltaTime) ||
        m_pRender->update(fDeltaTime)
    );

    return 0;
}


int sGame::render()
{
    m_pRender->render();

    return 0;
}


/*------------------------------------------------------------------------*\
|* Metodos para la Demo
\*------------------------------------------------------------------------*/
void sGame::demo(bool hayDemo)
{
    m_hayDemo = hayDemo;
}


int sGame::playingDemo(float fDeltaTime)
{
    if (!m_pLaberinto->hayPausa())
    {
        if (m_pLaberinto->isPlayingDemo() && !m_pLaberinto->estaEnElFin())
        {
            if (m_ftimeDemo > sGlobal::m_fIntervaloDemo)
            {
                // Es la primera vez que haya un periodo de inicio mayor:
                m_ftimeDemo = -(sGlobal::m_fIntervaloDemo * 5);
            }

            m_ftimeDemo += fDeltaTime;

            if (m_ftimeDemo > sGlobal::m_fIntervaloDemo)
            {
                m_ftimeDemo = 0;
                playAction();
            }
        }
    }
    return 0;
}


void sGame::playAction()
{
    // Decidimos que se hace y actua:
    int tecla;
    tecla = m_pLaberinto->decideTecla();
    switch (tecla)
    {
        case 'A':
            m_pLaberinto->izquierda();
            break;
        case 'W':
            m_pLaberinto->arriba();
            break;
        case 'D':
            m_pLaberinto->derecha();
            break;
        case 'S':
            m_pLaberinto->abajo();
            break;
        default:
            break;
    }
}
/*------------------------------------------------------------------------*/


sRenderSystem* sGame::getRender()
{
    return m_pRender;
}


/*------------------------------------------------------------------------*\
|* Fin de sGame.cpp
\*------------------------------------------------------------------------*/
