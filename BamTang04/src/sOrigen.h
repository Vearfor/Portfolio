/*------------------------------------------------------------------------*\
|* sOrigen.h
\*------------------------------------------------------------------------*/

#pragma once
/*------------------------------------------------------------------------*\
|* Includes
\*------------------------------------------------------------------------*/
#include "sBall.h"
#include <string>
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*\
|* Enumerados
\*------------------------------------------------------------------------*/
enum class eIncr
{
    eSuma,
    eResta
};
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*\
|* struct sOrigen
\*------------------------------------------------------------------------*/
struct sOrigen
    : public sBall
{
    sOrigen();
    ~sOrigen();

    virtual void modifyByCollision(float oldVel, float fdir) override;
    virtual int update(float fDeltaTime) override;
    virtual int checkParada(float fDeltaTime) override;
    virtual int matarBola(float fDeltaTime) override;
    virtual int render() override;

    void cambiaDir(eIncr incr, float fDeltaTime);
    void cambiaVel(eIncr incr, float fDeltaTime);

    int izquierda(float fDeltaTime);
    int derecha(float fDeltaTime);
    int arriba(float fDeltaTime);
    int abajo(float fDeltaTime);

    glm::vec4 m_vColorFlecha{ 1.0f };
};
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*\
|* Fin de sOrigen.h
\*------------------------------------------------------------------------*/
