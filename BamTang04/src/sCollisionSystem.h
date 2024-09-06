/*------------------------------------------------------------------------*\
|* sCollisionSystem.h
\*------------------------------------------------------------------------*/

#pragma once
//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

/*------------------------------------------------------------------------*/
struct sGame;
struct sBall;
/*------------------------------------------------------------------------*/
struct sCollisionSystem
{
    sCollisionSystem(int width, int height);
    ~sCollisionSystem();

    void updateLimits(int width, int height);
    int update(sGame*, float fDeltaTime);
    bool checkCollision(float fDeltaTime, sBall* bola1, sBall* bola2);
    void collision(sBall* pBola1, sBall* pBola2);
    void collision_0(sBall* pBola1, sBall* pBola2);

    static int m_iWidth;
    static int m_iHeight;
};
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*\
|* Fin de sCollisionSystem.h
\*------------------------------------------------------------------------*/
