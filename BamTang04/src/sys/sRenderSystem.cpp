                                                                                                /*------------------------------------------------------------------------*\
|* sRenderSystem.cpp
\*------------------------------------------------------------------------*/
#include "sRenderSystem.h"
#include "../sGameWindow.h"
#include "../sGame.h"
#include "../sBall.h"
#include "../sGlobal.h"
#include "../sOrigen.h"
#include <tool/consola/cConsola.h>
#include <swat/cColor.h>
#include <swat/texturas/cGestorFuentes.h>


/*------------------------------------------------------------------------*/
sRenderSystem::sRenderSystem(int width, int height)
    : m_initWidth(width)
    , m_initHeight(height)
{
}

sRenderSystem::~sRenderSystem()
{
    delete m_pWindow;
    cLog::print(" Destructor sRenderSystem\n");
}
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*/
int sRenderSystem::init()
{
    m_pWindow = new sGameWindow(m_initWidth, m_initHeight);

    miError(m_pWindow->crea(
        0, 0,
        m_initWidth, m_initHeight,
        0.01f, 100.0f, 45.0f,
        32, 32,
        false,
        eEstiloW::eCaption,
        cConsola::getNombreProceso(),
        "OpenGL Window Class",
        nullptr
    ));

    m_pWindow->muestraVentana();
    return 0;
}


/*------------------------------------------------------------------------*/
int sRenderSystem::finishInit(sGame * pGame)
{
    m_pOrigen = pGame->getMainOrigin();
    m_pTexturaTest = pGame->getTextura();
    m_pFonArialMin = pGame->getFonArialMin();
    m_pFonArialMax = pGame->getFonArialMax();
    m_pFonDef = pGame->getFonDef();
    m_pFonAgulon = pGame->getFonAgulon();

    return 0;
}


/*------------------------------------------------------------------------*/
int sRenderSystem::update(sGame* pGame, float fDeltaTime)
{
    m_pWindow->clean();
    m_pWindow->begin();     // Escritura/dibujado en Primer plano
    {
        showFps(m_pFonDef, fDeltaTime);
        showOrigin(m_pFonArialMin);
        showTest(pGame);

        auto& vecBolas = pGame->getVecBolas();
        for (auto* pBall : vecBolas)
        {
            pBall->render(fDeltaTime);
        }

        showHelp(pGame,m_pFonArialMax);
    }
    m_pWindow->end();       // Termina la escritura en primer plano
    m_pWindow->swap();

    return 0;
}


/*------------------------------------------------------------------------*/
void sRenderSystem::showOrigin(cFuente* pFon)
{
    int x = static_cast<int>(40);
    int y = static_cast<int>(getHeight()-30);

    mDo(pFon)->detalleTitulo(
        m_pWindow, 
        glm::vec3(x, y, 0.0f),
        1.0f, 
        cColor::vVerde,
        cColor::vAmarillo,
        "Velocidad:",
        "%6.3f", sGlobal::m_fVelocidadInicial);

    float fdir = m_pOrigen->m_fdir;

    mDo(pFon)->detalleTituloSiguiente(
        m_pWindow, 0, 0,
        1.0f,
        cColor::vVerde,
        cColor::vAmarillo,
        "Dir:",
        "%6.3f", fdir);
}


/*------------------------------------------------------------------------*/
void sRenderSystem::showFps(cFuente* poFon, float fDeltaTime)
{
    char vcFps[32];
    float fps = 1 / fDeltaTime;
    sprintf_s(vcFps, sizeof(vcFps) - 1, "%6.3f", fps);

    int Y = static_cast<int>(getHeight() - 25);
    poFon->escribe(
        m_pWindow,
        glm::ivec3(getWidth() - 220, Y, 0),
        1.0f,
        cColor::vBlanco,
        "Fps:");

    poFon->escribe(
        m_pWindow,
        glm::ivec3(getWidth() - 135, Y, 0),
        1.0f,
        cColor::vBlanco,
        vcFps);
}


/*------------------------------------------------------------------------*/
void sRenderSystem::showTest(sGame * pGame)
{
    if (pGame->hayTest())
    {
        float x = getWidth() / 4.0f;
        float y = (getHeight() / 4.0f) * 3.0f;
        float fancho = getWidth() / 2.0f;
        float falto = getHeight() / 2.0f;

        //------------------------------------------------------------------
        // Porque el color en curso es fundamental ?
        //------------------------------------------------------------------
        sOpenGL::color(cColor::vBlanco);
        //------------------------------------------------------------------
        sOpenGL::textura(x, y, 0.0f, fancho, falto, m_pTexturaTest);
    }
}

/*------------------------------------------------------------------------*/
void sRenderSystem::showHelp(sGame * pGame, cFuente* poFon)
{
    if (pGame->hayHelp())
    {
        float x = getWidth() / 4.0f;
        float y = (getHeight() / 4.0f) * 3.0f;
        float fancho = getWidth() / 2.0f;
        float falto = getHeight() / 2.0f;

        // Textura blanca transparente
        poFon->escribe(
            m_pWindow,
            glm::ivec3(static_cast<int>(x), static_cast<int>(y), 0),
            1.0f,
            cColor::vBlanco, "Mostramos la Ayuda");
    }
}
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*\
|* Fin de sRenderSystem.cpp
\*------------------------------------------------------------------------*/
