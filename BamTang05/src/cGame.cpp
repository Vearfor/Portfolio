/*------------------------------------------------------------------------*\
|*
|* cGame.cpp
|*
\*------------------------------------------------------------------------*/


#include "cGame.h"
#include "sGlobal.h"
#include "laberinto/sMyMaze.h"
#include "vistas/sVistaConsola.h"
#include "vistas/sVistaSDL.h"
#include "tool/nComun.h"


cGame::~cGame()
{
    delete m_pVistaConsola;
    delete m_pLaberinto;
}


int cGame::init()
{
    if (!m_isRunning)
    {
        m_pLaberinto = new sMyMaze();
        miError(m_pLaberinto->createMaze2D(sGlobal::m_iDim));

        m_pVistaConsola = new sVistaConsola();
        miError(
            m_pVistaConsola->inicia(m_pLaberinto) ||
            m_pVistaConsola->update() ||
            m_pVistaConsola->dibuja(m_pLaberinto)
        );

        m_pVistaSDL = new sVistaSDL();
        miError(m_pVistaSDL->inicia(m_pLaberinto));

        m_isRunning = true;
    }

    return 0;
}


bool cGame::isRunning()
{
    return m_isRunning;
}


int cGame::eventos()
{
    //m_pHandler->ResetEvent();

    m_isRunning = !m_pVistaSDL->eventos();

    return 0;
}


int cGame::update(float deltaTime)
{
    m_numFrames++;      // Por tener un numero que me de el numero de frames reales desde que empieza la ejecucion.

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


int cGame::render()
{
    m_pVistaSDL->dibuja(m_pLaberinto);

    return 0;
}


//int cGame::init()
//{
//    //miError(initSDL() || CreateWindow(&m_window, &m_renderer));
//
//    //// Aqui hay pensada, depende, parece ser, de la ventana
//    //// no tiene que estar en todas partes, y utilizar cuando se requiera
//    //m_pRenderer = new cRenderer();
//    //m_pRenderer->set(m_renderer);
//    //miError(m_pRenderer->init());
//
//    //m_pHandler = new cInputHandler();
//    //m_pKeyboard = new cKeyboard();
//    //initKeyboard();
//    //m_pHandler->addController(m_pKeyboard);
//
//    //// Hay que construir un gestor de Texturas/Imagenes
//    //// Si una textura ya esta cargada, devolvemos la textura
//    //m_texLogo = new cTextura(m_pRenderer, LOGO_FILE);
//    //miError(m_texLogo->init());
//    //m_desLogo.x = 30;
//    //m_desLogo.y = 30;
//    //m_desLogo.w = 100;
//    //m_desLogo.h = 100;
//
//    //m_texPruebaLetra = new cTextura(m_pRenderer, LETRA_FILE);
//    //miError(m_texPruebaLetra->init());
//    //m_desLetra.x = m_desLogo.x + m_desLogo.w;
//    //m_desLetra.y = m_desLogo.y;
//    //m_desLetra.w = 100;
//    //m_desLetra.h = 100;
//
//    //m_texKnight = new cTextura(m_pRenderer, KNIGHT_FILE);
//    //miError(m_texKnight->init());
//    //m_desKnight.x = 30;
//    //m_desKnight.y = m_desLogo.h + 30;
//    //m_desKnight.w = kMax_w - (2 * m_desKnight.x);
//    //m_desKnight.h = 200;
//
//    ////----------------------------------------------------------------------
//    //// Animacion Knight
//    ////----------------------------------------------------------------------
//    //m_animKnight = new cAnimacion(m_pRenderer, m_texKnight, NUM_FRAMES_KNIGHT, 0.25);
//    //miError(m_animKnight->init());
//
//
//    //// Tenemos NUM_FRAMES_KNIGHT imagenes
//    //// Imagen En Destino
//    //m_desAnim.x = m_desKnight.x;
//    //m_desAnim.y = m_desKnight.y + m_desKnight.h + 30;
//    //m_desAnim.w = m_desKnight.w / NUM_FRAMES_KNIGHT;
//    //m_desAnim.h = 200;
//
//    ////----------------------------------------------------------------------
//    //// Probando Animacion Knight + rotate
//    ////----------------------------------------------------------------------
//    //m_hero = new cKnight();
//    //m_desRotate.x = m_desAnim.x + (2 * m_desAnim.w) + 30;
//    //m_desRotate.y = m_desAnim.y;
//    //m_desRotate.w = m_desAnim.w;
//    //m_desRotate.h = m_desAnim.h;
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

//int cGame::initKeyboard()
//{
//    //auto button1 = InputButton(eGiraDerecha, SDLK_RIGHT);   m_pKeyboard->addButtonMap(button1);
//    //auto button2 = InputButton(eGiraIzquierda, SDLK_LEFT);  m_pKeyboard->addButtonMap(button2);
//    //auto button3 = InputButton(eEscape, SDLK_ESCAPE);       m_pKeyboard->addButtonMap(button3);
//    //auto button4 = InputButton(eTest, SDLK_TAB);            m_pKeyboard->addButtonMap(button4);
//    return 0;
//}

