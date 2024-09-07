/*------------------------------------------------------------------------*\
|* sInputSystem.cpp
\*------------------------------------------------------------------------*/

#include "sInputSystem.h"
#include "sGame.h"
#include "sGameWindow.h"
#include "sOrigen.h"
#include "sGlobal.h"
#include "sCollisionSystem.h"
#include <swat/input/cTeclado.h>
#include <swat/input/cRaton.h>


/*------------------------------------------------------------------------*/
sInputSystem::sInputSystem()
{
}

sInputSystem::~sInputSystem()
{
}
/*------------------------------------------------------------------------*/


int sInputSystem::update(sGame* pGame, float fDeltaTime)
{
    sGameWindow* pWindow = pGame->getMainWindow();
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
            pOrigen->cambiaDir(eIncrGrados::eSuma);
        }
        if (pTeclado->isDown(VK_SUBTRACT))
        {
            pOrigen->cambiaDir(eIncrGrados::eResta);
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
                    (sCollisionSystem::m_iHeight - sGlobal::m_windowCaptionSize) - pMouse->getLastPos().y
                };
            }
        }
    }

    return 0;
}


/*------------------------------------------------------------------------*\
|* Fin de sInputSystem.cpp
\*------------------------------------------------------------------------*/
