/*------------------------------------------------------------------------*\
|* sBall.h
\*------------------------------------------------------------------------*/

#pragma once
/*------------------------------------------------------------------------*\
|* Includes
\*------------------------------------------------------------------------*/
#include <GLM/glm.hpp>
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*\
|* Enumerados
\*------------------------------------------------------------------------*/
enum class eIncrGrados
{
    eSuma,
    eResta
};
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*\
|* struct sBall
\*------------------------------------------------------------------------*/
struct sBall
{
    sBall();
    virtual ~sBall();

    virtual int update(float fDeltaTime);
    virtual int render();
    virtual int checkLimites(float fDeltaTime, int width, int height);
    virtual int checkPosicion(int width, int hight);
    virtual int checkParada(float fDeltaTime);
    virtual int matarBola(float fDeltaTime);

    void cambiaDir(eIncrGrados incr);

    int m_bolaId{ -1 };
    glm::vec2 m_posicion{ 0.0f, 0.0f };
    float m_radio{ 1.0f };
    glm::vec4 m_color{ 1.0f, 0.0f, 0.0f, 1.0f };
    float m_fdir{ 0.0f };
    glm::vec2 m_vecVelocidad{ 0.0f, 0.0f };
    bool m_estaColisionando{ false };
    bool m_check{ false };
    bool m_parando{ false };
    bool m_destruir{ false };
    float m_tiempo{ 0.0f };

    static glm::vec2 calcIncremento(float fDeltaTime, glm::vec2 velBola);

    static bool m_hayGravedad;
    static bool m_hayFriccion;
    static int s_countBolas;
};
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*\
|* Fin de sBall.h
\*------------------------------------------------------------------------*/
