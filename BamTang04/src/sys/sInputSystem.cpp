/*------------------------------------------------------------------------*\
|* sInputSystem.cpp
\*------------------------------------------------------------------------*/

#include "sInputSystem.h"
#include "sRenderSystem.h"
#include "../sGame.h"
#include "../sGameWindow.h"
#include "../sOrigen.h"
#include "../sGlobal.h"
#include <swat/input/cTeclado.h>
#include <swat/input/cRaton.h>


/*------------------------------------------------------------------------*/
sInputSystem::sInputSystem()
{
}

sInputSystem::~sInputSystem()
{
    cLog::print(" Destructor sInputSystem\n");
}
/*------------------------------------------------------------------------*/


int sInputSystem::update(sGame* pGame, float fDeltaTime)
{
    sGameWindow* pWindow = pGame->getRender()->getMainWindow();
    sOrigen* pOrigen = pGame->getMainOrigin();
    cTeclado* pTeclado = pWindow->m_pTeclado;
    if (pTeclado)
    {
        /*----------------------------------------------------------------*\
        |* There is the call of asked function
        \*----------------------------------------------------------------*/
        if (pTeclado->isUp(VK_SPACE))
        {
            pGame->shootBall();
        }
        /*----------------------------------------------------------------*/

        if (pTeclado->isDown(VK_ESCAPE))
        {
            pWindow->destruyeVentana();
        }

        if (pTeclado->isUp('P'))
        {
            pGame->togglePausa();
        }

        if (pTeclado->isUp('G'))
        {
            sBall::m_hayGravedad = !sBall::m_hayGravedad;
        }

        if (pTeclado->isUp('F'))
        {
            sBall::m_hayFriccion = !sBall::m_hayFriccion;
        }

        if (pTeclado->isUp('R'))
        {
            pOrigen->m_posicion = sGlobal::m_posOrigen;
        }

        if (pTeclado->isUp('S'))
        {
            pOrigen->m_vecVelocidad = { 0.0f, 0.0f };
        }

        if (pTeclado->isDown('A'))
        {
            pOrigen->cambiaVel(eIncr::eSuma, fDeltaTime);
        }

        if (pTeclado->isDown('D'))
        {
            pOrigen->cambiaVel(eIncr::eResta, fDeltaTime);
        }

        if (pTeclado->isUp(VK_F1))
        {
            pGame->toggleHelp();
        }

        if (pTeclado->isUp('T'))
        {
            pGame->toggleTest();
        }

        /*----------------------------------------------------------------*\
        |* Directions Keys: to move origin object
        \*----------------------------------------------------------------*/
        if (pTeclado->isDown(VK_LEFT))
        {
            pOrigen->izquierda(fDeltaTime);
        }
        if (pTeclado->isDown(VK_RIGHT))
        {
            pOrigen->derecha(fDeltaTime);
        }
        if (pTeclado->isDown(VK_UP))
        {
            pOrigen->arriba(fDeltaTime);
        }
        if (pTeclado->isDown(VK_DOWN))
        {
            pOrigen->abajo(fDeltaTime);
        }
        /*----------------------------------------------------------------*/
        if (pTeclado->isDown(VK_ADD))
        {
            pOrigen->cambiaDir(eIncr::eSuma, fDeltaTime);
        }
        if (pTeclado->isDown(VK_SUBTRACT))
        {
            pOrigen->cambiaDir(eIncr::eResta, fDeltaTime);
        }
        /*----------------------------------------------------------------*/

        pTeclado->reset();
    }

    cRaton* pMouse = pWindow->m_pRaton;
    if (pMouse)
    {
        if (pMouse->LeftButtonPressed())
        {
            if (pGame->selectedOrigin(pMouse->getLastPos()))
            {
                pOrigen->m_posicion =
                {
                    pMouse->getLastPos().x,
                    getHeight() - pMouse->getLastPos().y
                };
            }
        }
    }

    return 0;
}


/*------------------------------------------------------------------------*\
|* Hacemos el uso de la instancia sGame:
\*------------------------------------------------------------------------*/
int sInputSystem::getHeight()
{
    return sGame::instancia()->getRender()->getHeight();
}
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*\
|* Fin de sInputSystem.cpp
\*------------------------------------------------------------------------*/
