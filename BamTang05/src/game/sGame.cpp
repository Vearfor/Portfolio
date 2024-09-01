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

    //// Update de la animacion para los dos destinos: m_desAnim, y el destino de m_hero
    //m_animKnight->update(deltaTime);

    //// Update del movimiento del hero
    //m_hero->update(deltaTime, kMax_w - 1, m_desAnim.x + (2 * m_desAnim.w) + 30);

    //if (m_pKeyboard->GetButtonDown(eActions::eTest))
    //    cLog::log(" down");

    //if (m_pKeyboard->GetButtonUp(eActions::eTest))
    //    cLog::log(" up\n");

    //if (m_pKeyboard->GetButton(eActions::eTest))
    //    cLog::log(" button\n");

    //if (m_pKeyboard->GetButtonUp(eActions::eEscape))
    //{
    //    cLog::log(" up Escape\n");
    //    m_isRunning = false;
    //}

    //cLog::log(" Frame %7d   Delta: %f\n", m_numFrames, deltaTime);

    return 0;
}


int sGame::render()
{
    m_pRender->render();

    return 0;
}


//int sGame::init()
//{
//    ...
//
//    //m_pHandler = new cInputHandler();
//    //m_pKeyboard = new cKeyboard();
//    //initKeyboard();
//    //m_pHandler->addController(m_pKeyboard);
//
//    ...
//
//    ////----------------------------------------------------------------------
//    //// Probando Animacion Knight + rotate
//    ////----------------------------------------------------------------------
//    //m_hero = new cKnight();
//    ...
//    //m_hero->setRenderer(m_pRenderer);
//    //m_hero->setController(m_pKeyboard);
//    //m_hero->setAnimacion(m_animKnight);
//    //m_hero->setPosDestino(&m_desRotate);
//    //m_hero->init();
//
//    //m_isRunning = true;
//
//    return 0;
//}

//int sGame::initKeyboard()
//{
//    //auto button1 = InputButton(eGiraDerecha, SDLK_RIGHT);   m_pKeyboard->addButtonMap(button1);
//    //auto button2 = InputButton(eGiraIzquierda, SDLK_LEFT);  m_pKeyboard->addButtonMap(button2);
//    //auto button3 = InputButton(eEscape, SDLK_ESCAPE);       m_pKeyboard->addButtonMap(button3);
//    //auto button4 = InputButton(eTest, SDLK_TAB);            m_pKeyboard->addButtonMap(button4);
//    return 0;
//}


/*------------------------------------------------------------------------*\
|* Fin de sGame.cpp
\*------------------------------------------------------------------------*/
