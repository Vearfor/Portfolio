/*------------------------------------------------------------------------*\
|* sCollisionSystem.cpp
\*------------------------------------------------------------------------*/

#include "sCollisionSystem.h"
#include "../sGame.h"
#include "../sBall.h"
#include "../sGlobal.h"
#include "sRenderSystem.h"
#include <tool/sMath.h>
#include <vector>


/*------------------------------------------------------------------------*\
|* Statics
\*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*/
sCollisionSystem::sCollisionSystem()
{
}

sCollisionSystem::~sCollisionSystem()
{
    cLog::print(" Destructor sCollisionSystem\n");
}
/*------------------------------------------------------------------------*/


int sCollisionSystem::update(sGame* pGame, float fDeltaTime)
{
    auto& vecBolas = pGame->getVecBolas();
    if (!pGame->hayPausa())
    {
        if (vecBolas.size() > 0)
        {
            for (auto* pBall : vecBolas)
            {
                pBall->checkLimites(fDeltaTime, getWidth(), getHeight());
                // Los flags los reseteamos para la colision entre Bolas:
                pBall->m_check = false;
                pBall->m_estaColisionando = false;
            }

            // Al menos 2 bolas
            if (vecBolas.size() > 1)
            {
                // Bola contra Bola:
                for (int i = 0; i < vecBolas.size(); i++)
                {
                    sBall* pBall1 = vecBolas[i];
                    if (!pBall1->m_check && !sMath::isZero(pBall1->m_vecVelocidad))
                    {
                        //--------------------------------------------------
                        for (int j = 0; j < vecBolas.size(); j++)
                        {
                            sBall* pBall2 = vecBolas[j];

                            // si son la misma, continuamos
                            if (pBall1->m_bolaId == pBall2->m_bolaId)
                                continue;

                            if (!pBall2->m_check && !pBall2->m_explotando)
                            {
                                if (checkCollision(fDeltaTime, pBall1, pBall2))
                                {
                                    collision(pBall1, pBall2);
                                    //float fVel1 = sMath::modulo(pBall1->m_vecVelocidad);
                                    //float fVel2 = sMath::modulo(pBall2->m_vecVelocidad);
                                    //cLog::print(" Vel Bola %6.3f  ---  Vel Bola %6.3f\n",
                                    //    fVel1,
                                    //    fVel2);
                                    break;
                                }
                            }
                        }
                        //--------------------------------------------------
                        // Marcamos las ya comprobadas con todos.
                        //--------------------------------------------------
                        pBall1->m_check = true;
                        //--------------------------------------------------
                    }
                }
            }
        }
    }
    return 0;
}


bool sCollisionSystem::checkCollision(float fDeltaTime, sBall* bola1, sBall* bola2)
{
    // if (!bola1->m_check && !bola2->m_check)
    {
        // Cuando el tiempo es 0, el inc es 0
        glm::vec2 inc1 = (bola1->m_vecVelocidad * fDeltaTime);
        glm::vec2 inc2 = (bola2->m_vecVelocidad * fDeltaTime);

        // Y si el inc es 0, es la comprobacion en la posicion actual
        glm::vec2 pos1 = bola1->m_posicion + inc1;
        glm::vec2 pos2 = bola2->m_posicion + inc2;

        glm::vec2 diff = pos1 - pos2;
        float dist = sMath::modulo(diff);
        float f2radio = bola1->m_radio + bola2->m_radio;
        if (dist < f2radio)
        {
            bola1->m_estaColisionando = true;
            bola1->m_estaColisionando = true;
            //cLog::print(" Collision: Bola %3ld -- Bola %3ld     Delta: %6.4f\n",
            //    bola1->m_bolaId,
            //    bola2->m_bolaId,
            //    fDeltaTime);
            return true;
        }
    }

    return false;
}


void sCollisionSystem::collision(sBall* pBola1, sBall* pBola2)
{
    float oldVel1 = sMath::modulo(pBola1->m_vecVelocidad);
    float oldVel2 = sMath::modulo(pBola2->m_vecVelocidad);
    // Para repartir sumo y reparto:
    float suma = oldVel1 + oldVel2;

    if (suma > 0.0f)
    {
        glm::vec2 vdiff = (pBola1->m_posicion) - (pBola2->m_posicion);

        float fdir = rad2deg(atan2f(vdiff.y, vdiff.x));

        float fdir1 = fdir;
        float fdir2 = ((fdir + 180.0f) > 360.0f) ? (fdir + 180.0f) - 360.0f : fdir + 180.0f;

        // Si laguna es negativa, la mostramos siempre como positiva:
        fdir1 = sMath::corrigeDir(fdir1);
        fdir2 = sMath::corrigeDir(fdir2);

        // Estas son las direcciones perpendiculares al choque, ¿no habria que tener en cuenta las direcciones anteriores?
        // Debería ser, y seguro que es mucho mas simple
        if (oldVel1 > oldVel2)
        {
            float quito = oldVel1 * 0.1f;
            oldVel1 = oldVel1 - quito;
            oldVel2 = oldVel2 + quito;
        }
        else
        {
            float quito = oldVel2 * 0.1f;
            oldVel2 = oldVel2 - quito;
            oldVel1 = oldVel1 + quito;
        }

        pBola1->modifyByCollision(oldVel1, fdir1);
        pBola2->modifyByCollision(oldVel2, fdir2);
    }
}


/*------------------------------------------------------------------------*\
|* Hacemos el uso de la instancia sGame:
\*------------------------------------------------------------------------*/
int sCollisionSystem::getWidth()
{
    return sGame::instancia()->getRender()->getWidth();
}


int sCollisionSystem::getHeight()
{
    return sGame::instancia()->getRender()->getHeight();
}
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*\
|* Fin de sCollisionSystem.cpp
\*------------------------------------------------------------------------*/
