/*------------------------------------------------------------------------*\
|* sPhysicsSystem.h
\*------------------------------------------------------------------------*/

#include "sPhysicsSystem.h"
#include "../sGame.h"
#include "../sBall.h"
#include <vector>


/*------------------------------------------------------------------------*/
sPhysicsSystem::sPhysicsSystem()
{
}

sPhysicsSystem::~sPhysicsSystem()
{
    cLog::print(" Destructor sPhysicsSystem\n");
}
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*/
int sPhysicsSystem::update(sGame* pGame, float fDeltaTime)
{
    auto& vecBolas = pGame->getVecBolas();
    if (!pGame->hayPausa())
    {
        if (vecBolas.size() > 0)
        {
            // Movimiento de bolas
            for (auto* pBall : vecBolas)
            {
                pBall->update(fDeltaTime);
            }

            // Destruccion de bolas
            while (pGame->hayDestruccion())
            {
                // Nos volvemos a recorrer el vector para destruir las bolas que se hubieran parado:
                for (int i = 0; i < vecBolas.size(); i++)
                {
                    auto* pBall = vecBolas[i];
                    if (pBall->m_destruir)
                    {
                        pGame->setDestruccion(true);
                        // Borramos este:
                        vecBolas.erase(vecBolas.begin() + i);
                        cLog::print(" Destruimos %ld: %ld\n", i, pBall->m_bolaId);
                        delete pBall;
                        break;
                    }
                    pGame->setDestruccion(false);
                }
                // si Nos hemos salido con break, queda por destruir, debemos salir habiendo recorrido todos
                // y que no hay ninguno pendiente de destruir.
            }
        }
    }
    return 0;
}


/*------------------------------------------------------------------------*\
|* Fin de sPhysicsSystem.h
\*------------------------------------------------------------------------*/
