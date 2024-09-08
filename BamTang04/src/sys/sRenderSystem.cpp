/*------------------------------------------------------------------------*\
|* sRenderSystem.cpp
\*------------------------------------------------------------------------*/
#include "sRenderSystem.h"
#include "../sGameWindow.h"
#include "../sGame.h"
#include "../sBall.h"
#include <tool/consola/cConsola.h>


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
int sRenderSystem::update(sGame* pGame, float fDeltaTime)
{
    m_pWindow->clean();
    m_pWindow->begin();     // Escritura/dibujado en Primer plano
    {
        auto& vecBolas = pGame->getVecBolas();
        for (auto* pBall : vecBolas)
        {
            pBall->render(fDeltaTime);
        }

        // Porque el color en curso es fundamental ?
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        cTextura* pTextura = pGame->getTextura();
        sOpenGL::textura(getWidth() / 4.0f, (getHeight() / 4.0f)*3.0f, 0.0f, getWidth() / 2.0f, getHeight() / 2.0f, pTextura);
    }
    m_pWindow->end();       // Termina la escritura en primer plano
    m_pWindow->swap();

    return 0;
}
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*\
|* Fin de sRenderSystem.cpp
\*------------------------------------------------------------------------*/
