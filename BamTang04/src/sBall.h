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
|* struct sBall
\*------------------------------------------------------------------------*/
struct sRenderSystem;
struct sExplosion;
/*------------------------------------------------------------------------*/
struct sBall
{
    sBall();
    virtual ~sBall();

    virtual void modifyByCollision(float oldVel, float fdir);
    virtual int update(float fDeltaTime);
    virtual int checkParada(float fDeltaTime);
    virtual int matarBola(float fDeltaTime);
    virtual int render();

    int checkLimites(float fDeltaTime, int width, int height);
    int checkPosicion(int width, int hight);

    void render_normal();
    void render_explosion();
    int aplicoGravedad(float fDeltaTime);
    int aplicoFriccionAire(float fDeltaTime);

    int m_bolaId{ -1 };
    glm::vec2 m_posicion{ 0.0f, 0.0f };
    float m_radio{ 1.0f };
    glm::vec4 m_color{ 1.0f, 0.0f, 0.0f, 1.0f };
    float m_fdir{ 0.0f };
    glm::vec2 m_vecVelocidad{ 0.0f, 0.0f };
    bool m_estaColisionando{ false };
    bool m_check{ false };
    bool m_parando{ false };
    bool m_explotando{ false };
    bool m_destruir{ false };
    float m_tiempo{ 0.0f };
    sExplosion* m_pExplosion{ nullptr };

    static glm::vec2 calcIncremento(float fDeltaTime, glm::vec2 velBola);

    static int s_countBolas;

protected:

    int getWidth();
    int getHeight();
};
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*\
|* Fin de sBall.h
\*------------------------------------------------------------------------*/
