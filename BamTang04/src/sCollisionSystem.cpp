/*------------------------------------------------------------------------*\
|* sCollisionSystem.cpp
\*------------------------------------------------------------------------*/

#include "sCollisionSystem.h"
#include "sGame.h"
#include "sBall.h"
#include "sGlobal.h"
#include <tool/sMath.h>
#include <vector>


/*------------------------------------------------------------------------*\
|* Statics
\*------------------------------------------------------------------------*/
int sCollisionSystem::m_iWidth = 0;
int sCollisionSystem::m_iHeight = 0;
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*/
sCollisionSystem::sCollisionSystem(int width, int height)
{
    m_iWidth = width;
    m_iHeight = height;
}

sCollisionSystem::~sCollisionSystem()
{
    cLog::print(" Destructor sCollisionSystem\n");
}
/*------------------------------------------------------------------------*/


void sCollisionSystem::updateLimits(int width, int height)
{
    m_iWidth = width;
    m_iHeight = height;
}


int sCollisionSystem::update(sGame*pGame, float fDeltaTime)
{
    auto& vecBolas = pGame->getVecBolas();
    if (!pGame->hayPausa())
    {
        if (vecBolas.size() > 0)
        {
            for (auto* pBall : vecBolas)
            {
                pBall->checkLimites(fDeltaTime, m_iWidth, m_iHeight);
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
                    if (!pBall1->m_check)
                    {
                        //--------------------------------------------------
                        for (int j = 0; j < vecBolas.size(); j++)
                        {
                            if (i == j)
                                continue;

                            sBall* pBall2 = vecBolas[j];
                            if (!pBall2->m_check)
                            {
                                if (
                                    checkCollision(fDeltaTime, pBall1, pBall2) ||
                                    checkCollision(0, pBall1, pBall2)
                                   )
                                {
                                    collision(pBall1, pBall2);
                                    float fVel1 = sMath::modulo(pBall1->m_vecVelocidad);
                                    float fVel2 = sMath::modulo(pBall2->m_vecVelocidad);
                                    cLog::print(" Vel Bola %6.3f  ---  Vel Bola %6.3f\n",
                                        fVel1,
                                        fVel2);
                                    break;
                                }
                            }
                        }
                        //--------------------------------------------------
                    }
                    pBall1->m_check = true;
                }
            }
        }
    }
    return 0;
}


bool sCollisionSystem::checkCollision(float fDeltaTime, sBall* bola1, sBall* bola2)
{
    if (!bola1->m_check && !bola2->m_check)
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

            cLog::print(" Collision: Bola %3ld -- Bola %3ld     Delta: %6.4f\n",
                bola1->m_bolaId,
                bola2->m_bolaId,
                fDeltaTime);
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
        float fVel, xVel, yVel;

        glm::vec2 vdiff = (pBola1->m_posicion) - (pBola2->m_posicion);

        float fdir = rad2deg(atan2f(vdiff.y, vdiff.x));

        pBola1->m_fdir = fdir;
        pBola2->m_fdir = ((fdir + 180.0f) > 360.0f) ? (fdir + 180.0f) - 360.0f : fdir + 180.0f;

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

        // La velocidad: la anterior repartida aplicando la reduccion de elasticidad:
        fVel = oldVel1 * sGlobal::m_fElasticidad;
        xVel = fVel * cos(deg2rad(pBola1->m_fdir));
        yVel = fVel * sin(deg2rad(pBola1->m_fdir));

        pBola1->m_vecVelocidad = glm::vec2{ xVel, yVel };

        // La velocidad: la anterior repartida aplicando la reduccion de elasticidad:
        fVel = oldVel2 * sGlobal::m_fElasticidad;
        xVel = fVel * cos(deg2rad(pBola2->m_fdir));
        yVel = fVel * sin(deg2rad(pBola2->m_fdir));

        pBola2->m_vecVelocidad = glm::vec2{ xVel, yVel };
    }
}


void sCollisionSystem::collision_0(sBall* pBola1, sBall* pBola2)
{
    float oldVel1 = sMath::modulo(pBola1->m_vecVelocidad);
    float oldVel2 = sMath::modulo(pBola2->m_vecVelocidad);
    // Para repartir sumo y reparto:
    float suma = oldVel1 + oldVel2;

    if (suma > 0.0f)
    {
        pBola1->m_check = true;
        pBola2->m_check = true;

        //------------------------------------------------------------------
        // Primero las velocidades compartidas/repartidas
        //------------------------------------------------------------------
        oldVel1 = (oldVel1 + 1) / suma;
        oldVel2 = (oldVel2 + 1) / suma;

        oldVel1 = oldVel1 * suma;
        oldVel2 = oldVel2 * suma;

        // La velocidad: la anterior repartida aplicando la reduccion de elasticidad:
        float fNewVel1 = oldVel1 * sGlobal::m_fElasticidad;
        float fNewVel2 = oldVel2 * sGlobal::m_fElasticidad;
        //------------------------------------------------------------------

        //------------------------------------------------------------------
        // Y las direcciones
        // La direccion perpendicular al choque:
        //------------------------------------------------------------------
        glm::vec2 vdiff = (pBola1->m_posicion) + (pBola2->m_posicion);

        float fdir = rad2deg(atan2f(vdiff.y, vdiff.x));

        float fNewDir1 = fdir;
        float fNewDir2 = ((fdir + 180.0f) > 360.0f) ? (fdir + 180.0f) - 360.0f : fdir + 180.0f;

        float xVec, yVec;

        // vec direccion 1 nuevo
        xVec = cos(deg2rad(fNewDir1));
        yVec = sin(deg2rad(fNewDir1));
        glm::vec2 vecNewDir1 = { xVec, yVec };

        // vec direccion 2 nuevo
        xVec = cos(deg2rad(fNewDir2));
        yVec = sin(deg2rad(fNewDir2));
        glm::vec2 vecNewDir2 = { xVec, yVec };

        // vec direccion 1 actual
        xVec = cos(deg2rad(pBola1->m_fdir));
        yVec = sin(deg2rad(pBola1->m_fdir));
        glm::vec2 vecDir1 = { xVec, yVec };

        // vec direccion 2 actual
        xVec = cos(deg2rad(pBola2->m_fdir));
        yVec = sin(deg2rad(pBola2->m_fdir));
        glm::vec2 vecDir2 = { xVec, yVec };

        //------------------------------------------------------------------
        // Combinando las direcciones
        // Probando la resta:
        //------------------------------------------------------------------
        glm::vec2 vecTotDir1 = vecDir1 - vecNewDir1;
        glm::vec2 vecTotDir2 = vecDir2 - vecNewDir2;

        float fdir1 = sMath::getAngulo(vecTotDir1);
        float fdir2 = sMath::getAngulo(vecTotDir2);

        pBola1->m_fdir = fdir1;
        pBola2->m_fdir = fdir2;

        xVec = fNewVel1 * cos(deg2rad(pBola1->m_fdir));
        yVec = fNewVel1 * sin(deg2rad(pBola1->m_fdir));
        pBola1->m_vecVelocidad = { xVec, yVec };

        xVec = fNewVel2 * cos(deg2rad(pBola2->m_fdir));
        yVec = fNewVel2 * sin(deg2rad(pBola2->m_fdir));
        pBola2->m_vecVelocidad = { xVec, yVec };
    }
}


/*------------------------------------------------------------------------*\
|* Fin de sCollisionSystem.cpp
\*------------------------------------------------------------------------*/
